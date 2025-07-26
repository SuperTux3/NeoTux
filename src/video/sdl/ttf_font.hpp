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

#ifndef HEADER_SUPERTUX_VIDEO_TTF_FONT_HPP
#define HEADER_SUPERTUX_VIDEO_TTF_FONT_HPP

#include <stdexcept>
#include <string_view>
#include <memory>
#include <SDL3_ttf/SDL_ttf.h>

class TTFFont
{
public:
	TTFFont(std::string_view filename, int font_size, float line_spacing = 1.0f, int shadow_size = 0, int border = 0);
	~TTFFont() = default;
	
	SDL_Surface* render_text_solid(const std::string &msg, SDL_Color color);
	
	float get_line_spacing() const { return m_line_spacing; }
	float get_height() const;
	
	float get_text_width(std::string_view text) const;
	float get_text_height(std::string_view text) const;
	
	int get_shadow_size() const { return m_shadow_size; }
	int get_border() const { return m_border; }	
	
	TTF_Font* get_ttf_font() const { return m_font.get(); }

private:
	std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> m_font;
	float m_line_spacing;
	int m_shadow_size;
	int m_border;
};

#endif
