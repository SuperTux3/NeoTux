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

#include "input_manager.hpp"
#include "sdl_exception.hpp"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_events.h>
#include <format>

InputManager g_input_manager;

InputManager::InputManager()
{
	if (!SDL_Init(SDL_INIT_GAMEPAD))
		throw SDLException("SDL_Init(SDL_INIT_GAMEPAD)");
}

void
InputManager::handle_event(const SDL_Event &ev)
{
	switch (ev.type)
	{
		case SDL_EVENT_MOUSE_MOTION:
			m_mouse_x = ev.motion.x;
			m_mouse_y = ev.motion.y;
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			m_mouse_btn = ev.button.button;
			m_mouse_down = true;
			break;
		case SDL_EVENT_MOUSE_BUTTON_UP:
			m_mouse_btn = ev.button.button;
			m_mouse_down = false;
			break;
		case SDL_EVENT_MOUSE_WHEEL:
			m_mouse_scroll_x += ev.wheel.integer_y;
			m_mouse_scroll_y += ev.wheel.integer_y;
			break;
		default:
			break;
	}
}

void
InputManager::reset()
{
	m_mouse_scroll_x = 0;
	m_mouse_scroll_y = 0;
}

std::string
InputManager::to_string() const
{
	return std::format(
		"Mouse down: {}\n"
		" Mouse btn: {}\n"
		"         x: {}\n"
		"         y: {}\n"
		"  scroll x: {}\n"
		"  scroll y: {}",
		is_mouse_down(),
		get_mouse_button(),
		get_mouse_x(),
		get_mouse_y(),
		get_scroll_x(),
		get_scroll_y());
}
