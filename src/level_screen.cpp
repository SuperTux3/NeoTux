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

#include "level_screen.hpp"
#include "camera.hpp"
#include "level.hpp"
#include "util/filesystem.hpp"
#include "video/font_cache.hpp"
#include "stats.hpp"
#include "video/video_system.hpp"
#include <format>

static FontCache font_cache{FS::path("fonts/SuperTux-Medium.ttf"), 30};

LevelScreen::LevelScreen(Level *level) :
	m_level(level),
	m_timer(3000, 1),
	m_checked(false),
	m_dont_check_again(false)
{
}

bool
LevelScreen::just_finished()
{
	bool not_checked = m_checked;
	m_checked = false;
	m_dont_check_again = true;
	return not_checked;
}

bool
LevelScreen::finished()
{
	bool tick = m_timer.tick();
	if (tick && !m_dont_check_again)
		m_checked = true;
	return tick;
}

void
LevelScreen::reset()
{
	m_timer.reset();
	m_checked = false;
	m_dont_check_again = false;
}

void
LevelScreen::draw()
{
	Painter *painter = g_video_system->get_painter();
	Camera *last_camera = painter->unregister_camera();
	Camera camera{last_camera->width, last_camera->height};
	painter->register_camera(&camera);
	
	TextureRef lvlname = font_cache.load(std::format("{}", m_level->get_name()), {255, 220, 220, 225});
	TextureRef lives = font_cache.load(std::format("Lives x {}", g_stats.lives), {255, 220, 220, 255});

	painter->draw_fill_rect({0, 0, (float)camera.width, (float)camera.height}, {0, 0, 0, 255});
	
	painter->draw(lvlname, std::nullopt,
		Rectf{(float)(camera.width/2.f - lvlname->get_size().width/2.f), 
		      (float)(camera.height/2.f - lvlname->get_size().height/2.f) - lvlname->get_size().height,
		      {(float)lvlname->get_size().width, (float)lvlname->get_size().height}});
	painter->draw(lives, std::nullopt,
		Rectf{(float)(camera.width/2.f - lives->get_size().width/2.f), 
		      (float)(camera.height/2.f - lives->get_size().height/2.f) + lives->get_size().height,
		      {(float)lives->get_size().width, (float)lives->get_size().height}});
	
	painter->register_camera(last_camera);
}
