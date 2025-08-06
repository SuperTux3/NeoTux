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

#include "player.hpp"
#include "object/moving_object.hpp"
#include "video/texture_manager.hpp"
#include "video/video_system.hpp"

Player::Player() :
	MovingObject({0, 0, {60, 100}}, {10, 20, {53, 100}}, "player")
{
	enable_gravity();
}

void
Player::update(Tilemap &tilemap, TilesReader &reader)
{
	MovingObject::update(tilemap, reader);
}

void
Player::draw()
{
	//MovingObject::draw();
	TextureRef tex = g_texture_manager.load("images/creatures/tux/big/stand-0.png");
	g_video_system->get_painter()->draw(tex, std::nullopt, m_rect);
}

