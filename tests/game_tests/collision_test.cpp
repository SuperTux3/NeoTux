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
#include "video/font_manager.hpp"
#include "video/texture_manager.hpp"
#include "video/video_system.hpp"
#include "level_reader.hpp"
#include "tiles_reader.hpp"

#include "collision.hpp"

void
CollisionTest::run()
{
	FontManager::load_builtin_fonts();
	Size winsize = g_video_system->get_window_size();
	g_rtcontext.width = winsize.width;
	g_rtcontext.height = winsize.height;
	
	g_mixer.play_music("music/antarctic/chipdisko.ogg");
	
	g_tiles_reader.open();	
	
	LevelReader reader;
	Level *level = reader.open("levels/collision_test.stl");
	Sector &sector = level->get_sector(0);
	Tilemap *tilemap = sector.get_tilemap_by_zpos(0);
	if (!tilemap)
		return;
	
	Painter* painter = g_video_system->get_painter();
	painter->register_camera(&g_rtcontext);
	
	Player player;
	
	Vec2 mline_beg{0, 0}, mline_end;
	
	
	double tick;
	Vec2 tline_beg{430, 434};
	Vec2 tline_end{598, 380};
	
	BEGIN_GAME_LOOP
		handle_events();
		painter->clear();
		tick += g_dtime;
		
		Rectf mouse_rect(g_input_manager.get_mouse_x(), g_input_manager.get_mouse_y(), {0, 0});
		
		if (g_input_manager.is_mouse_down() == true)
		{
			if (g_input_manager.get_mouse_button() == 1)
			{
				player.move((double)g_input_manager.get_mouse_dx()*1.0,
				            (double)g_input_manager.get_mouse_dy()*1.0);
				player.disable_gravity();
			}
			else if (g_input_manager.get_mouse_button() == 3) {
				player.move_to((int)g_input_manager.get_mouse_x() + g_rtcontext.x,
				               (int)g_input_manager.get_mouse_y() + g_rtcontext.y);
				player.disable_gravity();
			}
			else if (g_input_manager.get_mouse_button() == 2) {
				g_rtcontext.x -= g_input_manager.get_mouse_dx()*3.0;
				g_rtcontext.y -= g_input_manager.get_mouse_dy()*3.0;
			}
		}
		else if (player.m_likes_falling == false) {
			player.set_y_vel(0.0);
			player.enable_gravity();
		}
		
		mline_beg.x = player.get_rect().left;
		mline_beg.y = player.get_rect().top;
		mline_end.x = player.get_rect().left - g_rtcontext.x - (double)g_input_manager.get_mouse_x();
		mline_end.y = player.get_rect().top - g_rtcontext.y - (double)g_input_manager.get_mouse_y();
		
		
		TextureRef help_1 = g_font_manager.load(SUPERTUX_MEDIUM, 32,
			"Left click to move relative", {255, 255, 255, 155});
		painter->draw(help_1, std::nullopt,
			Rectf{0, 0, {(float)help_1->get_size().width, (float)help_1->get_size().height}});
		TextureRef help_2 = g_font_manager.load(SUPERTUX_MEDIUM, 32,
			"Middle click to move camera", {255, 255, 255, 155});
		painter->draw(help_2, std::nullopt,
			Rectf{0, (float)help_1->get_size().height*2, {(float)help_2->get_size().width, (float)help_2->get_size().height}});
		TextureRef help_3 = g_font_manager.load(SUPERTUX_MEDIUM, 32,
			"Right click to move Tux to cursor", {255, 255, 255, 155});
		painter->draw(help_3, std::nullopt,
			Rectf{0, (float)help_1->get_size().height*4, {(float)help_3->get_size().width, (float)help_3->get_size().height}});
			
		// Draw lines
		auto linecol = Collision::line_line(mline_beg, mline_end, tline_beg, tline_end);
		SDL_Color mline_col{255, 255, 255, 255};
		if (linecol)
		{
			mline_col.r = 0;
			mline_col.b = 0;
		}
		//mline_beg.rotate90();
		
		mline_end = mline_beg - (mline_end.get_normal() * 200);
		//mline_end.rotate90();
		
		//mline_end.rotate(90 * (3.14159/180));

		painter->draw_line(mline_beg, mline_end, mline_col);
		painter->draw_line(tline_beg, tline_end, {0, 255, 0, 255});
		
		//player.handle_input();
		//else if (g_input_manager.is_key_down('s'))
		//	player.down();
		
		player.update(sector, *tilemap);
		tilemap->draw(g_rtcontext);
		tilemap->try_object_collision(player);
		player.draw();
		
		
		Collision::col_side_t<float> cols[Collision::COL_SIZE_MAX];
		auto rectcol = Collision::line_rect(cols, tline_beg, tline_end, player.get_colbox());
		if (rectcol)
		{
			auto colbox = player.get_colbox();
			SDL_Color lcolor = {0, 255, 0, 255};
			painter->draw_fill_rect(player.get_colbox(), {133, 20, 180, 150});
			for (size_t i = 0; i < rectcol; ++i)
			{
				switch (cols[i].first)
				{
				case RECT_LEFT:
					painter->draw_line({ colbox.left, colbox.top }, { colbox.left, colbox.bottom }, lcolor);
					break;
				case RECT_RIGHT:
					painter->draw_line({ colbox.right, colbox.top }, { colbox.right, colbox.bottom }, lcolor);
					break;
				case RECT_TOP:
					painter->draw_line({ colbox.left, colbox.top }, { colbox.right, colbox.top }, lcolor);
					break;
				case RECT_BOTTOM:
					painter->draw_line({ colbox.left, colbox.bottom }, { colbox.right, colbox.bottom }, lcolor);
					break;
				}
			}
		}

	END_GAME_LOOP
	
	delete level;
	//delete tilemap;
}
