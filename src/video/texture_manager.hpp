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

#ifndef SUPERTUX_SRC_VIDEO_TEXTURE_MANAGER_HPP
#define SUPERTUX_SRC_VIDEO_TEXTURE_MANAGER_HPP

#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include "texture.hpp"
#include "thread_worker.hpp"

class TextureManager
{
public:
	TextureManager();
	~TextureManager();
	
	void init();
	TextureRef load(const std::string &filename, bool as_surface = false);
	void poll_unloaded();
public:
	ThreadWorker m_threads;
	std::list<std::pair<std::string, bool>> m_waiting_load;
	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
	std::unique_ptr<Texture> m_dummy_texture;
};

extern TextureManager g_texture_manager;

#endif
