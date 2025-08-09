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

#include "spiky.hpp"
#include "collision_system.hpp"
#include "object/player.hpp"
#include "video/painter.hpp"

Spiky::Spiky() :
	MovingSprite("", "spiky")
{
	enable_gravity();
	
	set_action("bag");
}

GameObject*
Spiky::construct(SexpElt elt)
{
	Spiky *that = new Spiky();
	that->parse_sexp(elt);
	return that;
}

void
Spiky::update(Sector &sector, Tilemap &tilemap)
{
	//if (m_grounded)
	//	m_y_vel = -15 * g_dtime;
	if (m_grounded)
	{
		set_y_vel(1);
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
					player->damage();
			}
		}
	}
}

void
Spiky::draw()
{
	//MovingObject::draw();
	MovingSprite::draw();
	//TextureRef tex = g_texture_manager.load("images/creatures/tux/big/stand-0.png");
	//g_video_system->get_painter()->draw(tex, std::nullopt, m_rect);
}

