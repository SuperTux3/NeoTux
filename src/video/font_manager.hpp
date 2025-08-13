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

#ifndef SUPERTUX_SRC_VIDEO_FONT_MANAGER_HPP
#define SUPERTUX_SRC_VIDEO_FONT_MANAGER_HPP

#include "video/font_cache.hpp"
#include "video/texture.hpp"
#include <unordered_map>
#include <algorithm>
#include <string>
#include <vector>

struct FontManagerItem
{
	FontManagerItem(std::string font_name) :
		font_name{font_name},
		fonts{}
	{}

	FontManagerItem(FontManagerItem&& other) noexcept :
		font_name(std::move(other.font_name)),
		fonts(std::move(other.fonts))
	{}

	std::string font_name;
	std::unordered_map<int, std::unique_ptr<FontCache>> fonts;
};

class FontManager
{
public:
	FontManager() = default;
	~FontManager() = default;
	
	static void load_builtin_fonts();
	
	int load_font(std::string name);
	TextureRef load(int font_id, int font_size, const std::string &message, SDL_Color color);
	bool try_gc_all();
private:
	void try_create(int font_id, int font_size);
	std::vector<FontManagerItem> m_fonts;
};

extern FontManager g_font_manager;
extern int SUPERTUX_MEDIUM;

#endif
