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
#include "video/texture_manager.hpp"
#include "video/video_system.hpp"
#include "level_reader.hpp"
#include "tiles_reader.hpp"

#include "collision.hpp"

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
	
	Rectf player_pos(40, 60, {60, 100});
	
	BEGIN_GAME_LOOP
		handle_events();
		painter->clear();
		
		TextureRef tex = g_texture_manager.load("images/creatures/tux/big/stand-0.png");
		painter->draw_fill_rect(player_pos, {0, 255, 0, 155});
		painter->draw(tex, std::nullopt, player_pos);
		
		Rectf mouse_rect(g_input_manager.get_mouse_x(), g_input_manager.get_mouse_y(), {0, 0});
		
		auto collide = Collision::aabb(player_pos, mouse_rect);
		if (collide.is_colliding())
			std::cout << "COLLIDING!! "<< collide.left_constraint << " " << 
				collide.right_constraint << " " << collide.top_constraint << 
				" " << collide.bottom_constraint << std::endl;
		else
			std::cout << "ISNT COLLIDING!!" << std::endl;
		
		painter->flip();
	END_GAME_LOOP
	
	delete level;
	//delete tilemap;
}
