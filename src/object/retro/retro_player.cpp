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

#include "retro_player.hpp"
#include "collision_system.hpp"
#include "object/retro/retro_brick.hpp"
#include "video/painter.hpp"

RetroPlayer::RetroPlayer() :
	Player(),
	m_moving(false)
	//MovingObject({0, 0, {60, 100}}, {10, 20, {53, 100}}, "player")
{
	enable_gravity();
	m_physics.set_gravity_modifier(1.2);
	
	set_action("small-stand-right");
}

void
RetroPlayer::controls_move(bool right)
{
	double dir = right ? 1.0 : -1.0;
	m_moving = true;
	move(500 * dir * g_dtime, 0);
	m_flip = right ? FLIP_NONE : FLIP_HORIZONTAL;
	m_direction = right;
	if (m_grounded)
		set_action(get_size_str()+"walk-right");
}

void
RetroPlayer::controls_jump()
{
	m_performed_jump = true;
	if (!m_grounded)
		return;
	
	m_jumped = true;
	set_y_vel(600);
	g_mixer.play_sound("sounds/bigjump.wav");
	set_action(get_size_str()+"jump-right");
}

void
RetroPlayer::handle_input()
{
	if (g_input_manager.mapping_pressed(JUMP_BINDING))
	{
		controls_jump();
	}
	
	if (g_input_manager.mapping_pressed(LEFT_BINDING))
		controls_move(false);
	else if (g_input_manager.mapping_pressed(RIGHT_BINDING))
		controls_move(true);
}

void
RetroPlayer::update(Sector &sector, Tilemap &tilemap)
{
	if (!m_moving && m_grounded)
	{
		set_action(get_size_str()+"stand-right");
	}
	else
		m_moving = false;
	
	if (!m_performed_jump && m_jumped == true && m_physics.get_y_vel() > 0.1)
	{
		m_physics.set_y_vel(0);
		m_jumped = false;
	}
	if (m_performed_jump)
		m_performed_jump = false;
	
	if (m_physics.get_y_vel() < -0.1)
		set_action(get_size_str()+"fall-right");
		
	
	Rectf orig_colbox = get_colbox();
		
	Rectf colbox = Collision::get_chunk_collisions(get_colbox(), CollisionSystem::COL_HASH_SIZE);
	
	MovingSprite::update(sector, tilemap);
}

void
RetroPlayer::draw()
{
	//MovingObject::draw();
	Player::draw();
	//TextureRef tex = g_texture_manager.load("images/creatures/tux/big/stand-0.png");
	//g_video_system->get_painter()->draw(tex, std::nullopt, m_rect);
}

