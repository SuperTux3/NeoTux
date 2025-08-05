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

#include "math/rect.hpp"
#include "math/size.hpp"
#include "game_object.hpp"

class MovingObject : public GameObject
{
public:
	MovingObject(Rectf rect, Rectf colbox, std::string_view name);
	virtual ~MovingObject() = default;
	
	void update();
	void draw() = 0;
	
	void move(float x, float y);
	
	const Rectf& get_rect() const { return m_rect; }
	Rectf get_colbox() const;
	
	void enable_gravity() { m_likes_falling = true; }
	void disable_gravity() { m_likes_falling = false; }
protected:
public:
	float m_y_vel;
	bool m_likes_falling,
		 m_grounded;
	Rectf m_rect;
	Rectf m_colbox;
};

#endif
