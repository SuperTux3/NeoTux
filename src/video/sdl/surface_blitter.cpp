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

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_endian.h>
#include <SDL3/SDL_surface.h>
#include "surface_blitter.hpp"

SurfaceBlitter::SurfaceBlitter(Size size) :
	m_sdl_surface(nullptr, SDL_DestroySurface),
	m_size(std::move(size))
{
	m_sdl_surface.reset(SDL_CreateSurface(m_size.width, m_size.height, SDL_PIXELFORMAT_ABGR32));
}

SDL_Color
SurfaceBlitter::read_pixel(int x, int y)
{
	if (x < 0 || y < 0 || x > m_size.width || y > m_size.height)
		return {0, 0, 0, 0};
	SDL_Color res;
	SDL_Surface *surface = m_sdl_surface.get();
	Uint8 *p;
	p = reinterpret_cast<Uint8*>(surface->pixels) + y * surface->pitch + x * SDL_BYTESPERPIXEL(surface->format);
	// TODO memcpy (if it even matters)
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	res.r = p[0];
	res.g = p[1];
	res.b = p[2];
	res.a = p[3];
#else
	res.r = p[3];
	res.g = p[2];
	res.b = p[1];
	res.a = p[0];
#endif
	return res;
}

void
SurfaceBlitter::write_pixel(int x, int y, SDL_Color color)
{
	if (x < 0 || y < 0 || x > m_size.width || y > m_size.height)
		return;
	SDL_Surface *surface = m_sdl_surface.get();
	Uint8 *p;
	p = reinterpret_cast<Uint8*>(surface->pixels) + y * surface->pitch + x * SDL_BYTESPERPIXEL(surface->format);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	p[0] = color.r;
	p[1] = color.g;
	p[2] = color.b;
	p[3] = color.a;
#else
	p[0] = color.a;
	p[1] = color.b;
	p[2] = color.g;
	p[3] = color.r;
#endif
	//SDL_WriteSurfacePixel(m_sdl_surface.get(), x, y, color.r, color.g, color.b, color.a);
}

// bresenham's circle
void
SurfaceBlitter::draw_circle(int x, int y, unsigned r, SDL_Color color)
{
	x -= m_size.width/2;
	y -= m_size.height/2;
	int xx = 0;
	int yy = r;
	int w = 1 - r;
	while (xx < yy)
	{
		if (w < 0)
		{
			w += 2 * xx + 3;
		}
		else {
			w += 2 * (xx - yy) + 5;
			yy -= 1;
		}
		xx += 1;
		// bottom right
		write_pixel(yy+m_size.height/2-1 + x, xx+m_size.width/2-1 + x, color);
		write_pixel(xx+m_size.width/2-1 + x, yy+m_size.height/2-1 + y, color);
		
		// top right
		write_pixel(yy+m_size.height/2-1 + x, (m_size.width/2)-xx + x, color);
		write_pixel(xx+m_size.width/2-1 + x, (m_size.height/2)-yy + y, color);
		
		// bottom left
		write_pixel((m_size.height/2)-yy + x, xx+m_size.width/2-1 + x, color);
		write_pixel((m_size.width/2)-xx + x, yy+m_size.height/2-1 + y , color);
		
		// top left
		write_pixel((m_size.width/2)-xx + x, (m_size.height/2)-yy + y, color);
		write_pixel((m_size.height/2)-yy + x, (m_size.width/2)-xx + y, color);
	}
}

void
SurfaceBlitter::destroy()
{
	m_sdl_surface.reset();
}

Texture*
SurfaceBlitter::to_texture()
{
	return Texture::create(m_sdl_surface.get());
}
