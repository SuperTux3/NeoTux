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

#ifndef SUPERTUX_SRC_VIDEO_TEXTURE_STORE_HPP
#define SUPERTUX_SRC_VIDEO_TEXTURE_STORE_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include "texture.hpp"

/*
 * Note: This is not the same as the TextureManager. This is designed for when you need
 *  to locally cache textures with a specific key of access. It's either that or you generate an
 *  absurd string (like "myimage.png(300x600)" for the TextureManager). This method is much cleaner
 *  since you can use any type with a std::hash function.
 */

template <typename T>
class TextureStore
{
public:
	TextureStore() = default;
	~TextureStore() = default;
	
	void store(Texture* tex, const std::string &filename, T id)
	{
		if (!m_textures.contains(filename))
			m_textures.emplace(filename, std::unordered_map<T, std::unique_ptr<Texture>>{});
		
		if (!m_textures[filename].contains(id))
			m_textures[filename].emplace(id, tex);
	}
	
	TextureRef load(const std::string &filename, T id)
	try {
		return m_textures.at(filename).at(id).get();
	}
	catch (const std::out_of_range&) {
		return nullptr;
	}
	
public:
	std::unordered_map<std::string, std::unordered_map<T, std::unique_ptr<Texture>>> m_textures;
};

#endif
