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

#ifndef SUPERTUX_SRC_INPUT_MANAGER_HPP
#define SUPERTUX_SRC_INPUT_MANAGER_HPP

#include <SDL3/SDL_events.h>
#include <string>

class InputManager
{
public:
	InputManager();
	~InputManager() = default;
	
	void handle_event(const SDL_Event &ev);
	
	unsigned get_mouse_x() const { return m_mouse_x; }
	unsigned get_mouse_y() const { return m_mouse_y; }
	int get_mouse_dx() const { return m_mouse_dx; }
	int get_mouse_dy() const { return m_mouse_dy; }
	bool is_mouse_down() const { return m_mouse_down; }
	bool get_mouse_scrolled() const { return m_mouse_scroll_x != 0 || m_mouse_scroll_y != 0; }
	int get_scroll_x() const { return m_mouse_scroll_x; }
	int get_scroll_y() const { return m_mouse_scroll_y; }
	unsigned get_mouse_button() const { return m_mouse_btn; }
	
	void reset();
	
	std::string to_string() const;
private:
	unsigned m_mouse_x, m_mouse_y;
	int m_mouse_dx, m_mouse_dy;
	bool m_mouse_down;
	unsigned m_mouse_btn;
	int m_mouse_scroll_x, m_mouse_scroll_y;
};

extern InputManager g_input_manager;

#endif
