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
#include <cmath>

Player::Player() :
	MovingSprite("images/creatures/tux/tux.sprite", "tux"),
	m_moving(false),
	m_powerup_state(0),
	m_iframes(1000, 1)
{
	enable_gravity();
	
	set_action(get_size_str()+"stand-right");
}

void
Player::move_left()
{
	m_moving = true;
	move(-0.5 * g_dtime, 0);
	m_flip = (int)FLIP_HORIZONTAL;
	if (m_grounded)
		set_action(get_size_str()+"walk-right");

}

void
Player::move_right()
{
	m_moving = true;
	move(0.5 * g_dtime, 0);
	m_flip = FLIP_NONE;
	if (m_grounded)
		set_action(get_size_str()+"walk-right");
}

void
Player::jump()
{
	if (!m_grounded)
		return;
	
	m_y_vel = 0.75;
	m_grounded = false;
	g_mixer.play_sound("sounds/bigjump.wav");
	set_action(get_size_str()+"jump-right");
}

void
Player::reset()
{
	Rectf colbox = Collision::get_chunk_collisions(get_colbox(), CollisionSystem::COL_HASH_SIZE);
	
	// DISGUSTING HACK: Remove any adjacent cells, since the collision system can be off sometimes
	// Remove object from collision system
	for (int x = colbox.left - 1; x <= colbox.right + 1; ++x)
		for (int y = colbox.top - 1; y <= colbox.bottom + 1; ++y)
			g_collision_system.remove(x, y, this);

	move_to(0, 0);
	m_wants_destruction = false;
	m_powerup_state = 0;
}

std::string
Player::get_size_str()
{
	if (m_powerup_state <= 0)
		return "small-";
	return "big-";
}


void
Player::grow(int amount)
{
	g_mixer.play_sound("sounds/retro/excellent.wav");
	m_powerup_state += amount;
	m_just_grew = true;
}

void
Player::damage()
{
	if (m_iframes.tick())
	{
		g_mixer.play_sound("sounds/retro/hurt.wav");
		if (m_powerup_state > 0)
			m_powerup_state = 0;
		else if (m_powerup_state >= 0)
			--m_powerup_state;
		
		m_iframes.reset();
	}
}

void
Player::update(Sector &sector, Tilemap &tilemap)
{
	if (m_just_grew)
	{
		g_mixer.play_sound("sounds/retro/upgrade.wav");
		g_mixer.play_sound("sounds/retro/excellent.wav");
		m_just_grew = false;
		set_action(get_size_str()+"stand-right");
	}
	
	if (!m_moving && m_grounded)
	{
		set_action(get_size_str()+"stand-right");
	}
	else
		m_moving = false;
	
	if (m_y_vel < -0.1)
		set_action(get_size_str()+"fall-right");
	
	MovingSprite::update(sector, tilemap);
}

bool
Player::is_dead() const
{
	return m_powerup_state == -1 || destroy_me();
}

void
Player::draw()
{
	if (!m_iframes.tick())
	{
		m_alpha = 0.6 + (sin(m_iframes.get_percentage()/3) * 0.1);
	}
	else
		m_alpha = 1.0;
	MovingSprite::draw();
}

