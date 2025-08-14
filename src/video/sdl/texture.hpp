//  SuperTux
//  Copyright (C) 2006 Matthias Braun <matze@braunis.de>
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

#ifndef HEADER_SUPERTUX_VIDEO_SDL_TEXTURE_HPP
#define HEADER_SUPERTUX_VIDEO_SDL_TEXTURE_HPP

#include <memory>
#include <SDL3/SDL_render.h>
#include "math/size.hpp"
#include "video/texture.hpp"

struct SDL_Texture;

class SDLTexture : public Texture
{
public:
	SDLTexture(std::string filename, bool as_surface = false);
	SDLTexture(SDL_Surface * const surface, bool destroy_surface = false, bool store_surface = false);
	~SDLTexture() = default;
	
	SDL_Texture * get_sdl_texture() { return m_sdl_texture.get(); }
private:
	std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_sdl_texture;
	// For blitting purposes, sometimes the sdl surface may be stored
	//std::unique_ptr<SDL_Surface, decltype(&SDL_DestroySurface)> m_sdl_surface;
};

#endif
