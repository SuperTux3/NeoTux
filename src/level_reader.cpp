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
#include "level_reader.hpp"
#include "util/filesystem.hpp"
#include "util/logger.hpp"

LevelReader::LevelReader() :
	m_parser()
{}

void*
LevelReader::open(const std::string &filename)
{
	std::string name;
	SexpElt root, elt, tiles;
	
	root = m_parser.read_file(FS::path(filename));
	
	if (!root.is_list())
		return nullptr;
		
	root = root.get_list();
	
	if (root.get_value() == "supertux-level")
		Logger::debug("Claims to be a supertux level...");
	
	elt = root.find_car("version");
	if (elt.next_inplace())
		if (elt.get_int() != 3)
			Logger::warn(std::format("Level format != 3 (Got: {}). Expect bad!", elt.get_int()));
	
	elt = root.find_car("name");
	if (elt.next_inplace())
	{
		if (elt.is_list() && elt.get_list().get_value() == "_")
			elt = elt.get_list().next();
		
		name = elt.get_value();
		Logger::debug("Name: " + name);
	}	
	
	tiles = root.find_car("tiles");
	if (tiles)
		;
	
	return nullptr;
}
