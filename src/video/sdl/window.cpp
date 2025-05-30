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

#include "window.hpp"
#include "sdl_exception.hpp"
#include <SDL3_image/SDL_image.h>

SDLWindow::SDLWindow() :
	m_sdl_window{nullptr, SDL_DestroyWindow}
{
	if (SDL_Init(SDL_INIT_VIDEO) == false)
		throw SDLException("SDL Failed to initialize");
	
}

void
SDLWindow::create_window(u32 flags, const std::string& title)
{
	m_sdl_window.reset(SDL_CreateWindow(title.c_str(), 800, 600, flags));
	
	set_icon("../data/images/engine/supertux-256x256.png");
}

void
SDLWindow::set_icon(const std::string& filename)
{
	try
	{
		SDL_Surface* icon = IMG_Load(filename.c_str());
		if (SDL_SetWindowIcon(m_sdl_window.get(), icon) == false)
			throw SDLException("SDL_SetWindowIcon");
	}
	catch (const SDLException& err) {
		// Ignore this specific error, not our problem
		if (err.sdl_error == "wayland: cannot set icon; required xdg_toplevel_icon_v1 protocol not supported")
			return;
		throw err;
	}
		
	//SDL_DestroySurface(icon);
}
