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

#include "mriceblock.hpp"
#include "audio/mixer.hpp"
#include "collision.hpp"
#include "collision_system.hpp"
#include "object/player.hpp"
#include "video/painter.hpp"

MrIceBlock::MrIceBlock() :
	MovingSprite("", "mriceblock"),
	m_dir(false),
	m_flat(false),
	m_kicked(false),
	m_wakeup(5000.0, -1),
	m_iframe(400.0, 1)
{
	enable_gravity();
	set_collidable(true);
	
	set_action("left");
}

GameObject*
MrIceBlock::construct(SexpElt elt)
{
	MrIceBlock *that = new MrIceBlock();
	that->parse_sexp(elt);
	return that;
}

void
MrIceBlock::update(Sector &sector, Tilemap &tilemap)
{
	if (!m_flat || m_kicked)
		move((m_dir ? 1.0 : -1.0) * (m_kicked ? 0.7 : 0.1) * g_dtime, 0);
	
	if (m_flat && !m_kicked)
	{
		if (m_wakeup.tick())
		{
			m_flat = false;
			set_action("left");
		}
	}
	
	MovingSprite::update(sector, tilemap);
	
	for (auto &colinfo : m_colinfo)
	{
		if (colinfo.left && m_dir == false)
		{
			m_dir = true;
			m_flip = FLIP_HORIZONTAL;
		}
		if (colinfo.right && m_dir == true)
		{
			m_dir = false;
			m_flip = 0;
		}
	}
	
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
				if (obj == this) continue;
				Player *player = dynamic_cast<Player*>(obj);
				
				auto collide = do_collision(*obj, false);
				if (collide.is_colliding())
				{
					if (m_kicked || (!m_kicked && !m_flat))
					{
						if ((collide.left || collide.right) && m_iframe.tick())
						{
							if (player)
							{
								player->damage();
							}
						}
						
						if (m_kicked && !player)
						{
							g_mixer.play_sound("sounds/retro/fall.wav");
							obj->mark_for_destruction();
						}
					}
					
					if (!player)
						return;
					
					if (collide.top)
					{
						if (!m_flat)
						{
							m_wakeup.reset();
							m_iframe.reset();
							m_flat = true;
							set_action("flat-left");
						}
						else {
							m_wakeup.reset();
							m_iframe.reset();
							m_kicked = false;
						}
						player->set_y_vel(0.4);
					}
					
					if (m_flat && !m_kicked && !m_iframe.tick())
					{
						if (collide.left)
						{
							m_iframe.reset();
							m_kicked = true;
							g_mixer.play_sound("sounds/retro/kick.wav");
							m_dir = true;
						}
						if (collide.right)
						{
							m_iframe.reset();
							m_kicked = true;
							g_mixer.play_sound("sounds/retro/kick.wav");
							m_dir = false;
						}
					}
				}
			}
		}
	}
}

void
MrIceBlock::draw()
{
	MovingSprite::draw();
}

