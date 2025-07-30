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

#include "sector.hpp"
#include "util/logger.hpp"

Sector::Sector(SexpElt root)
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
					Logger::debug("Sector Name: " + m_name);
				}
			}
			if (elt.get_value() == "tilemap")
			{
				Logger::debug("Parsing tilemap");
				m_tilemaps.emplace_back(elt);
			}
			
		}
		root.next_inplace();
	}
}
