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
#include <iostream>

InputManager g_input_manager;

size_t JUMP_BINDING;
size_t CROUCH_BINDING;
size_t LEFT_BINDING;
size_t RIGHT_BINDING;
size_t UP_BINDING;
size_t DOWN_BINDING;

void
InputManager::define_game_default_mappings()
{
	LEFT_BINDING = g_input_manager.define_mapping("Left", Binding{
		Binding::Key('a')
	});
	RIGHT_BINDING = g_input_manager.define_mapping("Right", Binding{
		Binding::Key('d')
	});
	UP_BINDING = g_input_manager.define_mapping("Up", Binding{
		Binding::Key('w')
	});
	DOWN_BINDING = g_input_manager.define_mapping("Down", Binding{
		Binding::Key('s')
	});
	JUMP_BINDING = UP_BINDING;
	CROUCH_BINDING = DOWN_BINDING;
}

InputManager::InputManager() :
	m_mouse_x(),
	m_mouse_y(),
	m_mouse_dx(),
	m_mouse_dy(),
	m_mouse_btn(),
	m_mouse_down(),
	m_mouse_scroll_x(),
	m_mouse_scroll_y()
{
	if (!SDL_Init(SDL_INIT_GAMEPAD))
		throw SDLException("SDL_Init(SDL_INIT_GAMEPAD)");
}

size_t
InputManager::define_mapping(std::string name, Binding binding)
{
	m_bindings.emplace_back(std::move(name), std::move(binding));
	return m_bindings.size()-1;
}

void
InputManager::handle_event(const SDL_Event &ev)
{
	switch (ev.type)
	{
		case SDL_EVENT_MOUSE_MOTION:
			m_mouse_x = ev.motion.x;
			m_mouse_y = ev.motion.y;
			m_mouse_dx = ev.motion.xrel;
			m_mouse_dy = ev.motion.yrel;
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
		case SDL_EVENT_KEY_DOWN: {
			char key = SDL_SCANCODE_TO_KEYCODE(ev.key.key);
			m_keys.push_back(key);
			for (auto &pair : m_bindings)
			{
				auto &binding = pair.second;
				if (binding.is_keyboard())
				{
					if ((key == binding.get_data()) &&
					    (binding.ctrl() == ((ev.key.mod & SDL_KMOD_CTRL) != 0)) &&
						(binding.alt() == ((ev.key.mod & SDL_KMOD_ALT) != 0)))
					{
						binding.pressed = true;
						break;
					}
				}
				else {
				}
			}
		}
			break;
		case SDL_EVENT_KEY_UP: {
			char key = SDL_SCANCODE_TO_KEYCODE(ev.key.key);
			auto it = m_keys.erase(std::remove(m_keys.begin(), m_keys.end(), key), m_keys.end());
			for (auto &pair : m_bindings)
			{
				auto &binding = pair.second;
				if (binding.is_keyboard())
				{
					if ((key == binding.get_data()) &&
					    (binding.ctrl() == ((ev.key.mod & SDL_KMOD_CTRL) != 0)) &&
						(binding.alt() == ((ev.key.mod & SDL_KMOD_ALT) != 0)))
					{
						binding.pressed = false;
						break;
					}
				}
				else {
				}
			}
		}
			break;
		default:
			break;
	}
}

bool
InputManager::is_key_down(char key) const
{
	return std::find(m_keys.begin(), m_keys.end(), key) != m_keys.end();
}

void
InputManager::reset()
{
	m_mouse_scroll_x = 0;
	m_mouse_scroll_y = 0;
	m_mouse_dx = 0;
	m_mouse_dy = 0;
}

std::string
InputManager::to_string() const
{
	return std::format(
		"Mouse down: {}\n"
		" Mouse btn: {}\n"
		"         x: {}\n"
		"         y: {}\n"
		"   delta x: {}\n"
		"   delta y: {}\n"
		"  scroll x: {}\n"
		"  scroll y: {}",
		is_mouse_down(),
		get_mouse_button(),
		get_mouse_x(),
		get_mouse_y(),
		get_mouse_dx(),
		get_mouse_dy(),
		get_scroll_x(),
		get_scroll_y());
}
