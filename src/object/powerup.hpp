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

#ifndef SUPERTUX_SRC_OBJECT_POWERUP_HPP
#define SUPERTUX_SRC_OBJECT_POWERUP_HPP

#include "object/moving_object.hpp"
#include "object/moving_sprite.hpp"
#include "timer.hpp"

class Powerup : public MovingSprite
{
public:
	enum PowerupType
	{
		NONE,
		EGG,
		COFFEE,
		HERRING
	};
public:
	Powerup(enum PowerupType type);
	virtual ~Powerup() = default;
	
	static std::string_view class_id() { return "powerup"; }
	static GameObject* construct(SexpElt elt);
	
	void update(Sector &sector, Tilemap &tilemap) override;
	void draw() override;
private:
	bool m_dir;
	Timer m_popout_timer;
	enum PowerupType m_type;
};

#endif
