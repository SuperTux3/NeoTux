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

#ifndef HEADER_SUPERTUX_VIDEO_SDL_WINDOW_HPP
#define HEADER_SUPERTUX_VIDEO_SDL_WINDOW_HPP

#include "../../types.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <string>
#include <memory>
#include <iostream>
class SDLWindow
{
public:
	SDLWindow();
	~SDLWindow();
	
	void set_title(const std::string& title);
	void set_icon(const std::string& icon);
	void create_window(u32 flags, const std::string& title);
	
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_sdl_window;
};

#endif // HEADER_SUPERTUX_VIDEO_SDL_WINDOW_HPP
