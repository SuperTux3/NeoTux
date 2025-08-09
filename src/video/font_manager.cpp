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

#include "font_manager.hpp"
#include "util/filesystem.hpp"
#include <utility>

FontManager g_font_manager{};
int SUPERTUX_MEDIUM = 0;

void
FontManager::load_builtin_fonts()
{
	SUPERTUX_MEDIUM = g_font_manager.load_font("fonts/SuperTux-Medium.ttf");
}

int
FontManager::load_font(std::string name)
{
	m_fonts.push_back({
		FS::path(name), std::unordered_map<int, std::unique_ptr<FontCache>>{}});
	return m_fonts.size()-1;
}

void
FontManager::try_create(int font_id, int font_size)
{
	auto &fonts = m_fonts[font_id];
	if (!fonts.second.contains(font_size))
		fonts.second.emplace(font_size, std::make_unique<FontCache>(fonts.first, font_size));
}

TextureRef
FontManager::load(int font_id, int font_size, const std::string &message, SDL_Color color)
{
	try_create(std::move(font_id), std::move(font_size));
	
	return m_fonts[font_id].second[font_size]->load(message, std::move(color));
}
