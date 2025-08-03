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

#ifndef SUPERTUX_SRC_VIDEO_SDL_SURFACE_BLITTER_HPP
#define SUPERTUX_SRC_VIDEO_SDL_SURFACE_BLITTER_HPP

#include <SDL3/SDL_pixels.h>
#include <memory>
#include <SDL3/SDL_surface.h>
#include "math/size.hpp"
#include "video/texture.hpp"

class SurfaceBlitter
{
public:
	SurfaceBlitter(Size size);
	~SurfaceBlitter() = default;
	
	const Size& get_size() const { return m_size; }
	
	SDL_Color read_pixel(int x, int y);
	void write_pixel(int x, int y, SDL_Color color);
	
	void draw_circle(int x, int y, unsigned r, SDL_Color color);
	
	Texture* to_texture();
	void destroy();
private:
	std::unique_ptr<SDL_Surface, decltype(&SDL_DestroySurface)> m_sdl_surface;
	Size m_size;
};

#endif
