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

#include <util/logger.hpp>
#include <format>
#include "game.hpp"
#include "video/sdl/sdl_video_system.hpp"
#include "video/bgfx/bgfx_video_system.hpp"
#include "video/video_system.hpp"
#include "settings.hpp"
#include <SDL3/SDL_events.h>

Game g_game{};

Game::Game() :
	m_quit(false)
{
}

void
Game::update()
{
	
}

void
Game::run()
{
	switch (g_settings->renderer)
	{
		case VideoSystem::VIDEO_NULL:
			break;
		case VideoSystem::VIDEO_SDL:
			g_video_system = std::make_unique<SDLVideoSystem>();
			break;
		case VideoSystem::VIDEO_BGFX_OPENGL:
		case VideoSystem::VIDEO_BGFX_OPENGLES:
		case VideoSystem::VIDEO_BGFX_VULKAN:
		case VideoSystem::VIDEO_BGFX_METAL:
		default:
			g_video_system = std::make_unique<BGFXVideoSystem>(g_settings->renderer);
	}
	Logger::info(std::format("Using {} backend", VideoSystem::get_video_string(g_settings->renderer)));
	

	while (!m_quit)
	{
		handle_events();
	}
}

void
Game::handle_events()
{
	if (!g_video_system)
		return;
	SDL_Event ev;
	while (SDL_PollEvent(&ev))
	{
		switch (ev.type)
		{
			case SDL_EVENT_QUIT:
				m_quit = true;
				g_video_system->shutdown();
				break;
			case SDL_EVENT_WINDOW_RESIZED:
			{
				g_video_system->on_resize(ev.window.data1, ev.window.data2);
			}
		}
	}
}
