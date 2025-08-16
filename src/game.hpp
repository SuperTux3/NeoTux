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

#ifndef HEADER_SUPERTUX_GAME_HPP
#define HEADER_SUPERTUX_GAME_HPP

#include "audio/mixer.hpp"
#include "input_manager.hpp"
#include "settings.hpp"
#include "util/logger.hpp"
#include <memory>
#include <format>
#include "video/texture_manager.hpp"

#define BEGIN_GAME_LOOP double __last_time; \
	g_dtime = 0; \
	while (is_running()) { \
		__last_time = ((double)SDL_GetTicksNS() * 0.000000001); \
		if (!g_video_system) continue; \
	
#define END_GAME_LOOP g_video_system->flip(); \
	g_texture_manager.poll_unloaded(); \
	g_input_manager.reset(); \
	if (g_settings->forced_delay != 0) \
		SDL_Delay(g_settings->forced_delay); \
	g_dtime = (((double)SDL_GetTicksNS() * 0.000000001) - __last_time) * g_settings->speed; \
	if (g_dtime > .19) { \
		Logger::warn(std::format("Frame took too long! ({:.0f} ms). Skipping", g_dtime * 1000)); \
		g_dtime = 0; \
	} \
}

class Game
{
public:
	Game();
	virtual ~Game() = default;
	
	void init_video_system();
	virtual void run();
	void update();
	void shutdown();
	
	void handle_events();
	
	bool is_running() const { return !m_quit; }
public:
	bool m_quit;
};

extern double g_dtime;

static double
get_fps()
{
	return 1.0 / g_dtime;
}


extern std::unique_ptr<Game> g_game;

#endif
