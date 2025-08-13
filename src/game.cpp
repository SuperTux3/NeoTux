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

#include <optional>
#include <util/logger.hpp>
#include <format>
#include "config.h"
#include "game.hpp"
#include "camera.hpp"
#include "gui/box.hpp"
#include "gui/container.hpp"
#include "input_manager.hpp"
#include "level_reader.hpp"
#include "tiles_reader.hpp"
#include "gui/gui_reader.hpp"
#include "util/filesystem.hpp"
#include "video/sdl/painter.hpp"
#include "video/sdl/sdl_video_system.hpp"
#include "video/sdl/surface_blitter.hpp"
#ifdef NEOTUX_BGFX
#include "video/bgfx/bgfx_video_system.hpp"
#endif
#include "video/texture_manager.hpp"
#include "video/video_system.hpp"
#include "settings.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL.h>

std::unique_ptr<Game> g_game{nullptr};
double g_dtime = 0;

Game::Game() :
	m_quit(false)
{
	init_video_system();
	
	InputManager::define_game_default_mappings();
	g_input_manager.load_gamepads();
}

void
Game::update()
{
	
}

void
Game::shutdown()
{
	g_mixer.shutdown();
	g_video_system->shutdown();
	SDL_Quit();
}

void
Game::init_video_system()
{
	switch (g_settings->renderer)
	{
		case VideoSystem::VIDEO_NULL:
			break;
		case VideoSystem::VIDEO_BGFX_OPENGL:
		case VideoSystem::VIDEO_BGFX_OPENGLES:
		case VideoSystem::VIDEO_BGFX_VULKAN:
	    case VideoSystem::VIDEO_BGFX_METAL:
		case VideoSystem::VIDEO_SDL:
		default:
		    g_video_system = std::make_unique<SDLVideoSystem>();
	}
	Logger::info(std::format("Using {} backend",
	                         VideoSystem::video_system_to_str(g_video_system->get_video_system())));
}

void
Game::run()
{
	std::cout << " !!! NOTE !!! \n\n"
	             "   Please start Supertux with `--test everything` or your test of choice.\n"
				 "   The core game loop has not been worked on yet.\n"
				 "\n"
				 " Cheers, Swagbro." << std::endl;
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
				break;
			case SDL_EVENT_WINDOW_RESIZED:
			{
				g_video_system->on_resize(ev.window.data1, ev.window.data2);
				g_camera.width  = ev.window.data1;
				g_camera.height = ev.window.data2;
			}
		}
		g_input_manager.handle_event(ev);
	}
	
}
