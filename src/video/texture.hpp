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
#include <string>

class Texture
{
public:
	Texture();
	virtual ~Texture() = default;
	
	void load_file(const std::string &filename);
	//virtual void draw(Texture* texture, Rectf src, Rectf dest) = 0;
	static Texture* create(const std::string &filename);
	static Texture* create(SDL_Surface *surface);
protected:
	static SDL_Surface* create_surface(const std::string &filename);

	Size size;	
};

using TextureRef = Texture*;

#endif
