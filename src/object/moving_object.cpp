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
#include "object/player.hpp"
#include "sector.hpp"
#include "tilemap.hpp"
#include "video/video_system.hpp"
#include <iostream>

MovingObject::MovingObject(Rectf rect, Rectf colbox, std::string_view name) :
	GameObject(name),
	m_rect(std::move(rect)),
	m_colbox(std::move(colbox)),
	m_grounded(false),
	m_y_vel(0),
	m_collidable(true),
	m_tilelike(false)
{
	
}

MovingObject::~MovingObject()
{
	Rectf colbox = Collision::get_chunk_collisions(get_colbox(), CollisionSystem::COL_HASH_SIZE);
	
	for (int x = colbox.left - 1; x <= colbox.right + 1; ++x)
		for (int y = colbox.top - 1; y <= colbox.bottom + 1; ++y)
			g_collision_system.remove(x, y, this);
}

bool
MovingObject::parse_sexp(SexpElt elt)
{
	SexpElt telt;
	double x{};
	double y{};
	telt = elt.find_car("x");
	if (telt)
		x = telt.next().get_number();
	
	telt = elt.find_car("y");
	if (telt)
		y = telt.next().get_number();
		
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
MovingObject::do_collision(Rectf rect, bool do_real_collision_stuff, std::optional<Rectf> custom_colbox)
{
	auto collide = Collision::aabb(custom_colbox ? *custom_colbox : get_colbox(), rect);
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

void
MovingObject::do_slope_collision(uint16_t sl_t,
                                 Vec2 line_beg, Vec2 line_end,
                                 bool do_real_collision_stuff,
                                 std::optional<Rectf> custom_colbox)
{
	Collision::col_side_t<float> cols[Collision::COL_SIZE_MAX];
	Rectf colbox = (custom_colbox ? *custom_colbox : get_colbox());
	int collide = Collision::line_rect(cols, line_beg, line_end, colbox);
	bool is_slope_roof = (sl_t & Collision::SLOPE_NORTHEAST) == Collision::SLOPE_NORTHEAST ||
		(sl_t & Collision::SLOPE_NORTHWEST) == Collision::SLOPE_NORTHWEST;
	if (collide)
	{
		bool top = false;
		for (int i = 0; i < collide; ++i)
		{
			if (cols[i].first == RECT_TOP)
			{
				top = true;
				break;
			}
		}
		for (int i = 0; i < collide; ++i)
		{
			if (!top)
			{
				if (!is_slope_roof)
				{
					if (cols[i].first == RECT_RIGHT)
						move(0, -(colbox.bottom - cols[i].second.y - 0.2));
					if (cols[i].first == RECT_LEFT)
						move(0, -(colbox.bottom - cols[i].second.y - 0.2));

					m_grounded = true;
				}
			}
			else {
				if (cols[i].first == RECT_RIGHT)
					move(0, -(colbox.top - cols[i].second.y - 0.3));
				if (cols[i].first == RECT_LEFT)
					move(0, -(colbox.top - cols[i].second.y - 0.3));
				// Only when jumping
				if (m_y_vel > 0.1)
					m_y_vel = 0.0;
			}
		}
		m_on_slope = true;
	}
	else
		m_on_slope = false;
}


Collision::CollideInfo<float>
MovingObject::colliding_with(const MovingObject &other) const
{
	return Collision::aabb(get_colbox(), other.get_colbox());
}

void
MovingObject::update(Sector &sector, Tilemap &tilemap)
{
	Rectf colbox_tmp = get_colbox();
	Rectf colbox = Collision::get_chunk_collisions(get_colbox(), CollisionSystem::COL_HASH_SIZE);
	
	if (m_likes_falling)
	{
		m_y_vel -= .0020581 * g_dtime;
		if (!m_grounded)
		{
			move(0, -m_y_vel * g_dtime);
		}

		auto col = tilemap.try_object_collision(*this);
		if (!col && m_grounded)
		{
			m_y_vel = 0;
			m_grounded = false;
		}
		if (col)
			m_colinfo = std::move(*col);
	}
	
	for (int x = colbox.left; x <= colbox.right; ++x)
	{
		for (int y = colbox.top; y <= colbox.bottom; ++y)
		{
			const CollisionSystem::object_list_t *objects = g_collision_system.get_objects(x, y);
			if (!objects)
				continue;
			for (MovingObject *obj : *objects)
			{
				if (obj == this) continue;
				
				auto collide = do_collision(*obj, obj->is_tilelike(), colbox_tmp);
				if (collide)
				{
					on_collision(sector, *obj, collide);
				}
			}
		}
	}

	if (colbox != m_last_colbox)
	{
		// B - A where B is colbox and A is m_last_colbox
		for (long x = m_last_colbox.left; x <= m_last_colbox.right; ++x)
			for (long y = m_last_colbox.top; y <= m_last_colbox.bottom; ++y)
				if (x != std::clamp<long>(x, colbox.left, colbox.right) ||
				    y != std::clamp<long>(y, colbox.top, colbox.bottom))
					g_collision_system.remove(x, y, this);
		
		for (long x = colbox.left; x <= colbox.right; ++x)
			for (long y = colbox.top; y <= colbox.bottom; ++y)
				if (x != std::clamp<long>(x, m_last_colbox.left, m_last_colbox.right) ||
				    y != std::clamp<long>(y, m_last_colbox.top, m_last_colbox.bottom))
					g_collision_system.add(x, y, this);
		
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
