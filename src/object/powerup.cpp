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

#include "powerup.hpp"
#include "collision.hpp"
#include "collision_system.hpp"
#include "object/player.hpp"
#include "video/painter.hpp"
#include "video/texture_manager.hpp"

Powerup::Powerup(enum PowerupType type) :
	MovingSprite("", "powerup"),
	m_dir(true),
	m_type(std::move(type)),
	m_popout_timer(800, 1)
{
	enable_gravity();
	set_collidable(true);
	
	switch (m_type)
	{
	case Powerup::EGG:
		set_sprite("images/powerups/egg/egg.sprite");
		break;
	case Powerup::COFFEE:
		//wtf
		break;
	}
	
	set_action("default");
}

GameObject*
Powerup::construct(SexpElt elt)
{
	Powerup *that = new Powerup(Powerup::COFFEE);
	that->parse_sexp(elt);
	return that;
}

void
Powerup::update(Sector &sector, Tilemap &tilemap)
{
	if (!m_popout_timer.tick())
		return;
	
	move((m_dir ? 1.0 : -1.0) * 0.1 * g_dtime, 0);
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
				if (!player)
					continue;
				
				auto collide = do_collision(*obj, false);
				if (collide.is_colliding())
				{
					player->grow();
					mark_for_destruction();
				}
			}
		}
	}
}

void
Powerup::draw()
{
	//MovingSprite::draw();
	if (!m_action)
		return;
	TextureRef tex = g_texture_manager.load(m_parent_dir + "/" + m_action->get_image(m_action_timer));
	float crop_height =
		(m_popout_timer.get_percentage()/100) * tex->get_size().height;
	Rectf crop_sprite{
		0,
		-tex->get_size().height + crop_height,
		{ (float)tex->get_size().width,
		  (float)tex->get_size().height }
	};
	Rectf crop_res = m_rect;
	crop_res.top += m_rect.get_height() - (crop_height);
	g_video_system->get_painter()->draw(tex, crop_sprite, crop_res, m_flip, m_alpha);
	MovingObject::draw();
}

