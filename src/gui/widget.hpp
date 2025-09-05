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
	
#ifndef SUPERTUX_SRC_GUI_WIDGET_HPP
#define SUPERTUX_SRC_GUI_WIDGET_HPP

#include <functional>
#include "util/sexp.hpp"

struct ViewContext;
struct Widget;
struct SDL_Color;
extern std::unordered_map<std::string_view, std::function<Widget*(SexpElt)>> _registered_widgets;

class Widget
{
public:
	using factory_functor = std::function<Widget*(SexpElt)>;
	
	Widget() = default;
	virtual ~Widget() = default;
	
	static std::string_view class_id() { return "widget"; }
	virtual std::string_view obj_name() { return "Widget"; }

	static void register_all_widgets();
	static void parse_color(SDL_Color &color, SexpElt elt);
	
	// factory stuff
	template <typename T>
	static void register_widget()
	{
		std::string_view id = T::class_id();
		_registered_widgets.emplace(id, &T::construct);
	}
	
	static factory_functor get_widget_cstor(const std::string &name);
	static Widget* create(SexpElt elt);

	bool handle_events() { return false; };
	virtual void draw(ViewContext&) = 0;
	virtual void update() = 0;

	virtual bool on_mouse_button_up() { return false; }
	virtual bool on_mouse_button_down() { return false; }
	virtual bool on_mouse_motion() { return false; }
	virtual bool on_mouse_wheel() { return false; }
	virtual bool on_key_up() { return false; }
	virtual bool on_key_down() { return false; }
};


#endif
