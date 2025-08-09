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
#include "sector.hpp"
#include "stats.hpp"

BonusBlock::BonusBlock() :
	MovingSprite("", "bonusblock"),
	m_type(Powerup::NONE),
	m_activated(false)
{
	disable_gravity();
	set_collidable(true);
	set_tilelike(true);
	
	set_action("normal");
}

bool
BonusBlock::parse_sexp(SexpElt elt)
{
	SexpElt telt;
	std::string contents;
	
	telt = elt.find_car("contents");
	if (telt)
	{
		contents = telt.next().get_value();
		m_type = Powerup::EGG;
	}
	
	return MovingSprite::parse_sexp(elt);
}

GameObject*
BonusBlock::construct(SexpElt elt)
{
	BonusBlock *that = new BonusBlock();
	that->parse_sexp(elt);
	return that;
}

void
BonusBlock::update(Sector &sector, Tilemap &tilemap)
{
	
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
				
				auto collide = obj->do_collision(*this, true);
				if (collide.is_colliding())
				{
					if (!player)
						continue;
					
					if (collide.top && !m_activated)
					{
						set_action("empty");
						if (m_type != Powerup::NONE)
						{
							g_mixer.play_sound("sounds/retro/upgrade.wav");
							Powerup *powerup = new Powerup(m_type);
							powerup->move_to(m_rect.left, m_rect.top - m_rect.get_height());
							sector.add_object(powerup);
						}
						else
						{
							g_stats.bump_coins();
						}
						m_activated = true;
					}
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

