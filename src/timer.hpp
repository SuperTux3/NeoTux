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
	using duration_t = uint64_t;
	Timer(duration_t ms, int loops = 1);
	~Timer() = default;
	
	void set_duration(duration_t ms);
	void set_loops(int loops = 1);
	
	unsigned get_iterations() const;
	double get_percentage() const;
	
	bool tick();
	void reset();
private:
	int m_loops;
	unsigned m_iterations;
	duration_t m_duration;
	duration_t m_current_time;
	duration_t m_last_time;
};

#endif
