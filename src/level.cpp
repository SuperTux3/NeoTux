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

#include <format>
#include "util/logger.hpp"
#include "level.hpp"

Level::Level(SexpElt root) :
	m_sectors(),
	m_name(),
	m_author(),
	m_license()
{
	SexpElt elt;
	
	while (root)
	{
		if (root.is_list())
		{
			elt = root.get_list();
			if (elt.get_value() == "name")
			{
				if (elt.next_inplace())
				{
					if (elt.is_list() && elt.get_list().get_value() == "_")
						elt = elt.get_list().next();
						
					m_name = elt.get_value();
					Logger::debug("Name: " + m_name);
				}
			}
			if (elt.get_value() == "sector")
			{
				Logger::debug("Parsing sector");
				m_sectors.emplace_back(elt);
			}
		}

#if 0
		elt = root.find_car("version");
		if (elt.next_inplace())
			if (elt.get_int() != 3)
				Logger::warn(std::format("Level format != 3 (Got: {}). Expect bad!", elt.get_int()));
#endif
		
		root.next_inplace();
	}
}

void
Level::update()
{
	for (const Sector& sector : m_sectors)
		sector.update();
}

void
Level::draw()
{
	for (Sector& sector : m_sectors)
		sector.draw();
}
