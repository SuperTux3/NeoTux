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

#include "gui_reader.hpp"
#include "util/filesystem.hpp"
#include "util/logger.hpp"

GuiReader::GuiReader() :
	m_parser()
{}

Widget*
GuiReader::open(const std::string &filename)
{
	SexpElt root;
	
	root = m_parser.read_file(FS::path(filename));
	
	if (!root.is_list())
		return nullptr;
		
	root = root.get_list();
	
	if (root.get_value() == "supertux-gui")
		Logger::debug("Claims to be a gui...");
	
	root.next_inplace();
	return Widget::create(root);
}

