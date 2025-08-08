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

#ifndef ALL_OBJECTS_HPP
#define ALL_OBJECTS_HPP

#include "object/coin.hpp"
#include "object/game_object.hpp"
#include "object/player.hpp"
#include "object/retro/retro_bag.hpp"
#include "object/retro/retro_brick.hpp"
#include "object/retro/retro_player.hpp"

static void init_all_objects()
{
	GameObject::register_object<Coin>();
	//GameObject::register_object<Player>();
	//GameObject::register_object<RetroPlayer>();
	GameObject::register_object<RetroBag>();
	GameObject::register_object<RetroBrick>();
}

#endif
