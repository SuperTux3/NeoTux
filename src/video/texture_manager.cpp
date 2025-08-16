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

#include "texture_manager.hpp"
#include "util/filesystem.hpp"
#include "video/sdl/surface_blitter.hpp"

TextureManager g_texture_manager{};

TextureManager::TextureManager() :
	m_threads(1),
	m_dummy_texture()
{
	
}

TextureManager::~TextureManager()
{
}

void
TextureManager::init()
{
	SurfaceBlitter blitter({64, 64});
	blitter.fill({0, 255, 0, 255});
	blitter.draw_circle(32, 32, 32, {255, 0, 255, 255});
	m_dummy_texture.reset(blitter.to_texture());
}

void
TextureManager::poll_unloaded()
{
	if (m_waiting_load.begin() == m_waiting_load.end())
		return;
	auto p = m_waiting_load.front();
	std::cout << "Here! " << p.first << " " << p.second << std::endl;
	TextureRef tex = Texture::create(FS::path(p.first), p.second);
	m_textures.insert({p.first, std::unique_ptr<Texture>(tex)});
	
	m_waiting_load.pop_front();
}

TextureRef
TextureManager::load(const std::string &filename, bool as_surface)
{
	if (m_textures.contains(filename))
		return m_textures[filename].get();
	else
	{
		bool exists = false;
		for (auto &wait : m_waiting_load)
		{
			if (wait.first == filename && wait.second == as_surface)
				exists = true;
		}
		if (!exists)
			m_waiting_load.emplace_front(filename, as_surface);
		//TextureRef tex = Texture::create(FS::path(filename), as_surface);
		//m_textures.insert({filename, std::unique_ptr<Texture>(tex)});
		return nullptr;
	}
}
