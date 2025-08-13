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
#include "util/filesystem.hpp"
#include "util/logger.hpp"
#include <SDL3/SDL_gamepad.h>
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
		Binding::Key('a'), Binding::Gamepad(SDL_GAMEPAD_BUTTON_DPAD_LEFT)
	});
	RIGHT_BINDING = g_input_manager.define_mapping("Right", Binding{
		Binding::Key('d'), Binding::Gamepad(SDL_GAMEPAD_BUTTON_DPAD_RIGHT)
	});
	UP_BINDING = g_input_manager.define_mapping("Up", Binding{
		Binding::Key('w'), Binding::Gamepad(SDL_GAMEPAD_BUTTON_DPAD_UP)
	});
	DOWN_BINDING = g_input_manager.define_mapping("Down", Binding{
		Binding::Key('s'), Binding::Gamepad(SDL_GAMEPAD_BUTTON_DPAD_DOWN)
	});
	JUMP_BINDING = g_input_manager.define_mapping("Jump", Binding{
		Binding::Key('w'), Binding::Gamepad(SDL_GAMEPAD_BUTTON_SOUTH)
	});
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
	
	SDL_AddGamepadMappingsFromFile(FS::path("gamecontrollerdb.txt").c_str());
}

void
InputManager::load_gamepads()
{
	std::unique_ptr<SDL_JoystickID, decltype(&SDL_free)> gamepads_store{nullptr, SDL_free};
	int count;
	gamepads_store.reset(SDL_GetGamepads(&count));
	SDL_JoystickID *gamepads = gamepads_store.get();
	if (!count || gamepads == nullptr)
		return;
	
	while (--count >= 0)
	{
		SDL_Gamepad *tmp;
		for (auto &pad : m_gamepads)
		{
			if (SDL_GetGamepadID(pad.get()) == gamepads[count])
				goto gamepad_exists;
		}
		tmp = SDL_OpenGamepad(gamepads[count]);
		m_gamepads.emplace_back(tmp, SDL_CloseGamepad);
gamepad_exists:
		continue;
	}
}

bool
InputManager::register_gamepad_by_id(int id)
{
	for (auto &pad : m_gamepads)
	{
		if (SDL_GetGamepadID(pad.get()) == id)
		{
			Logger::warn("Registered gamepad already exists.");
			return false;
		}
	}

	Logger::info("Gamepad detected!");	
	m_gamepads.emplace_back(SDL_OpenGamepad(id), SDL_CloseGamepad);
	return true;
}

void
InputManager::unregister_gamepad(int id)
{
	for (auto it = m_gamepads.begin(); it != m_gamepads.end(); ++it)
	{
		if (SDL_GetGamepadID(it->get()) == id)
		{
			Logger::info("Unregistering gamepad...");
			m_gamepads.erase(it);
			return;
		}
	}
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
		case SDL_EVENT_GAMEPAD_ADDED:
			if (g_input_manager.register_gamepad_by_id(ev.gdevice.which))
			{
				SDL_RumbleGamepad(SDL_GetGamepadFromID(ev.gdevice.which), 0xFFFF, 0xFFFF, 300);
			}
			break;
		case SDL_EVENT_GAMEPAD_REMOVED:
			g_input_manager.unregister_gamepad(ev.gdevice.which);
			break;
		case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
			int btn = ev.gbutton.button;
			for (auto &pair : m_bindings)
			{
				auto &binding = pair.second;
				if (binding.is_gamepad())
				{
					if (btn == binding.get_gamepad_button())
					{
						binding.pressed = true;
					}
				}
			}
		}
			break;
		case SDL_EVENT_GAMEPAD_BUTTON_UP: {
			int btn = ev.gbutton.button;
			for (auto &pair : m_bindings)
			{
				auto &binding = pair.second;
				if (binding.is_gamepad())
				{
					if (btn == binding.get_gamepad_button())
					{
						binding.pressed = false;
					}
				}
			}
		}
			break;
		case SDL_EVENT_KEY_DOWN: {
			char key = SDL_SCANCODE_TO_KEYCODE(ev.key.key);
			m_keys.push_back(key);
			for (auto &pair : m_bindings)
			{
				auto &binding = pair.second;
				if (binding.is_keyboard())
				{
					if ((key == binding.get_key()) &&
					    (binding.ctrl() == ((ev.key.mod & SDL_KMOD_CTRL) != 0)) &&
						(binding.alt() == ((ev.key.mod & SDL_KMOD_ALT) != 0)))
					{
						binding.pressed = true;
					}
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
					if ((key == binding.get_key()) &&
					    (binding.ctrl() == ((ev.key.mod & SDL_KMOD_CTRL) != 0)) &&
						(binding.alt() == ((ev.key.mod & SDL_KMOD_ALT) != 0)))
					{
						binding.pressed = false;
					}
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
