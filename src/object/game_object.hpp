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

#ifndef SUPERTUX_SRC_OBJECT_GAME_OBJECT_HPP
#define SUPERTUX_SRC_OBJECT_GAME_OBJECT_HPP

#include <string>

struct Tilemap;

class GameObject
{
public:
	GameObject(std::string_view name) :
		m_name(name)
	{};
	virtual ~GameObject() = default;
	
	virtual void update(Tilemap &tilemap) = 0;
	virtual void draw() = 0;
	virtual std::string_view get_name() const { return m_name; }
private:
	std::string_view m_name;
};

#endif
