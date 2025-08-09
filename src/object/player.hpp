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

#ifndef SUPERTUX_SRC_OBJECT_PLAYER_HPP
#define SUPERTUX_SRC_OBJECT_PLAYER_HPP

#include "object/moving_object.hpp"
#include "object/moving_sprite.hpp"
#include "timer.hpp"

class Player : public MovingSprite
{
public:
	Player();
	virtual ~Player() = default;
	
	void update(Sector &sector, Tilemap &tilemap) override;
	void draw() override;
	
	void move_left();
	void move_right();
	void jump();
	
	void reset();
	
	void grow(int amt = 1);
	void damage();
	bool is_dead() const;
protected:
	std::string get_size_str();
	int m_powerup_state;
	bool m_just_grew;
	bool m_moving;
	Timer m_iframes;
};

#endif
