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

#include "player.hpp"
#include "collision_system.hpp"
#include "object/moving_object.hpp"
#include "video/painter.hpp"
#include "video/texture_manager.hpp"
#include "video/video_system.hpp"

Player::Player() :
	MovingSprite("images/creatures/tux/tux.sprite", "tux"),
	m_moving(false)
	//MovingObject({0, 0, {60, 100}}, {10, 20, {53, 100}}, "player")
{
	enable_gravity();
	
	set_action("small-stand-right");
}

void
Player::move_left()
{
	m_moving = true;
	move(-0.5 * g_dtime, 0);
	m_flip = (int)FLIP_HORIZONTAL;
	if (m_grounded)
		set_action("small-walk-right");

}

void
Player::move_right()
{
	m_moving = true;
	move(0.5 * g_dtime, 0);
	m_flip = FLIP_NONE;
	if (m_grounded)
		set_action("small-walk-right");
}

void
Player::jump()
{
	if (!m_grounded)
		return;
	
	m_y_vel = 0.75;
	m_grounded = false;
	g_mixer.play_sound("sounds/bigjump.wav");
	set_action("small-jump-right");
}

void
Player::update(Tilemap &tilemap)
{

	if (!m_moving && m_grounded)
	{
		set_action("small-stand-right");
	}
	else
		m_moving = false;
	
	if (m_y_vel < -0.1)
		set_action("small-fall-right");
	
	MovingSprite::update(tilemap);
}

void
Player::draw()
{
	//MovingObject::draw();
	MovingSprite::draw();
	//TextureRef tex = g_texture_manager.load("images/creatures/tux/big/stand-0.png");
	//g_video_system->get_painter()->draw(tex, std::nullopt, m_rect);
}

