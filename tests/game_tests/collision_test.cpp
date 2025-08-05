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

#include "collision_test.hpp"
#include "input_manager.hpp"
#include "math/size.hpp"
#include "camera.hpp"
#include "object/player.hpp"
#include "util/filesystem.hpp"
#include "video/font_cache.hpp"
#include "video/texture_manager.hpp"
#include "video/video_system.hpp"
#include "level_reader.hpp"
#include "tiles_reader.hpp"

#include "collision.hpp"

static FontCache g_font_cache{FS::path("fonts/SuperTux-Medium.ttf"), 32};

void
CollisionTest::run()
{
	Size winsize = g_video_system->get_window_size();
	g_camera.width = winsize.width;
	g_camera.height = winsize.height;
	
	g_mixer.play_music("music/antarctic/chipdisko.ogg");
	
	TilesReader tiles_reader;
	tiles_reader.open();	
	
	LevelReader reader;
	Level *level = reader.open("levels/via_nostalgica.stl");
	Sector &sector = level->get_sector(0);
	Tilemap *tilemap = sector.get_tilemap_by_zpos(0);
	
	Painter* painter = g_video_system->get_painter();
	painter->register_camera(&g_camera);
	
	Player player;
	
	BEGIN_GAME_LOOP
		handle_events();
		painter->clear();
		
		Rectf mouse_rect(g_input_manager.get_mouse_x(), g_input_manager.get_mouse_y(), {0, 0});
		
		if (g_input_manager.is_mouse_down() == true)
		{
			if (g_input_manager.get_mouse_button() == 1)
				player.move((double)g_input_manager.get_mouse_dx()*1200.0*g_dtime,
				            (double)g_input_manager.get_mouse_dy()*1200.0*g_dtime);
			else if (g_input_manager.get_mouse_button() == 3) {
				player.move_to((int)g_input_manager.get_mouse_x() + g_camera.x,
				               (int)g_input_manager.get_mouse_y() + g_camera.y);
			}
			else if (g_input_manager.get_mouse_button() == 2) {
				g_camera.x -= g_input_manager.get_mouse_dx()*1500.0*g_dtime;
				g_camera.y -= g_input_manager.get_mouse_dy()*1500.0*g_dtime;
			}
		}
		
		TextureRef help_1 = g_font_cache.load("Left click to move relative", {255, 255, 255, 155});
		painter->draw(help_1, std::nullopt,
			Rectf{0, 0, {(float)help_1->get_size().width, (float)help_1->get_size().height}});
		TextureRef help_2 = g_font_cache.load("Middle click to move camera", {255, 255, 255, 155});
		painter->draw(help_2, std::nullopt,
			Rectf{0, (float)help_1->get_size().height*2, {(float)help_2->get_size().width, (float)help_2->get_size().height}});
		TextureRef help_3 = g_font_cache.load("Right click to move Tux to cursor", {255, 255, 255, 155});
		painter->draw(help_3, std::nullopt,
			Rectf{0, (float)help_1->get_size().height*4, {(float)help_3->get_size().width, (float)help_3->get_size().height}});
		
		tilemap->draw(g_camera, tiles_reader);
		for (int x = 0; x < tilemap->get_size().width; ++x)
		{
			for (int y = 0; y < tilemap->get_size().height; ++y)
			{
				int rx = x * 32;
				int ry = y * 32;
				Rectf rrect{(float)rx, (float)ry, {32.f, 32.f}};
				const Tile &tile = tilemap->get_tile(x, y);
				if (tile.get_id() != 0)
				{
					
					auto collide = Collision::aabb(player.get_colbox(), rrect);
					if (collide.is_colliding())
					{
						player.m_grounded = true;
						
						if (collide.top)
							player.move(0, collide.top_constraint);
						if (collide.bottom)
							player.move(0, -collide.bottom_constraint);
						if (collide.left)
							player.move(collide.left_constraint, 0);
						if (collide.right)
							player.move(-collide.right_constraint, 0);
					}
				}
			}
		}
		
		player.update();
		player.draw();
		

		painter->flip();
	END_GAME_LOOP
	
	delete level;
	//delete tilemap;
}
