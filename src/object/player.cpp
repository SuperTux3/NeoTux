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
#include "input_manager.hpp"
#include "object/moving_object.hpp"
#include "video/painter.hpp"
#include "video/video_system.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numbers>

constexpr double JUMP_EARLY_APEX_FACTOR = 3.0;
constexpr double JUMP_GRACE_TIME = 0.25;
constexpr double MAX_WALK_XM = 230;

Player::Player() :
	MovingSprite("images/creatures/tux/tux.sprite", "tux"),
	m_state(PLAYER_STATE_SIZE),
	m_powerup_state(0),
	m_iframes(1000, 1)
{
	enable_gravity();
	m_physics.set_gravity_modifier(1.0);
	set_action(get_size_str()+"stand-right");
}

void
Player::try_jump_apex()
{
	bool early_apex = m_state.get(PLAYER_JUMP_EARLY_APEX);
	m_state.set(PLAYER_JUMP_EARLY_APEX, !early_apex);
	m_physics.set_gravity_modifier(early_apex ? 1.0 : JUMP_EARLY_APEX_FACTOR);
}


void
Player::controls_move(bool right)
{
	bool already_moved = false;
	bool cant_slope_move = false;
	double dir = right ? 1.0 : -1.0;
	if (!(m_slope_normals.size() > 1 &&
	      m_slope_normals[1].x + m_slope_normals[0].x == 0.0))
	{
		for (auto &slope_normal : m_slope_normals)
		{
			Vec2 slope_rotated = slope_normal.rotate90();
			if (m_on_slope && !already_moved)
			{
				move(slope_rotated.x * -dir * 0.5 * g_dtime, slope_rotated.y * -dir * 0.5 * g_dtime);
				already_moved = true;
			}
		}
	}
	else
		cant_slope_move = true;
	m_state.set(PLAYER_MOVING, true);
	if (!m_on_slope || m_physics.get_y_vel() < -0.03 || cant_slope_move)
		m_physics.set_x_vel(m_physics.get_x_vel() + 100 * dir);;
	m_flip = right ? FLIP_NONE : FLIP_HORIZONTAL;
	m_direction = right;
	if (m_grounded || m_on_slope)
		set_action(get_size_str()+"walk-right");
}

void
Player::controls_jump()
{
	if (!m_grounded)
		return;
	
	m_physics.set_y_vel(std::abs(m_physics.get_x_vel()) > MAX_WALK_XM ? 580.0 : 520.0);
	m_state.set(PLAYER_JUMPING, true);
	m_grounded = false;
	g_mixer.play_sound("sounds/bigjump.wav");
	set_action(get_size_str()+"jump-right");
}

void
Player::handle_input()
{
	if (g_input_manager.is_key_down('w'))
	{
		controls_jump();
	}
	else if (!g_input_manager.is_key_down('w')) {
		if (m_state.get(PLAYER_JUMPING))
		{
			m_state.set(PLAYER_JUMPING, false);
			try_jump_apex();
		}
	}
	
	if (m_state.get(PLAYER_JUMP_EARLY_APEX) && m_physics.get_y_vel() <= 0)
		try_jump_apex();
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
	m_state.set(PLAYER_JUST_GREW, true);
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
	if (m_state.get(PLAYER_JUST_GREW))
	{
		g_mixer.play_sound("sounds/retro/upgrade.wav");
		g_mixer.play_sound("sounds/retro/excellent.wav");
		m_state.set(PLAYER_JUST_GREW, false);
		set_action(get_size_str()+"stand-right");
	}
	
	m_physics.set_x_vel(m_physics.get_x_vel() * 0.85);
	//m_physics.set_x_vel(std::clamp(m_physics.get_x_vel(), -1000.0, 1000.0));
	if (!m_state.get(PLAYER_MOVING) && m_physics.get_x_vel() == std::clamp(m_physics.get_x_vel(), -0.25, 0.25))
	{
		m_physics.set_x_vel(0);
		if (m_grounded || m_on_slope)
			set_action(get_size_str()+"stand-right");
	}
	
	if (!m_state.get(PLAYER_MOVING) && (m_grounded || m_on_slope))
	{
		//set_action(get_size_str()+"stand-right");
	}
	else
		m_state.set(PLAYER_MOVING, false);
	
	move(0.5 * (m_physics.get_x_vel() * g_dtime), 0);
	MovingSprite::update(sector, tilemap);
	
	if (!m_on_slope && m_physics.get_y_vel() < -0.1)
		set_action(get_size_str()+"fall-right");
	
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

