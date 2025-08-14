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

#include "texture.hpp"
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_image/SDL_image.h>
#include "sdl_exception.hpp"
#include "video/sdl/surface_blitter.hpp"
#include "video/sdl/texture.hpp"
#include "video/video_system.hpp"

Texture::Texture() :
	m_time(0),
	m_sdl_surface(nullptr, SDL_DestroySurface)
{}

void
Texture::load_file(const std::string& filename)
{
}

Texture*
Texture::create(const std::string &filename, bool as_surface)
{
	switch (g_video_system->get_video_system())
	{
	case VideoSystem::Backend::VIDEO_SDL:
		return new SDLTexture(filename, as_surface);
	}
}

Texture*
Texture::create(SDL_Surface *surface)
{
	switch (g_video_system->get_video_system())
	{
	case VideoSystem::Backend::VIDEO_SDL:
		return new SDLTexture(surface, false);
	}
}

void
Texture::poke_last_used()
{
	m_time = SDL_GetTicks();
}

SDL_Surface*
Texture::create_surface(const std::string& filename)
{
	SDL_Surface* image = IMG_Load(filename.c_str());
#ifdef NEOTUX_PSP
	// Size of PSP textures can only be so large
	// TODO should downscale later 
	if (image->w >= 512)
		image->w = 512;
	if (image->h >= 512)
		image->h = 512;
#endif
	if (!image)
		throw SDLException("IMG_Load");
	return image;
}
