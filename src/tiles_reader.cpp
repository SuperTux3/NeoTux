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

TilesReader g_tiles_reader{};

TilesReader::TilesReader() :
	m_parser()
{}

void
TilesReader::reset()
{
	m_tileinfo.clear();
	m_tiles.clear();
}

void
TilesReader::open(std::string filename)
{
	if (filename.empty())
		filename = "images/tiles.strf";
	SexpElt root, elt, telt, telt_ids, telt_attr, telt_datas;
	
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
			std::vector<std::string> image;
			uint16_t attrs = 0;
			int fps = -1;
			if (elt.get_value() == "tile")
			{
				TileInfo::id_t id = 0;
				telt = elt.find_car("id");
				if (telt)
					id = telt.next().get_int();
				
				telt = elt.find_car("images");
				if (telt)
				{
					while (telt.next_inplace())
					{
						image.push_back(telt.get_value());
					}
				}
				
				telt = elt.find_car("fps");
				if (telt)
				{
					fps = telt.next().get_int();
				}
				
				telt = elt.find_car("solid");
				if (telt)
					if (telt.next() && telt.next().get_value() == "#t")
						attrs |= TileMeta::SOLID;
				
				tileinfo = new TileInfo(
					Size(1,1),
					image,
					fps,
				    Timer((1.0/fps)*1000, -1));
				// This manages memory instead
				m_tileinfo.push_back(std::unique_ptr<TileInfo>(tileinfo));
				m_tiles.emplace(id, TileMeta{0, 0, tileinfo, attrs});
				
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
				{
					while (telt.next_inplace())
					{
						image.push_back(telt.get_value());
					}
				}
				
				telt = elt.find_car("fps");
				if (telt)
					fps = telt.next().get_int();
				
				telt_ids = elt.find_car("ids");
				if (!telt_ids)
					continue;
				
				telt_attr = elt.find_car("attributes");
				if (!telt_attr)
					continue;
				
				telt_datas = elt.find_car("datas");
				
				tileinfo = new TileInfo(
					Size(width, height),
					image,
					fps,
				    Timer((1.0/fps) * 1000, -1)
				);
				// This manages memory instead
				m_tileinfo.push_back(std::unique_ptr<TileInfo>(tileinfo));
				
				for (int i = 0; telt_ids.next_inplace() && telt_attr.next_inplace(); ++i)
				{
					uint16_t attr = telt_attr.get_int();
					uint16_t datas = 0;
					if (telt_datas)
					{
						telt_datas.next_inplace();
						datas = telt_datas.get_int();
					}
					TileInfo::id_t id = telt_ids.get_int();
					if (id == 0)
						continue;
					unsigned x = i % width;
					unsigned y = i / width;
					//std::cout << std::format("x: {} | y: {} | {}", x, y, id) << std::endl;
					
					m_tiles.emplace(id, TileMeta{x, y, tileinfo, attr, datas});
				}
			}
		}
	}
}
