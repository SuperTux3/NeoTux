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
#include "object/moving_sprite.hpp"
#include "object/player.hpp"
#include "timer.hpp"
#include "video/video_system.hpp"
#include "level_reader.hpp"
#include "tiles_reader.hpp"

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
	
	MovingSprite sprite("images/creatures/tux/tux.sprite", "super the tux");
	sprite.set_action("small-stand-right");
	
	Player player{};
	Timer timer{1000, -1};
	
	BEGIN_GAME_LOOP
		handle_events();
		painter->clear();
		
		sprite.update(sector, *tilemap);
		
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
				player.set_y_vel(0.0);
				player.enable_gravity();
			}
		}
		
		player.update(sector, *tilemap);
		tilemap->try_object_collision(player);
		
		if (timer.tick())
			std::cout << "Timer ticked!" << std::endl;
			
		g_camera.x = (player.get_rect().left + player.get_rect().get_width() / 2.f) - g_camera.width / 2.f;
		g_camera.y = (player.get_rect().top + player.get_rect().get_height() / 2.f) - g_camera.height / 2.f;
		
		tilemap->draw(g_camera);
		player.draw();
		sprite.draw();
	END_GAME_LOOP
	
	delete level;
	//delete tilemap;
}
