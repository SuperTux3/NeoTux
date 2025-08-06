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

#include "timer.hpp"

Timer::Timer(uint64_t ms, int loops /*= 1*/) :
	m_loops(loops),
	m_iterations(0),
	m_duration(ms),
	m_last_time(SDL_GetTicks())
{}

double
Timer::get_percentage()
{
	if (m_loops == 0)
		return 100.0;
	else
		return ((double)(SDL_GetTicks() - m_last_time) / (double)m_duration) * 100;
}

unsigned
Timer::get_iterations()
{
	return m_iterations;
}

bool
Timer::tick()
{
	if (SDL_GetTicks() > m_last_time + m_duration && m_loops != 0)
	{
		if (m_loops != -1)
			--m_loops;
		
		++m_iterations;
		m_last_time = SDL_GetTicks();
		return true;
	}
	return false;
}

