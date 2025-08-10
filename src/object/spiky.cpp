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
	MovingSprite("images/creatures/spiky/spiky.sprite", "spiky")
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
Spiky::on_collision(Sector &sector, MovingObject &obj, Collision::CollideInfo<float> collide)
{
	Player *player = dynamic_cast<Player*>(&obj);
	if (!player)
		return;
	
	player->damage();
}

void
Spiky::update(Sector &sector, Tilemap &tilemap)
{
	if (m_grounded)
	{
		set_y_vel(1);
	}
	
	MovingSprite::update(sector, tilemap);
}

void
Spiky::draw()
{
	//MovingObject::draw();
	MovingSprite::draw();
	//TextureRef tex = g_texture_manager.load("images/creatures/tux/big/stand-0.png");
	//g_video_system->get_painter()->draw(tex, std::nullopt, m_rect);
}

