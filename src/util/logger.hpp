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

// SPDX-License-Identifier: LGPL-3.0-only

#ifndef SUPERTUX_SRC_UTIL_LOGGER
#define SUPERTUX_SRC_UTIL_LOGGER

#include <cstdint>
#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>
#include <fmt/format.h>

class Logger;
extern Logger g_logger;

#define LOGGING_LEVEL(lv, name, func)                                      \
	static constexpr uint8_t name = lv;                                    \
	static void func(const std::string& msg)                               \
	{                                                                      \
		g_logger.log(lv, #name, msg);                                      \
	}                                                                      \
	static void func(const std::string& cat, const std::string& msg)       \
	{                                                                      \
		g_logger.log(lv, #name, cat, msg);                                 \
	}

class Logger
{
public:
	using Level = uint8_t;

	Logger() = default;

	void log(Level lv, const std::string& lvname, const std::string& cat, const std::string& msg);
	void log(Level lv, const std::string& lvname, const std::string& msg);

	std::string get_color_code(Level lv);

	static constexpr uint8_t DEBUG = 0;
	static void debug(const std::string& msg)
	{
#ifndef NDEBUG
		g_logger.log(DEBUG, "DEBUG", msg);
#endif
	}
	static void debug(const std::string& cat, const std::string& msg)
	{
#ifndef NDEBUG
		g_logger.log(DEBUG, "DEBUG", cat, msg);
#endif
	}

	LOGGING_LEVEL(1, INFO, info)
	LOGGING_LEVEL(2, WARN, warn)
	LOGGING_LEVEL(3, ERROR, error)

	static void fatal(const std::string& cat, const std::string& msg, int exitcode = 1)
	{
		Logger::error(cat, msg);
		std::exit(exitcode);
	}

	static void fatal(const std::string& msg, int exitcode = 1)
	{
		Logger::error(msg);
		std::exit(exitcode);
	}
};

#endif
