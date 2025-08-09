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

#include "stats_overlay.hpp"
#include "camera.hpp"
#include "util/filesystem.hpp"
#include "video/font_cache.hpp"
#include "stats.hpp"
#include "video/video_system.hpp"
#include <format>

static FontCache font_cache{FS::path("fonts/SuperTux-Medium.ttf"), 24};

StatsOverlay::StatsOverlay()
{
}

void
StatsOverlay::draw()
{
	Painter *painter = g_video_system->get_painter();
	Camera *last_camera = painter->unregister_camera();
	Camera camera{last_camera->width, last_camera->height};
	painter->register_camera(&camera);
	
	TextureRef time = font_cache.load(std::format("Time: {}", g_stats.time), {255, 220, 220, 225});
	TextureRef coins = font_cache.load(std::format("Coins: {}", g_stats.coins), {255, 220, 220, 225});
	TextureRef lives = font_cache.load(std::format("Lives: {}", g_stats.lives), {255, 220, 220, 225});

	
	painter->draw(time, std::nullopt,
		Rectf{0, 0,
		      {(float)time->get_size().width, (float)time->get_size().height}});
	painter->draw(coins, std::nullopt,
		Rectf{(float)g_camera.width - lives->get_size().width, 0,
		      {(float)coins->get_size().width, (float)coins->get_size().height}});
	painter->draw(lives, std::nullopt,
		Rectf{(float)g_camera.width - lives->get_size().width,
		      (float)lives->get_size().height,
		      {(float)lives->get_size().width, (float)lives->get_size().height}});
	
	painter->register_camera(last_camera);
}
