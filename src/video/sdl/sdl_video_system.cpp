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

#include "sdl_video_system.hpp"

SDLVideoSystem::SDLVideoSystem()
	: m_sdl_renderer(nullptr, &SDL_DestroyRenderer)
{
	init();
}

SDLVideoSystem::~SDLVideoSystem()
{
}

std::string_view
SDLVideoSystem::get_name() const
{
	return "SDL";
}


void
SDLVideoSystem::init()
{
	create_window(0);
	m_sdl_renderer.reset(SDL_CreateRenderer(window.m_sdl_window.get(), NULL));
	//flip();
}

void
SDLVideoSystem::flip()
{
	SDL_RenderPresent(m_sdl_renderer.get());
}

void
SDLVideoSystem::on_resize(int w, int h)
{
}

void
SDLVideoSystem::shutdown()
{
}
