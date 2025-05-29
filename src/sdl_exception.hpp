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

#ifndef SUPERTUX_SRC_VIDEO_SDL_SDL_EXCEPTION_HPP
#define SUPERTUX_SRC_VIDEO_SDL_SDL_EXCEPTION_HPP

#include <SDL3/SDL_error.h>
#include <stdexcept>
#include <string>

class SDLException : std::runtime_error
{
public:
	SDLException(const std::string& what = "") :
		sdl_error{SDL_GetError()},
		std::runtime_error(what + ": " + SDL_GetError())
	{}
	
	const std::string sdl_error;
};

#endif
