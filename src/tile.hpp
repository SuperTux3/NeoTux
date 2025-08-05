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

#ifndef SUPERTUX_SRC_TILE_HPP
#define SUPERTUX_SRC_TILE_HPP

class Tile
{
public:
	Tile() :
		m_id(0)
	{}
	Tile(unsigned long id);
	~Tile() = default;
	
	void set_id(unsigned long id) { m_id = id; } 
	unsigned long get_id() const { return m_id; }
private:
	unsigned long m_id;
};

#endif
