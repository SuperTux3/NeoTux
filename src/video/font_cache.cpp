//  SuperTux 
//  Copyright (C) 2025 Hyland B. <me@ow.swag.toys> 
// 
//  This program is free software: you can redistribute it and/or modify 
//  it under the terms of the GNU General Public License as published by 
//  the Free Software Foundation, either version 3 of the License, or 
//  (at your option) any later version. 
// 
//  This program is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
//  GNU General Public License for more details. 
// 
//  You should have received a copy of the GNU General Public License 
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "font_cache.hpp"
#include "util/logger.hpp"
#include <iostream>

FontCache::FontCache(std::string_view fontpath, int font_size) :
	m_font(fontpath, font_size),
	m_strings()
{}

bool
FontCache::try_gc()
{
	static int64_t last_cleanup = 0;
	if (last_cleanup == 0)
		last_cleanup = SDL_GetTicks();
	// Make GC more aggressive when there are more strings (can happen with constant string updates)
	int64_t GC_TIME = (1 + 15.0/(int)(((m_strings.size())/32.f)+1)) * 1000.f;
	int64_t GC_INTERVAL = GC_TIME;
	bool did_cleanup = false;
	int64_t curr = SDL_GetTicks();
	
	if (last_cleanup >= curr - GC_INTERVAL)
	{
		return false;
	}
	Logger::debug("Trying Font cleanup...");
	last_cleanup = curr;

	// TODO better way?
gc_loop:
	for (auto it = m_strings.begin(); it != m_strings.end(); ++it)
	{
		TextureRef tex = it->second.get();
		if ((int64_t)tex->get_last_used() < curr - GC_TIME)
		{
			Logger::debug("Cleaned up \"" + it->first + "\"");
			m_strings.erase(it);
			did_cleanup = true;
			goto gc_loop;
		}
	}
	return did_cleanup;
}

TextureRef
FontCache::load(const std::string &msg, SDL_Color color)
{
	if (m_strings.contains(msg))
	{
		TextureRef tex = m_strings[msg].get();
		tex->poke_last_used();
		return tex;
	}
	else
	{
		SDL_Surface *surface = m_font.render_text_solid(msg, color);
		// TODO error handling

		TextureRef tex = Texture::create(surface);
		tex->poke_last_used();

		SDL_DestroySurface(surface);
		m_strings.insert({msg, std::unique_ptr<Texture>(tex)});
		return tex;
	}
}

