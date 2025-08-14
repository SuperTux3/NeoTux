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

#include "raw_draw_test.hpp"
#include "input_manager.hpp"
#include "math/size.hpp"
#include "camera.hpp"
#include "object/player.hpp"
#include "video/font_manager.hpp"
#include "video/sdl/painter.hpp"
#include "video/sdl/texture.hpp"
#include "video/texture_manager.hpp"
#include "video/video_system.hpp"
#include "level_reader.hpp"
#include "tiles_reader.hpp"

#include "collision.hpp"
#include <SDL3/SDL_render.h>
#include <optional>

void
RawDrawTest::run()
{
	FontManager::load_builtin_fonts();
	Painter *painter = g_video_system->get_painter();
	SDLPainter *sdl_painter = static_cast<SDLPainter*>(painter);
	SDL_Renderer *renderer = sdl_painter->m_sdl_renderer.get();
	
	TextureRef ptex = g_texture_manager.load("images/creatures/tux/small/idle-0.png");
	SDL_Texture* tex = static_cast<SDLTexture*>(ptex)->get_sdl_texture();
	
	double total = 3.0;

	Camera cam{(unsigned)g_video_system->get_window_size().width, (unsigned)g_video_system->get_window_size().height};
	bool jumped = false;
	bool use_painter = false;
	BEGIN_GAME_LOOP
		
		handle_events();
		
		if (g_input_manager.mapping_pressed(LEFT_BINDING))
		{
			cam.x += 1;
		}
		else if (g_input_manager.mapping_pressed(RIGHT_BINDING))
		{
			cam.x -= 1;
		}
		
		if (g_input_manager.mapping_pressed(JUMP_BINDING) && !jumped)
		{
			jumped = true;
			use_painter = !use_painter;
		}
		else if (!g_input_manager.mapping_pressed(JUMP_BINDING))
			jumped = false;
		
		
		if (g_input_manager.mapping_pressed(LEFT_BINDING))
		{
			cam.x += 1;
		}
		if (g_input_manager.mapping_pressed(UP_BINDING))
			total += 7  * g_dtime;
		else if (g_input_manager.mapping_pressed(DOWN_BINDING))
			total -= 7  * g_dtime;
		if (g_input_manager.get_scroll_y() != 0)
			cam.zoom += g_input_manager.get_scroll_y() / 15.0;
		if (g_input_manager.mapping_pressed(ZOOMIN_BINDING))
			cam.zoom += 1 * g_dtime;
		else if (g_input_manager.mapping_pressed(ZOOMOUT_BINDING))
			cam.zoom -= 1 * g_dtime;

		if (use_painter)
			painter->clear();
		else
			SDL_RenderClear(renderer);
		TextureRef fpstex = g_font_manager.load(SUPERTUX_MEDIUM, 18,
			std::format("({}) FPS: {:.0f}", use_painter ? "Painter" : "SDL", get_fps()), {255, 255, 255, 155});
		SDL_Texture* fps = static_cast<SDLTexture*>(fpstex)->get_sdl_texture();
		
		for (int i = 0; i < (int)total; ++i)
		{
			for (int j = 0; j < (int)total; ++j)
			{
				SDL_FRect dest =
					{(float)(((i*12) * cam.zoom) + cam.x),
					 (float)(((j*12) + cam.y) * cam.zoom),
					 (float)(tex->w * cam.zoom),
					 (float)(tex->h * cam.zoom)};
				if (use_painter)
					painter->draw(ptex, std::nullopt, dest);
				else
					SDL_RenderTexture(renderer, tex, NULL, &dest);
			}
		}
		SDL_FRect dest2{0,0,(float)fps->w, (float)fps->h};
		if (use_painter)
			painter->draw(fpstex, std::nullopt, dest2);
		else
			SDL_RenderTexture(renderer, fps, NULL, &dest2);
		
		if (use_painter)
			painter->flip();
		else
			SDL_RenderPresent(renderer);
		
		// Don't use END_GAME_LOOP since we already flipped
		g_input_manager.reset(); \
		if (g_settings->forced_delay != 0)  
			SDL_Delay(g_settings->forced_delay);  
		g_dtime = (((double)SDL_GetTicksNS() * 0.000000001) - __last_time) * g_settings->speed;  
		if (g_dtime > .19) {  
			Logger::warn(std::format("Frame took too long! ({:.0f} ms). Skipping", g_dtime * 1000));  
			g_dtime = 0;  
		}
	}
}
