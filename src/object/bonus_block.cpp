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

#include "bonus_block.hpp"
#include "collision.hpp"
#include "collision_system.hpp"
#include "object/player.hpp"

BonusBlock::BonusBlock() :
	MovingSprite("", "bonusblock")
{
	disable_gravity();
	set_collidable(true);
	
	set_action("normal");
}

GameObject*
BonusBlock::construct(SexpElt elt)
{
	BonusBlock *that = new BonusBlock();
	that->parse_sexp(elt);
	return that;
}

void
BonusBlock::update(Tilemap &tilemap)
{
	
	MovingSprite::update(tilemap);
	
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
				
				auto collide = obj->do_collision(*this, true);
				if (collide.is_colliding())
				{
					if (!player)
						continue;
					
					if (collide.top)
						set_action("empty");
				}
			}
		}
	}
}

void
BonusBlock::draw()
{
	MovingSprite::draw();
}

