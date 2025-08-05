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

#include "game.hpp"
#include "moving_object.hpp"

MovingObject::MovingObject(Rectf rect, Sizef colbox, std::string_view name) :
	GameObject(name),
	m_rect(std::move(rect)),
	m_colbox(std::move(colbox)),
	y_vel()
{
	
}

void
MovingObject::update()
{
	y_vel -= 9.81;
	move(0, -y_vel * g_dtime);
}

void
MovingObject::move(float x, float y)
{
	if (x != 0)
	{
		m_rect.left += x;
		m_rect.right += x;
	}
	if (y != 0)
	{
		m_rect.top += y;
		m_rect.bottom += y;
	}
}
