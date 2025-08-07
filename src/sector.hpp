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

#ifndef SUPERTUX_SRC_SECTOR_HPP
#define SUPERTUX_SRC_SECTOR_HPP

#include <string>
#include <vector>
#include <memory>
#include "object/moving_object.hpp"
#include "tilemap.hpp"
#include "util/sexp.hpp"

class Sector
{
public:
	Sector(SexpElt root);
	~Sector() = default;
	
	
	Tilemap& get_tilemap(size_t idx) { return m_tilemaps[idx]; }
	Tilemap* get_tilemap_by_zpos(long zpos);
	const size_t num_tilemaps() const { return m_tilemaps.size(); }
	Tilemap& operator[](size_t idx) { return get_tilemap(idx); }
private:
	std::string m_name;
	
	std::vector<std::shared_ptr<MovingObject>> m_objects;
	std::vector<Tilemap> m_tilemaps;
};

#endif
