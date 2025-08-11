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
#include "collision.hpp"
#include "collision_system.hpp"
#include "object/moving_object.hpp"
#include "video/painter.hpp"
#include "video/video_system.hpp"
#include <cmath>
#include <iostream>
#include <numbers>

Player::Player() :
	MovingSprite("images/creatures/tux/tux.sprite", "tux"),
	m_moving(false),
	m_powerup_state(0),
	m_iframes(1000, 1)
{
	enable_gravity();
	
	set_action(get_size_str()+"stand-right");
}

// TODO Merge these functions
void
Player::move_left()
{
	bool already_moved = false;
	bool cant_slope_move = false;
	if (!(m_slope_normals.size() > 1 &&
	      m_slope_normals[1].x + m_slope_normals[0].x == 0.0))
	{
		for (auto &slope_normal : m_slope_normals)
		{
			Vec2 slope_rotated = slope_normal.rotate90();
			if (m_on_slope && !already_moved)
			{
				move(slope_rotated.x * 0.6, slope_rotated.y * 0.6);
				already_moved = true;
			}
		}
	}
	else
		cant_slope_move = true;
	m_moving = true;
	if (!m_on_slope || m_y_vel < -0.03 || cant_slope_move)
		move(-0.5 * g_dtime, 0);
	m_flip = (int)FLIP_HORIZONTAL;
	m_direction = false;
	if (m_grounded || m_on_slope)
		set_action(get_size_str()+"walk-right");

}

void
Player::move_right()
{
	bool already_moved = false;
	bool cant_slope_move = false;
	if (!(m_slope_normals.size() > 1 &&
	      m_slope_normals[1].x + m_slope_normals[0].x == 0.0))
	{
		for (auto &slope_normal : m_slope_normals)
		{
			Vec2 slope_rotated = slope_normal.rotate90();
			if (m_on_slope && !already_moved)
			{
				move(-slope_rotated.x * 0.6, -slope_rotated.y * 0.6);
				already_moved = true;
			}
		}
	}
	else
		cant_slope_move = true;
	m_moving = true;
	if (!m_on_slope || cant_slope_move)
		move(0.5 * g_dtime, 0);
	m_flip = FLIP_NONE;
	m_direction = true;
	if (m_grounded || m_on_slope)
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
Player::damage(bool kill)
{
	if (m_iframes.tick())
	{
		g_mixer.play_sound("sounds/retro/hurt.wav");
		
		if (kill)
			m_powerup_state = -1;
		else if (m_powerup_state > 0)
			m_powerup_state = 0;
		else if (m_powerup_state >= 0)
			--m_powerup_state;
		
		m_iframes.reset();
	}
}

void
Player::update(Sector &sector, Tilemap &tilemap)
{
	//std::cout << angle << std::endl;
	
	if (m_just_grew)
	{
		g_mixer.play_sound("sounds/retro/upgrade.wav");
		g_mixer.play_sound("sounds/retro/excellent.wav");
		m_just_grew = false;
		set_action(get_size_str()+"stand-right");
	}
	
	if (!m_moving && (m_grounded || m_on_slope))
	{
		set_action(get_size_str()+"stand-right");
	}
	else
		m_moving = false;
	
	MovingSprite::update(sector, tilemap);
	
	if (!m_on_slope && m_y_vel < -0.1)
		set_action(get_size_str()+"fall-right");
	
	std::cout << m_slope_normals[0].to_string() << " " << m_slope_normals[1].to_string() << std::endl;
	std::cout << m_slope_normals[1].y + m_slope_normals[0].y << std::endl;
	if (m_slope_normals.size() > 1 &&
		m_slope_normals[1].x + m_slope_normals[0].x == 0.0)
	{
		// Return early so we don't slide Tux
		return;
	}
	
	if (m_on_slope)
	{
		using namespace Collision;
		uint16_t deform_mask = (m_slope_data & Collision::SlopeInfo::DEFORM_MASK);
		if (deform_mask == DEFORM_LEFT || deform_mask == DEFORM_RIGHT)
			move(m_slope_normal.x * 0.03, -m_slope_normal.y * 0.03);
	}
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
	Vec2 colbox = {get_colbox().left, get_colbox().top};
	g_video_system->get_painter()->draw_line(colbox, colbox + m_slope_normal * 60, {0, 255, 0, 255});
	MovingSprite::draw();
}

