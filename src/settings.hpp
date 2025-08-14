//  NeoTux
//  Copyright (C) 2025 MatusGuy
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

#ifndef HEADER_SUPERTUX_SETTINGS_HPP
#define HEADER_SUPERTUX_SETTINGS_HPP

#include <memory>

#include "config.h"
#include "video/video_system.hpp"

struct Settings {
	bool show_help = false;
	bool verbose = false;
	bool show_hitboxes = false;
	int aggressive_caching =
#		ifdef NEOTUX_PSP
			1;
#		else
			0;
#		endif
	double speed = 1.0;
	int forced_delay = 0;
	VideoSystem::Backend renderer = VideoSystem::VIDEO_AUTO;
};

extern std::unique_ptr<Settings> g_settings;

#endif // HEADER_SUPERTUX_SETTINGS_HPP
