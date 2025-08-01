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

#include <iostream>
#include <functional>
#include <unordered_map>
#include "widget.hpp"
#include "util/logger.hpp"

std::unordered_map<std::string_view, Widget::factory_functor> _registered_widgets{};

Widget::factory_functor
Widget::get_widget_cstor(const std::string &name)
{
	return _registered_widgets.at(name);
}

Widget*
Widget::create(SexpElt elt)
{
	//elt.next_inplace();
	if (!elt.is_list())
		return nullptr;
	elt = elt.get_list();
	if (!elt.is_value())
		return nullptr;
	
	std::string widget_type = elt.get_value();
	Logger::debug("Got: " + elt.get_value());
	return _registered_widgets.at(widget_type)(elt.next());
}
