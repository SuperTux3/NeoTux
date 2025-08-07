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

#include "retro_bag.hpp"
#include "video/painter.hpp"

RetroBag::RetroBag() :
	MovingSprite("images/creatures/retro/bag/bag.sprite", "old-bag")
{
	enable_gravity();
	
	set_action("bag");
}

GameObject*
RetroBag::construct(SexpElt elt)
{
	return (GameObject*)0x1;
}

void
RetroBag::update(Tilemap &tilemap)
{
	//if (m_grounded)
	//	m_y_vel = -15 * g_dtime;
	if (m_grounded)
	{
		set_y_vel(1);
	}
	
	MovingSprite::update(tilemap);
}

void
RetroBag::draw()
{
	//MovingObject::draw();
	MovingSprite::draw();
	//TextureRef tex = g_texture_manager.load("images/creatures/tux/big/stand-0.png");
	//g_video_system->get_painter()->draw(tex, std::nullopt, m_rect);
}

