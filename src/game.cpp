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
#include "config.h"
#include "game.hpp"
#include "camera.hpp"
#include "util/filesystem.hpp"
#include "video/sdl/painter.hpp"
#include "video/sdl/sdl_video_system.hpp"
#ifdef NEOTUX_BGFX
#include "video/bgfx/bgfx_video_system.hpp"
#endif
#include "video/texture_manager.hpp"
#include "video/video_system.hpp"
#include "video/font_cache.hpp"
#include "settings.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL.h>

Game g_game{};

static FontCache g_font_cache{FS::path("fonts/SuperTux-Medium.ttf"), 32};

Game::Game() :
	m_input_manager(),
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
		TextureRef tex = pair.second.get();
		SDL_FRect dest = { (float)((i*40)%winsize.width), (float)((i*40)/winsize.width)*40, 40, 40 };
		//SDL_RenderTexture(static_cast<SDLVideoSystem*>(g_video_system.get())->m_sdl_renderer.get(), texture, NULL, &dest);
		
		Painter* painter = g_video_system->get_painter();
		
		painter->draw(tex, std::nullopt, dest);
		
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
			g_video_system.reset(new SDLVideoSystem{});
		    break;
		case VideoSystem::VIDEO_BGFX_OPENGL:
		case VideoSystem::VIDEO_BGFX_OPENGLES:
		case VideoSystem::VIDEO_BGFX_VULKAN:
	    case VideoSystem::VIDEO_BGFX_METAL:
		default:
		    g_video_system = std::make_unique<SDLVideoSystem>();
	}
	Logger::info(std::format("Using {} backend",
	                         VideoSystem::video_system_to_str(g_settings->renderer)));

	Size winsize = g_video_system->get_window_size();
	Camera camera(winsize.width, winsize.height);
	
	g_font_cache.load("This message should be cleaned up.", {255, 255, 255, 255});
	
	int i = 0;
	while (!m_quit)
	{
		if (!g_video_system)
			continue;
		g_font_cache.try_gc();
	

		Painter* painter = g_video_system->get_painter();
		painter->register_camera(&camera);
		handle_events();
		
		camera.x = sin((float)i/10.f)*100.f - 50 - sin((float)i/300.f)*55;
		camera.y = cos((float)i/20.f)*100.f - 50 - tan((float)i/100.f)*55;

		SDL_RenderClear(static_cast<SDLPainter*>(painter)->m_sdl_renderer.get());
		
		g_texture_manager.load("images/creatures/mr_bomb/left-0.png");
		g_texture_manager.load("images/creatures/nolok/walk-0.png");
		g_texture_manager.load("images/creatures/owl/carry-0.png");
		g_texture_manager.load("images/creatures/penny/stand-0.png");
		//g_texture_manager.add("images/creatures/spiky/spikycry.png");
		TextureRef ref = g_texture_manager.load("images/engine/supertux.png");
		
		TextureRef text = g_font_cache.load("Hello Super Tux", {255, 255, 255, 255});
		painter->draw(text, std::nullopt, SDL_FRect{50,50,(float)text->get_size().width,(float)text->get_size().height}); 
		
		
		camera.x = sin((float)i/10.f)*80.f - 200 - sin((float)i/300.f)*35;
		camera.y = cos((float)i/20.f)*80.f - 200 - tan((float)i/80.f)*35;
		draw_textures();		
		
		g_video_system->flip();
		SDL_Delay(10);
		
		++i;
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
		m_input_manager.handle_event(ev);
	}
	
}
