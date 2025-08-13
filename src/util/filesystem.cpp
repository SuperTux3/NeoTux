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

#include <string>
#include <iostream>
#include "filesystem.hpp"

#include "config.h"
#ifndef NEOTUX_DATA_DIR
#define NEOTUX_DATA_DIR ""
#endif

namespace FS
{

std::string
path(const std::string& file)
{
	return std::string(NEOTUX_DATA_DIR) + "/" + file;
}

std::string
parent_dir(const std::string &file)
{
	auto pos = file.find_last_of('/');
	if (pos == std::string::npos)
		return file;
	
	return file.substr(0, pos);
}

}
