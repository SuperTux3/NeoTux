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

#ifndef SUPERTUX_SRC_LEVEL_SCREEN_HPP
#define SUPERTUX_SRC_LEVEL_SCREEN_HPP

#include "timer.hpp"
struct Level;

class LevelScreen
{
public:
	LevelScreen(Level *level);
	~LevelScreen() = default;
	
	void draw();
	void reset();
	bool just_finished();
	bool finished();
	
	void set_level(Level *level) { m_level = level; }
private:
	bool m_checked, m_dont_check_again;
	Level *m_level;
	Timer m_timer;
};

#endif
