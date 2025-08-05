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
#include "everything_test.hpp"
#include "video/font_cache.hpp"
#include "config.h"
#include "camera.hpp"
#include "gui/box.hpp"
#include "gui/container.hpp"
#include "input_manager.hpp"
#include "level_reader.hpp"
#include "tiles_reader.hpp"
#include "util/filesystem.hpp"
#include "gui/gui_reader.hpp"
#include "video/sdl/surface_blitter.hpp"
#ifdef NEOTUX_BGFX
#include "video/bgfx/bgfx_video_system.hpp"
#endif
#include "video/texture_manager.hpp"
#include "video/video_system.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL.h>

static FontCache g_font_cache{FS::path("fonts/SuperTux-Medium.ttf"), 32};

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
EverythingTest::run()
{
	Size winsize = g_video_system->get_window_size();
	g_camera.width = winsize.width;
	g_camera.height = winsize.height;
	
	g_font_cache.load("This message should be cleaned up.", {255, 255, 255, 255});
	g_mixer.play_music("music/antarctic/chipdisko.ogg");

	// Tiles
	TilesReader tiles_reader;
	tiles_reader.open();	
	
	// Level info
	LevelReader reader;
	Level *level = reader.open("levels/via_nostalgica.stl");
	Sector &sector = level->get_sector(0);
	Tilemap *tilemap = sector.get_tilemap_by_zpos(0);
	
	// Setup guis
	Widget::register_widget<BoxWidget>();
	Widget::register_widget<ContainerWidget>();
	//Widget::register_widget("box", BoxWidget::construct);
	GuiReader gui_reader;
	Widget *box = gui_reader.open("guis/main.stui");
	//BoxWidget box(SDL_FRect{20, 20, 120, 40});
	
	SurfaceBlitter thing({128, 128});
	//for (int x = 5; x < 85; ++x)
	//	for (int y = 5; y < 85; ++y)
	//		thing.write_pixel(x + sin(y/2)*4, y, {x*2, 130+y, x+y*2, 255});
	thing.fill({255, 0, 0, 255});
	thing.draw_circle(50, 80, 20, {0, 255, 0, 255});
	std::unique_ptr<Texture> cool_thing(thing.to_texture());
	thing.destroy();
	
	double i = 0, j = 1;
	BEGIN_GAME_LOOP
		g_font_cache.try_gc();
		TextureRef ref = g_texture_manager.load("images/engine/supertux.png");
		
		Painter* painter = g_video_system->get_painter();
		painter->clear();
		painter->register_camera(&g_camera);
		handle_events();
		
		painter->draw(cool_thing.get(), std::nullopt, SDL_FRect{80, 80, 300, 300});
		
		painter->begin_clip(SDL_Rect{90, 30, 600, 800});
		
		g_camera.x = 5;
		g_camera.y = 5;
		
		if (box)
		{
			box->update();
			box->draw();
			
		}
		
		// Draw tiles
		for (int x = 0; x < tilemap->get_size().width; ++x)
		{
			for (int y = 0; y < tilemap->get_size().height; ++y)
			{
				const Tile &tile = tilemap->get_tile(x, y);
				if (tile.get_id() != 0)
				{
					try{
					TileMeta &tilemeta = tiles_reader.m_tiles.at(tile.get_id());
					if (tilemeta.info->image.empty())
						continue;
					TextureRef tex = g_texture_manager.load("images/"+tilemeta.info->image);
					painter->draw(tex,
						tilemeta.get_src_rect(tex),
						SDL_FRect(x * 20 + (sin((float)i/260.f)*3600.f - 3500),
							y * 20,
							20,
							20));
					}catch(...) {
						continue;
					}
				}
			}
		}
		
		painter->end_clip();
		
		g_camera.x = sin((float)i/10.f)*100.f - 50 - sin((float)i/300.f)*55;
		g_camera.y = cos((float)i/20.f)*100.f - 50 - tan((float)i/100.f)*55;
		
		g_texture_manager.load("images/creatures/mr_bomb/left-0.png");
		g_texture_manager.load("images/creatures/nolok/walk-0.png");
		g_texture_manager.load("images/creatures/owl/carry-0.png");
		g_texture_manager.load("images/creatures/penny/stand-0.png");
		//g_texture_manager.add("images/creatures/spiky/spikycry.png");
		
		TextureRef text = g_font_cache.load("Hello Super Tux", {255, 255, 255, 255});
		painter->draw(text, std::nullopt, SDL_FRect{50,50,(float)text->get_size().width,(float)text->get_size().height}); 
		
		
		if (((int)i % 1200) == 0)
		{
			g_mixer.play_sound("sounds/bigjump.wav");
		}
		
		g_camera.x = sin((float)i/10.f)*80.f - 200 - sin((float)i/300.f)*35;
		g_camera.y = cos((float)i/20.f)*80.f - 200 - tan((float)i/80.f)*35;
		draw_textures();
		
		g_camera.x = 0;
		g_camera.y = 0;
		
//#ifndef NDEBUG
		TextureRef draws = g_font_cache.load(std::format("Draws: {}", painter->get_draw_count()), {255, 255, 255, 255});
		painter->draw(draws, std::nullopt, SDL_FRect{0, 0, (float)draws->get_size().width,(float)draws->get_size().height});
		TextureRef total_draws = g_font_cache.load(std::format("Total Draws: {}", painter->get_total_draw_count()), {255, 255, 255, 255});
		painter->draw(total_draws, std::nullopt, SDL_FRect{0, (float)draws->get_size().height, (float)total_draws->get_size().width,(float)total_draws->get_size().height});
//#endif

		std::cout << "Deltatime: " << g_dtime << std::endl;

		//Logger::debug(g_input_manager.to_string());
		i += DTIME * 60;
	END_GAME_LOOP
	
	delete level;
	delete box;
}
