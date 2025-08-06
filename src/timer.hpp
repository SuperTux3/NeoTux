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

#ifndef SUPERTUX_SRC_TIMER_HPP
#define SUPERTUX_SRC_TIMER_HPP

#include "game.hpp"

class Timer
{
public:
	Timer(uint64_t ms, int loops = 1);
	~Timer() = default;
	
	double get_percentage();
	bool tick();
private:
	int m_loops;
	uint64_t m_duration;
	uint64_t m_current_time;
	uint64_t m_last_time;
};

#endif
