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

#ifndef HEADER_SUPERTUX_VIDEO_TEXTURE_HPP
#define HEADER_SUPERTUX_VIDEO_TEXTURE_HPP

#include "math/size.hpp"
#include "math/rect.hpp"
#include <SDL3/SDL_surface.h>
#include <cstdint>
#include <memory>
#include <string>

class Texture
{
public:
	Texture();
	virtual ~Texture() = default;
	
	void load_file(const std::string &filename);
	//virtual void draw(Texture* texture, Rectf src, Rectf dest) = 0;
	static Texture* create(const std::string &filename, bool as_surface = false);
	static Texture* create(SDL_Surface *surface, bool destroy_surface = true, bool keep_surface = false);
	
	Size get_size() const { return size; }
	void poke_last_used();
	uint64_t get_last_used() const { return m_time; }
	SDL_Surface* get_sdl_surface() const { return m_sdl_surface.get(); }
	static SDL_Surface* create_surface(const std::string &filename);

protected:	
	uint64_t m_time;
	Size size;
	std::unique_ptr<SDL_Surface, decltype(&SDL_DestroySurface)> m_sdl_surface;
};

using TextureRef = Texture*;

#endif
