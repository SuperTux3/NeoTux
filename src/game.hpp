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
#include <memory>

#define BEGIN_GAME_LOOP double __last_time; \
	g_dtime = 0; \
	while (is_running()) { \
		__last_time = SDL_GetTicks(); \
		if (!g_video_system) continue; \
	
#define END_GAME_LOOP g_video_system->flip(); \
	g_input_manager.reset(); \
	g_dtime = (double)(SDL_GetTicks() - __last_time) / 1000.0; \
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
private:
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
