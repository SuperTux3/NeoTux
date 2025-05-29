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

#include "ttf_font.hpp"
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include "sdl_exception.hpp"

using namespace std::string_literals;

TTFFont::TTFFont(std::string_view filename,
                 int font_size,
				 float line_spacing,
				 int shadow_size,
				 int border) :
	m_font{nullptr, TTF_CloseFont},
	m_line_spacing{line_spacing},
	m_shadow_size{shadow_size},
	m_border{border}
{
	TTF_Font* font = TTF_OpenFont(filename.data(), font_size);
	if (!m_font)
	{
		throw SDLException("Couldn't load TTFFont");
	}
}

float
TTFFont::get_text_width(std::string_view text) const
{
	
}
