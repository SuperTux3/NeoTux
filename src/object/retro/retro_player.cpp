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
	
	set_action("small-stand-right");
}

void
RetroPlayer::move_left()
{
	m_moving = true;
	move(-0.5 * g_dtime, 0);
	m_flip = (int)FLIP_HORIZONTAL;
	if (m_grounded)
		set_action(get_size_str()+"walk-right");

}

void
RetroPlayer::move_right()
{
	m_moving = true;
	move(0.5 * g_dtime, 0);
	m_flip = FLIP_NONE;
	if (m_grounded)
		set_action(get_size_str()+"walk-right");
}

void
RetroPlayer::jump()
{
	if (!m_grounded)
		return;
	
	m_y_vel = 0.75;
	m_grounded = false;
	g_mixer.play_sound("sounds/bigjump.wav");
	set_action(get_size_str()+"jump-right");
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
	
	if (m_y_vel < -0.1)
		set_action(get_size_str()+"fall-right");
		
	//Player::update(tilemap);
	
	MovingSprite::update(sector, tilemap);
	
	Rectf orig_colbox = get_colbox();
		
	Rectf colbox = Collision::get_chunk_collisions(get_colbox(), CollisionSystem::COL_HASH_SIZE);
	
	for (int x = colbox.left; x <= colbox.right; ++x)
	{
		for (int y = colbox.top; y <= colbox.bottom; ++y)
		{
			const CollisionSystem::object_list_t *objects = g_collision_system.get_objects(x, y);
			if (!objects)
				continue;
			
			for (MovingObject *obj : *objects)
			{
				if (obj == this)
					continue;
				
				auto col = do_collision(*obj, obj->is_tilelike(), orig_colbox);
				if (obj->is_tilelike())
				{
					if (col.is_colliding())
						obj->on_collision(sector, *this, col);
					//do_collision(*obj);
				}
				
				RetroBrick *brick = dynamic_cast<RetroBrick*>(obj);
				if (!brick)
				{
					do_collision(*obj, obj->is_tilelike());
				 	continue;
				}
				auto collide = do_collision(*obj, !obj->m_likes_falling);
				if (collide)
				{
					if (collide.top)
					{
						if (!obj->m_likes_falling)
						{
							obj->enable_gravity();
						}
						else
						{
							obj->set_y_vel(0.3);
						}
					}
					if (collide.bottom)
					{
						if (obj->m_grounded && obj->m_likes_falling)
						{
							obj->set_y_vel(1.0);;
						}
						move(0, -collide.bottom_constraint);
					}
					if (obj->m_likes_falling)
					{
					if (collide.left)
						obj->move(-collide.left_constraint, 0);
					if (collide.right)
						obj->move(collide.right_constraint, 0);
					}
				}
			}
		}
	}
}

void
RetroPlayer::draw()
{
	//MovingObject::draw();
	Player::draw();
	//TextureRef tex = g_texture_manager.load("images/creatures/tux/big/stand-0.png");
	//g_video_system->get_painter()->draw(tex, std::nullopt, m_rect);
}

