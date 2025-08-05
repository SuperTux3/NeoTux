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
	
	Player player;
	
	BEGIN_GAME_LOOP
		handle_events();
		painter->clear();
		
		// Draw tiles
		for (int x = 0; x < 100; ++x)
		{
			for (int y = 0; y < tilemap->get_size().height; ++y)
			{
				int rx = x * 32;
				int ry = y * 32 - 200;
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
						Rectf{(float)rx, (float)ry, {32.f, 32.f}});
					}catch(...) {
						continue;
					}
				}
			}
		}
		
		Rectf mouse_rect(g_input_manager.get_mouse_x(), g_input_manager.get_mouse_y(), {0, 0});
		
		player.update();
		player.draw();
		
		auto collide = Collision::aabb(player.get_rect(), mouse_rect);
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
