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

#ifndef SUPERTUX_SRC_GUI_TEXT_HPP
#define SUPERTUX_SRC_GUI_TEXT_HPP

#include <SDL3/SDL_pixels.h>
#include <vector>
#include <memory>
#include "gui/box.hpp"

class TextWidget : public BoxWidget
{
public:
	TextWidget(const Rectf &props, std::string message, unsigned font_size, SDL_Color color);
	~TextWidget() = default;

	static std::string_view class_id() { return "text"; }
	std::string_view obj_name() override { return "Text"; }
	
	static Widget* construct(SexpElt elt);
	
	void add(BoxWidget *box);
	
	virtual void draw() override;
	virtual void update() override;
	
private:
	std::string m_message;
	unsigned m_font_size;
	SDL_Color m_color;
};

#endif
