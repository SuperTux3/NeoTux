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

#ifndef SUPERTUX_SRC_OBJECT_MOVING_OBJECT_HPP
#define SUPERTUX_SRC_OBJECT_MOVING_OBJECT_HPP

#include "collision.hpp"
#include "math/rect.hpp"
#include "math/size.hpp"
#include "game_object.hpp"

class Tilemap;

class MovingObject : public GameObject
{
public:
	MovingObject(Rectf rect, Rectf colbox, std::string_view name);
	virtual ~MovingObject() = default;
	
	static std::string_view class_id() { return "moving-object"; }
	bool parse_sexp(SexpElt elt);
	
	virtual void update(Tilemap &tilemap);
	void draw();
	
	void move(double x, double y);
	void move_to(float x, float y);
	void set_y_vel(double y_vel);
	void set_collidable(bool state) { m_collidable = state; }
	bool is_collidable() const { return m_collidable; };

	Collision::CollideInfo<float> do_collision(Rectf rect, bool do_real_collision_stuff = true);
	Collision::CollideInfo<float>  do_collision(const MovingObject &other, bool do_real_collision_stuff = true)
	{
		if (!other.is_collidable())
			return do_collision(other.get_colbox(), false);
		return do_collision(other.get_colbox(), do_real_collision_stuff);
	}
	Collision::CollideInfo<float> colliding_with(const MovingObject &other) const;
	
	const Rectf& get_rect() const { return m_rect; }
	Rectf get_colbox() const;
	
	void enable_gravity() { m_likes_falling = true; }
	void disable_gravity() { m_likes_falling = false; }
protected:
public:
	double m_y_vel;
	bool m_likes_falling,
		 m_grounded,
		 m_collidable;
	Rectf m_rect;
	Rectf m_colbox;
	Rectf m_last_colbox;
};

#endif
