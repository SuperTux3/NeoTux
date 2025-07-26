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

#ifndef SUPERTUX_SRC_VIDEO_FONT_CACHE_HPP
#define SUPERTUX_SRC_VIDEO_FONT_CACHE_HPP

#include "texture.hpp"
#include <unordered_map>
#include "sdl/ttf_font.hpp"

class FontCache
{
public:
	FontCache(std::string_view fontpath, int font_size);
	~FontCache() = default;
	
	TextureRef load(const std::string &message, SDL_Color color);
private:
	TTFFont m_font;
	std::unordered_map<std::string /*message*/, TextureRef> m_strings;
};

#endif
