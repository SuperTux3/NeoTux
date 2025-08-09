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

#include "milestone_1_test.hpp"
#include "collision_system.hpp"
#include "input_manager.hpp"
#include "level_screen.hpp"
#include "math/size.hpp"
#include "camera.hpp"
#include "object/all_objects.hpp"
#include "object/retro/retro_player.hpp"
#include "stats.hpp"
#include "stats_overlay.hpp"
#include "timer.hpp"
#include "util/filesystem.hpp"
#include "video/font_cache.hpp"
#include "video/texture_manager.hpp"
#include "video/video_system.hpp"
#include "level_reader.hpp"
#include "tiles_reader.hpp"

#include "collision.hpp"

static FontCache g_font_cache{FS::path("fonts/SuperTux-Medium.ttf"), 32};

std::vector<std::string> _levels = {
	"antarctica.stl",
	"who_is_dawn.stl",
	"long_office_nights.stl",
	"get_to_choppa.stl",
	"where_my_super_cape.stl",
	"redmond_headquarters.stl",
};
int _levels_idx = -1;

void
Milestone1Test::run()
{
	init_all_objects();
	
	// First is considered a completion to start
	bool completed = true;
	Size winsize = g_video_system->get_window_size();
	g_camera.width = winsize.width;
	g_camera.height = winsize.height;
	
	g_tiles_reader.open();	
	
	LevelReader reader;
	Level *level = nullptr;
	Sector *sector = nullptr;
	Tilemap *tilemap = nullptr;
	
	Painter* painter = g_video_system->get_painter();
	painter->register_camera(&g_camera);
	
	StatsOverlay stats;
	LevelScreen levelscreen{nullptr};
	
	RetroPlayer player{};
	
	BEGIN_GAME_LOOP
		handle_events();
		painter->clear();
		
		if (tilemap && player.get_rect().left > (tilemap->get_size().width * 32) - 220)
			completed = true;
			
		if (tilemap && player.get_rect().top > (tilemap->get_size().height * 32))
			player.damage(true);
		
		if (completed)
		{
			++_levels_idx;
			if (_levels_idx >= _levels.size())
				_levels_idx = 0;
			
			delete level;
			level = reader.open("levels/revenge_in_redmond/" + _levels[_levels_idx]);
			sector = &level->get_sector(0);
			tilemap = sector->get_tilemap_by_zpos(0);
			
			player.reset();
			sector->move_to_spawn(player);
			g_stats.reset(false, true);
			levelscreen.set_level(level);
			levelscreen.reset();
			completed = false;
			continue;
		}
		
		if (!levelscreen.finished())
		{
			if (g_mixer.is_playing_music())
				g_mixer.stop_playing_music();
			levelscreen.draw();
			painter->flip();
			continue;
		}
		
		if (levelscreen.just_finished())
		{
			g_mixer.play_music("music/antarctic/chipdisko.ogg");
		}
		
		if (player.is_dead() || !g_stats.tick_timer())
		{
			g_mixer.play_sound("sounds/retro/hurt.wav");
			player.reset();
			sector->move_to_spawn(player);
			g_stats.reset(true);
			levelscreen.reset();
		}
		
		std::cout << g_dtime << std::endl;
		
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
		player.update(*sector, *tilemap);
		level->update();
		tilemap->try_object_collision(player);
		
		if (g_input_manager.is_key_down('a'))
			player.move_left();
		else if (g_input_manager.is_key_down('d'))
			player.move_right();
		
		if (g_input_manager.is_key_down('w'))
			player.jump();
		
		g_camera.x = 
			std::max(0.f,
				std::min<float>(tilemap->get_size().width * 32 - g_camera.width,
				         (player.get_rect().left + player.get_rect().get_width() / 2.f) - g_camera.width / 2.f));
		g_camera.y = 
			std::max(0.f,
				std::min<float>(tilemap->get_size().height * 32 - g_camera.height,
				         (player.get_rect().top + player.get_rect().get_height() / 2.f) - g_camera.height / 2.f));
		
		
		//tilemap->draw(g_camera);
		level->draw();
		player.draw();
		
		g_collision_system.debug_draw();
		
		stats.draw();

		painter->flip();
	END_GAME_LOOP
	
	delete level;
	//delete tilemap;
}
