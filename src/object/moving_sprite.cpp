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

#include "moving_sprite.hpp"
#include "object/moving_object.hpp"
#include "video/texture_manager.hpp"
#include "video/video_system.hpp"

MovingSprite::MovingSprite(std::string_view name) :
	MovingObject({0, 0, 0, 0}, {0, 0, 0, 0}, std::move(name))
{
}

MovingSprite::MovingSprite(const std::string &sprite_file, std::string_view name) :
	MovingSprite(name)
{
	enable_gravity();
}

MovingSprite::MovingSprite(SexpElt selt, std::string_view name) :
	MovingSprite(name)
{
	
}

void
MovingSprite::update(Tilemap &tilemap)
{
	MovingObject::update(tilemap);
}

void
MovingSprite::draw()
{
	//MovingObject::draw();
	TextureRef tex = g_texture_manager.load("images/creatures/tux/big/stand-0.png");
	g_video_system->get_painter()->draw(tex, std::nullopt, m_rect);
}
