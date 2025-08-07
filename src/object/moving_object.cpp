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
#include "tilemap.hpp"
#include "video/video_system.hpp"
#include <iostream>

MovingObject::MovingObject(Rectf rect, Rectf colbox, std::string_view name) :
	GameObject(name),
	m_rect(std::move(rect)),
	m_colbox(std::move(colbox)),
	m_grounded(false),
	m_y_vel(0)
{
	
}

Rectf
MovingObject::get_colbox() const
{
	Rectf res{
		m_rect.left + m_colbox.left,
		m_rect.top + m_colbox.top,
		m_rect.left + m_colbox.right - m_colbox.left,
		m_rect.top + m_colbox.bottom - m_colbox.top
	};
	return res;
}

void
MovingObject::update(Tilemap &tilemap)
{
	if (!m_likes_falling)
		return;
	m_y_vel -= .0020581 * g_dtime;
	if (!m_grounded)
	{
		move(0, -m_y_vel * g_dtime);
	}
	
	move(0, -1);
	if (tilemap.try_object_collision(*this) == false && m_grounded)
	{
		m_y_vel = 0;
		m_grounded = false;
	}
	move(0, 1);
}

void
MovingObject::draw()
{
	g_video_system->get_painter()->draw_fill_rect(get_colbox(), {155, 30, 30, 255/2});
	//g_video_system->get_painter()->draw_fill_rect(m_rect, {155, 30, 30, 255/2});
}

void
MovingObject::move(double x, double y)
{
	m_rect.left += x;
	m_rect.right += x;
	m_rect.top += y;
	m_rect.bottom += y;
}

void
MovingObject::move_to(float x, float y)
{
	float old_width = m_rect.get_width(),
	      old_height = m_rect.get_height();
	m_rect.left = x;
	m_rect.right = x + old_width;
	m_rect.top = y;
	m_rect.bottom = y + old_height;
}
