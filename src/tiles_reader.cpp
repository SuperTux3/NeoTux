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
#include "tiles_reader.hpp"
#include "util/filesystem.hpp"
#include "util/logger.hpp"

TilesReader::TilesReader() :
	m_parser()
{}

void
TilesReader::open(std::string filename)
{
	if (filename.empty())
		filename = "images/tiles.strf";
	SexpElt root, elt, telt, telt_ids, telt_attr;
	
	root = m_parser.read_file(FS::path(filename));
	
	if (!root.is_list())
		return;
	
	root = root.get_list();
	
	if (root.get_value() == "supertux-tiles")
		Logger::debug("Claims to be supertux tile ids");
	
	while (root.next_inplace())
	{
		if (!root.is_list())
			continue;
		elt = root.get_list();
		if (elt.is_value())
		{
			TileInfo *tileinfo;
			std::string image;
			if (elt.get_value() == "tile")
			{
				TileInfo::id_t id = 0;
				telt = elt.find_car("id");
				if (telt)
					id = telt.next().get_int();
				
				telt = elt.find_car("images");
				if (telt)
					image = telt.next().get_value();
				
				tileinfo = new TileInfo(Size(1,1), image);
				// This manages memory instead
				m_tileinfo.push_back(std::unique_ptr<TileInfo>(tileinfo));
				m_tiles.emplace(id, TileMeta{0, 0, tileinfo});
				
			}
			else if (elt.get_value() == "tiles")
			{
				long width = 0, height = 0;
				telt = elt.find_car("width");
				if (telt)
					width = telt.next().get_int();
				telt = elt.find_car("height");
				if (telt)
					height = telt.next().get_int();
				telt = elt.find_car("images");
				if (telt)
					image = telt.next().get_value();
				
				telt_ids = elt.find_car("ids");
				if (!telt_ids)
					continue;
				
				telt_attr = elt.find_car("attributes");
				if (!telt_attr)
					continue;
				
				tileinfo = new TileInfo(Size(width, height), image);
				// This manages memory instead
				m_tileinfo.push_back(std::unique_ptr<TileInfo>(tileinfo));
				
				for (int i = 0; telt_ids.next_inplace() && telt_attr.next_inplace(); ++i)
				{
					uint16_t attr = telt_attr.get_int();
					TileInfo::id_t id = telt_ids.get_int();
					if (id == 0)
						continue;
					unsigned x = i % width;
					unsigned y = i / width;
					//std::cout << std::format("x: {} | y: {} | {}", x, y, id) << std::endl;
					
					m_tiles.emplace(id, TileMeta{x, y, tileinfo, attr});
				}
			}
		}
	}
}
