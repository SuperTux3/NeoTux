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

#include "platforming_test.hpp"
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
PlatformingTest::run()
{
	Size winsize = g_video_system->get_window_size();
	g_camera.width = winsize.width;
	g_camera.height = winsize.height;
	
	g_mixer.play_music("music/antarctic/chipdisko.ogg");
	
	g_tiles_reader.open();	
	
	LevelReader reader;
	Level *level = reader.open("levels/via_nostalgica.stl");
	Sector &sector = level->get_sector(0);
	Tilemap *tilemap = sector.get_tilemap_by_zpos(0);
	
	Painter* painter = g_video_system->get_painter();
	painter->register_camera(&g_camera);
	
	Player player{};
	
	BEGIN_GAME_LOOP
		handle_events();
		painter->clear();
		
		Rectf mouse_rect(g_input_manager.get_mouse_x(), g_input_manager.get_mouse_y(), {0, 0});
		if (g_input_manager.is_mouse_down() == true)
		{
			if (g_input_manager.get_mouse_button() == 1)
			{
				player.disable_gravity();
				player.move((double)g_input_manager.get_mouse_dx()*g_dtime,
				            (double)g_input_manager.get_mouse_dy()*g_dtime);
			}
		}
		else
		{
			if (player.m_likes_falling == false)
			{
				player.m_y_vel = 0.0;
				player.enable_gravity();
			}
		}
		
		if (g_input_manager.is_key_down('a'))
			player.move(-0.5 * g_dtime, 0);
		else if (g_input_manager.is_key_down('d'))
			player.move(0.5 * g_dtime, 0);
		if (g_input_manager.is_key_down('w') && player.m_grounded == true)
		{
			player.m_y_vel = 0.75;
			player.m_grounded = false;
			g_mixer.play_sound("sounds/bigjump.wav");
		}
		
		g_camera.x = (player.get_rect().left + player.get_rect().get_width() / 2.f) - g_camera.width / 2.f;
		g_camera.y = (player.get_rect().top + player.get_rect().get_height() / 2.f) - g_camera.height / 2.f;
		
		tilemap->draw(g_camera);
		
		player.update(*tilemap);
		player.draw();
		
		tilemap->try_object_collision(player);
		

		painter->flip();
	END_GAME_LOOP
	
	delete level;
	//delete tilemap;
}
