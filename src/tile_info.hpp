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

#ifndef SUPERTUX_SRC_TILE_INFO_HPP
#define SUPERTUX_SRC_TILE_INFO_HPP

#include <vector>
#include "timer.hpp"
#include "math/size.hpp"
#include "video/texture.hpp"

struct TileInfo
{
	using id_t = unsigned long;
	Size size;
	std::vector<std::string> images;
	int fps;
	Timer timer;
	
	const std::string& get_image() const
	{
		return images[timer.get_iterations() % (images.size())];
	}
};

struct TileMeta
{
	enum Attrs {
		/** solid tile that is indestructible by Tux */
		SOLID	 = 0x0001,
		/** uni-directional solid tile */
		UNISOLID  = 0x0002,
		/** a brick that can be destroyed by jumping under it */
		BRICK	 = 0x0004, //Marked for removal, DO NOT USE!
		/** the level should be finished when touching a goaltile.
		 * if data is 0 then the endsequence should be triggered, if data is 1
		 * then we can finish the level instantly.
		 */
		GOAL	  = 0x0008, //Marked for removal, DO NOT USE!
		/** slope tile */
		SLOPE     = 0x0010,
		/** Bonusbox, content is stored in \a data */
		FULLBOX   = 0x0020, //Marked for removal, DO NOT USE!
		/** Tile is a coin */
		COIN      = 0x0040, //Marked for removal, DO NOT USE!

		/* interesting flags (the following are passed to gameobjects) */
		FIRST_INTERESTING_FLAG = 0x0100,

		/** an ice brick that makes tux sliding more than usual */
		ICE	   = 0x0100,
		/** a water tile in which tux starts to swim */
		WATER  = 0x0200,
		/** a tile that hurts Tux if he touches it */
		HURTS  = 0x0400,
		/** for lava: WATER, HURTS, FIRE */
		FIRE   = 0x0800,
		/** a walljumping trigger tile */
		WALLJUMP  = 0x1000
	};

	Rectf get_src_rect(TextureRef tex);
	
	unsigned x;
	unsigned y;
	TileInfo *info;
	uint16_t attrs;
};

#endif
