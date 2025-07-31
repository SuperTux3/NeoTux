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

#include "tilemap.hpp"
#include "util/logger.hpp"
#include <format>

Tilemap::Tilemap(SexpElt root) :
	m_size(),
	m_tiles(),
	m_zpos()
{
	SexpElt elt;
	elt = root.find_car("width", 1);
	if (elt.next_inplace())
		m_size.width = elt.get_int();
	elt = root.find_car("height", 1);
	if (elt.next_inplace())
		m_size.height = elt.get_int();
	elt = root.find_car("z-pos", 1);
	if (elt.next_inplace())
		m_zpos = elt.get_int();
	
	elt = root.find_car("tiles", 1);
	while (elt.next_inplace())
	{
		long tile_id = elt.get_int();
		// TODO chunking
		m_tiles.emplace_back(tile_id);
	}
	
	
	Logger::debug(std::format("Tilemap info\n\t"
	                          "Width: {}\n\t"
							  "Height: {}\n\t"
							  "z-pos: {}\n\t"
							  "# of tiles: {}", m_size.width, m_size.height, m_zpos, m_tiles.size()));
}

const Tile&
Tilemap::get_tile(unsigned long x, unsigned long y)
{
	return m_tiles.at(x + (y * m_size.width));
}

