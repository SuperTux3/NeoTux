//  SuperTux 
//  Copyright (C) 2025 MatusGuy
//                2025 Hyland B. <me@ow.swag.toys> 
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

#include "logger.hpp"

Logger g_logger;

void Logger::log(Level lv, const std::string& lvname, const std::string& cat,
                 const std::string& msg)
{
	std::ostream& stream = (lv == ERROR) ? std::cerr : std::cout;
	
	stream << "[" << get_color_code(lv) << lvname << "\033[0m] [" << cat << "] " << msg
	        << std::endl;
}

void Logger::log(Level lv, const std::string& lvname, const std::string& msg)
{
	std::ostream& stream = (lv == ERROR) ? std::cerr : std::cout;

	stream << "[" << get_color_code(lv) << lvname << "\033[0m] " << msg << std::endl;
}

std::string Logger::get_color_code(Level lv)
{
	static char* no_color = std::getenv("NO_COLOR");
	if (no_color != NULL && no_color[0] != '\0')
		return "";
	
	switch (lv) {
	case INFO:
		return "\033[0;36m";

	case WARN:
		return "\033[0;33m";

	case ERROR:
		return "\033[1;31m";

	default:
		return "";
	}
}
