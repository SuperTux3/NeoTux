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

#include "coin.hpp"
#include "collision.hpp"
#include "collision_system.hpp"
#include "object/player.hpp"
#include "stats.hpp"
#include "video/painter.hpp"

Coin::Coin() :
	MovingSprite("", "old-bag"),
	m_just_collected(false),
	m_collected_timer(300.0, 1)
{
	disable_gravity();
	set_collidable(false);
	
	set_action("normal");
}

GameObject*
Coin::construct(SexpElt elt)
{
	Coin *that = new Coin();
	that->parse_sexp(elt);
	return that;
}

void
Coin::update(Sector &sector, Tilemap &tilemap)
{
	//if (m_grounded)
	//	m_y_vel = -15 * g_dtime;
	
	if (m_just_collected)
	{
		if (m_collected_timer.tick())
		{
			mark_for_destruction();
			return;
		}
		m_alpha = 1.0 - (m_collected_timer.get_percentage() / 100.0);
		move(0, -0.4);
		return;
	}
	
	MovingSprite::update(sector, tilemap);
	
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
				if (!player)
					continue;
				
				auto collide = do_collision(*obj, false);
				if (collide.is_colliding())
				{
					m_just_collected = true;
					g_stats.bump_coins();
					m_collected_timer.reset();
					break;
				}
			}
		}
	}
}

void
Coin::draw()
{
	//MovingObject::draw();
	MovingSprite::draw();
	//TextureRef tex = g_texture_manager.load("images/creatures/tux/big/stand-0.png");
	//g_video_system->get_painter()->draw(tex, std::nullopt, m_rect);
}

