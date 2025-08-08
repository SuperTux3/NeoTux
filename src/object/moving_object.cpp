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

#include "collision.hpp"
#include "collision_system.hpp"
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

bool
MovingObject::parse_moving_object_sexp(SexpElt elt)
{
	double x{};
	double y{};
	SexpElt telt;
	telt = elt.find_car("x");
	if (telt)
		x = telt.next().get_number();
	
	telt = elt.find_car("y");
	if (telt)
		y = telt.next().get_number();
		
	std::cout << x << " " << y << std::endl;
	
	if (x != 0.0 && y != 0.0)
	{
		move_to(x, y);
	}
	
	return true;
}

Rectf
MovingObject::get_colbox() const
{
	Rectf res{
		m_rect.left + m_colbox.left,
		m_rect.top + m_colbox.top,
		m_rect.left + m_colbox.left + m_colbox.right,
		m_rect.top + m_colbox.top + m_colbox.bottom
	};
	return res;
}

Collision::CollideInfo<float>
MovingObject::do_collision(Rectf rect, bool do_real_collision_stuff)
{
	auto collide = Collision::aabb(get_colbox(), rect);
	if (collide.is_colliding() && do_real_collision_stuff)
	{
		if (collide.top)
		{
			if (m_y_vel > 0)
			{
				m_y_vel = 0;
				move(0, collide.top_constraint);
			}
		}
		else if (collide.bottom)
		{
			if (m_y_vel < 0)
			{	
				m_grounded = true;
				move(0, -collide.bottom_constraint);
			}
		}
		else if (collide.left)
			move(collide.left_constraint, 0);
		else if (collide.right)
			move(-collide.right_constraint, 0);
	}
	return collide;
}

Collision::CollideInfo<float>
MovingObject::colliding_with(const MovingObject &other) const
{
	return Collision::aabb(get_colbox(), other.get_colbox());
}

void
MovingObject::update(Tilemap &tilemap)
{
	if (m_likes_falling)
	{
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

	// Test collision	
	Rectf colbox = Collision::get_chunk_collisions(get_colbox(), CollisionSystem::COL_HASH_SIZE);
	
	for (int x = colbox.left; x <= colbox.right; ++x)
	{
		for (int y = colbox.top; y <= colbox.bottom; ++y)
		{
			const std::vector<MovingObject*> *objects = g_collision_system.get_objects(x, y);
			if (!objects)
				continue;
			for (MovingObject *obj : *objects)
			{
				if (obj == this)
					break;
				
				// if (colliding_with(*obj))
				// {
				// 	obj->move_to(0, 0);
				// }
			}
		}
	}
	

	// Update collision spatial hash
	if (colbox != m_last_colbox)
	{
		//std::cout << std::endl << "m_last_colbox: " << m_last_colbox.to_string() << std::endl;
		//std::cout << "colbox: " << colbox.to_string() << std::endl;
		for (long l = colbox.left; l <= colbox.right; ++l)
		{
			for (long ol = m_last_colbox.left; ol <= m_last_colbox.right; ++ol)
			{
				// New area
				if (l != std::clamp<float>(l, m_last_colbox.left, m_last_colbox.right))
					for (long t = colbox.top; t <= colbox.bottom; ++t)
						g_collision_system.add(l, t, this);
				
				// Old area
				if (ol != std::clamp<float>(ol, colbox.left, colbox.right))
					for (long ot = colbox.top; ot <= colbox.bottom; ++ot)
						g_collision_system.remove(ol, ot, this);
			}
		}
		
		for (long l = colbox.top; l <= colbox.bottom; ++l)
		{
			for (long ol = m_last_colbox.top; ol <= m_last_colbox.bottom; ++ol)
			{
				// New area
				if (l != std::clamp<float>(l, m_last_colbox.top, m_last_colbox.bottom))
					for (long t = colbox.left; t <= colbox.right; ++t)
						g_collision_system.add(t, l, this);
				
				// Old area
				if (ol != std::clamp<float>(ol, colbox.top, colbox.bottom))
					for (long ot = colbox.left; ot <= colbox.right; ++ot)
						g_collision_system.remove(ot, ol, this);
			}
		}
	}
	m_last_colbox = colbox;
}

void
MovingObject::draw()
{
	if (g_settings->show_hitboxes)
		g_video_system->get_painter()->draw_fill_rect(get_colbox(), {155, 30, 30, 255/2});
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

void
MovingObject::set_y_vel(double y_vel)
{
	m_y_vel = y_vel;
	m_grounded = false;
}
