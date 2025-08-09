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

#include "stats.hpp"

Stats g_stats{};

Stats::Stats() :
	m_timer(1000, -1)
{
	reset();
}

void
Stats::reset()
{
	lives = 3;
	coins = 0;
	time = 120;
	m_timer.reset();
}

void
Stats::bump_coins()
{
	if (++coins >= 100)
	{
		coins = 0;
		++lives;
		g_mixer.play_sound("sounds/retro/lifeup.wav");
	}
	else
		g_mixer.play_sound("sounds/retro/distro.wav");
}

bool
Stats::tick_timer()
{
	if (m_timer.tick() && time > 0)
	{
		if (--time == 0)
			if (lives > 0)
				--lives;
	}
	
	return time > 0;
}
