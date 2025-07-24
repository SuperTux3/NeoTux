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
#include "video/texture_manager.hpp"
#include "video/video_system.hpp"
#include "settings.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL.h>

Game g_game{};

Game::Game() :
	m_quit(false)
{
}

void
Game::update()
{
	
}

#include "video/sdl/sdl_video_system.hpp"
#include "video/sdl/texture.hpp"

static void
draw_textures()
{
	int i = 0;
	Size winsize = g_video_system->get_window_size();
	for (auto const &pair: g_texture_manager.m_textures)
	{
		TextureRef tex = pair.second;
		SDL_FRect dest = { (float)((i*40)%winsize.width), (float)((i*40)/winsize.width)*40, 40, 40 };
		SDL_Texture *texture = static_cast<SDLTexture*>(tex)->get_sdl_texture();
		SDL_RenderTexture(static_cast<SDLVideoSystem*>(g_video_system.get())->m_sdl_renderer.get(), texture, NULL, &dest);
		
		
		++i;
	}
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
	Logger::info(std::format("Using {} backend", VideoSystem::video_system_to_str(g_settings->renderer)));
	

	while (!m_quit)
	{
		if (!g_video_system)
			continue;
		handle_events();

		SDL_RenderClear(static_cast<SDLVideoSystem*>(g_video_system.get())->m_sdl_renderer.get());
		
		g_texture_manager.add("images/creatures/mr_bomb/left-0.png");
		g_texture_manager.add("images/creatures/nolok/walk-0.png");
		g_texture_manager.add("images/creatures/owl/carry-0.png");
		g_texture_manager.add("images/creatures/penny/stand-0.png");
		//g_texture_manager.add("images/creatures/spiky/spikycry.png");
		TextureRef ref = g_texture_manager.add("images/engine/supertux.png");
		
		draw_textures();		
		
		g_video_system->flip();
		SDL_Delay(10);
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
