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
#include "camera.hpp"
#include "collision_system.hpp"
#include "util/logger.hpp"
#include <cassert>

Sector::Sector(SexpElt root) :
	m_objects(),
	m_zero_tilemap(nullptr)
{
	SexpElt elt;
	size_t zero_tilemap_idx = 0;
	
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
			else if (elt.get_value() == "tilemap") {
				Logger::debug("Parsing tilemap");
				Tilemap tm(elt);
				// HACK: Insert zpos in order for now
				long zpos = tm.get_zpos();
				if (m_tilemaps.size() > 0)
				for (int i = 0; i < m_tilemaps.size(); ++i)
				{
					if (zpos <= m_tilemaps[i].get_zpos())
					{
						m_tilemaps.emplace(m_tilemaps.begin() + i, tm);
						if (i <= zero_tilemap_idx)
							++zero_tilemap_idx;
						break;
					}
				}
				else
					m_tilemaps.emplace_back(tm);
				
				if (tm.get_zpos() == 0)
				{
					// iterators/pointers become invalidated, so we get at this last
					zero_tilemap_idx = m_tilemaps.size()-1;
				}
			}
			else {
				std::string obj_name = elt.get_value();
				MovingObject *obj = static_cast<MovingObject*>(GameObject::create(elt));
				if (obj)
					m_objects.emplace_back(std::shared_ptr<MovingObject>(obj));
			}
			
		}
		root.next_inplace();
	}
	
	// Cache zero tilemap for update calls
	m_zero_tilemap = &m_tilemaps[zero_tilemap_idx];
	assert(m_zero_tilemap->get_zpos() == 0);
}

void
Sector::update()
{
	assert(m_zero_tilemap);
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		MovingObject *obj = it->get();
		obj->update(*m_zero_tilemap);
		if (obj->destroy_me())
		{
			m_objects.erase(it);
			continue;
		}
		m_zero_tilemap->try_object_collision(*obj);
	}
}

void
Sector::draw()
{
	for (auto &object : m_objects)
	{
		object.get()->draw();
	}
	
	for (auto &object : m_tilemaps)
	{
		object.draw(g_camera);
	}
}

Tilemap*
Sector::get_tilemap_by_zpos(long zpos)
{
	for (Tilemap &tilemap : m_tilemaps)
	{
		if (tilemap.get_zpos() == zpos)
			return &tilemap;
	}
	
	return nullptr;
}

