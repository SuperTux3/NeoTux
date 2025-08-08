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

#include "retro_brick.hpp"
#include "collision.hpp"
#include "collision_system.hpp"
#include "object/player.hpp"

RetroBrick::RetroBrick() :
	MovingSprite("images/objects/bonus_block/retro_brick.sprite", class_id())
{
	disable_gravity();
	set_action("normal");
}

GameObject*
RetroBrick::construct(SexpElt elt)
{
	RetroBrick *that = new RetroBrick();
	that->parse_sexp(elt);
	return that;
}

void
RetroBrick::update(Tilemap &tilemap)
{
	MovingSprite::update(tilemap);
	
}

void
RetroBrick::draw()
{
	//MovingObject::draw();
	MovingSprite::draw();
	//TextureRef tex = g_texture_manager.load("images/creatures/tux/big/stand-0.png");
	//g_video_system->get_painter()->draw(tex, std::nullopt, m_rect);
}

