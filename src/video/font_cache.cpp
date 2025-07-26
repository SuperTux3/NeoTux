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
#include <iostream>

FontCache::FontCache(std::string_view fontpath, int font_size) :
	m_font(fontpath, font_size),
	m_strings()
{}

TextureRef
FontCache::load(const std::string &msg, SDL_Color color)
{
	if (m_strings.contains(msg))
		return m_strings[msg];
	else
	{
		SDL_Surface *surface = m_font.render_text_solid(msg, color);
		// TODO error handling

		TextureRef tex = Texture::create(surface);

		SDL_DestroySurface(surface);
		m_strings.insert({msg, tex});
		return tex;
	}
}

