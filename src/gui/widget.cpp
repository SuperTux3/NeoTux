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

#include <SDL3/SDL_pixels.h>
#include <cstdint>
#include <iostream>
#include <functional>
#include <stdexcept>
#include <unordered_map>
#include "widget.hpp"
#include "gui/box.hpp"
#include "gui/button.hpp"
#include "gui/container.hpp"
#include "gui/text.hpp"
#include "util/logger.hpp"

std::unordered_map<std::string_view, Widget::factory_functor> _registered_widgets{};

/*static*/ void
Widget::register_all_widgets()
{
	Widget::register_widget<BoxWidget>();
	Widget::register_widget<ButtonWidget>();
	Widget::register_widget<ContainerWidget>();
	Widget::register_widget<TextWidget>();
}

/*static*/ void
Widget::parse_color(SDL_Color &color, SexpElt elt)
try {
	color.r = elt.get_int();
	elt.next_inplace();
	color.g = elt.get_int();
	elt.next_inplace();
	color.b = elt.get_int();
	elt.next_inplace();
	color.a = elt.get_int_or(255);
}
catch (...) {
	if (!elt.is_value())
		return;
	std::string val = elt.get_value();
	if (val.length() < 7)
		return;
	if (val[0] != '#')
		return;
	
	std::string bits = val.substr(1, 2);
	color.r = std::stoi(bits, nullptr, 16);
	bits = val.substr(3, 2);
	color.g = std::stoi(bits, nullptr, 16);
	bits = val.substr(5, 2);
	color.b = std::stoi(bits, nullptr, 16);
	if (val.length() < 9)
	{
		color.a = 255;
		return;
	}
	bits = val.substr(7, 2);
	color.a = std::stoi(bits, nullptr, 16);
}


/*static*/ Widget::factory_functor
Widget::get_widget_cstor(const std::string &name)
{
	return _registered_widgets.at(name);
}

/*static*/ Widget*
Widget::create(SexpElt elt)
try {
	//elt.next_inplace();
	if (!elt.is_list())
		return nullptr;
	elt = elt.get_list();
	if (!elt.is_value())
		return nullptr;
	
	std::string widget_type = elt.get_value();
	return _registered_widgets.at(widget_type)(elt.next());
}
catch (const std::out_of_range&)
{
	return nullptr;
}
