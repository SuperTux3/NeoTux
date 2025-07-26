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
#include "video/video_system.hpp"
#include "sdl_video_system.hpp"
#include "video/sdl/painter.hpp"

SDLTexture::SDLTexture(std::string filename) :
	SDLTexture{create_surface(filename), true}
{}

SDLTexture::SDLTexture(SDL_Surface * const surface, bool destroy_surface/* = false */) :
	m_sdl_texture{nullptr, SDL_DestroyTexture}
{
	SDLVideoSystem *video = static_cast<SDLVideoSystem*>(g_video_system.get());
	SDLPainter *painter = static_cast<SDLPainter*>(g_video_system->get_painter());
	SDL_Texture *texture = SDL_CreateTextureFromSurface(painter->m_sdl_renderer.get(), surface);
	size.width = surface->w;
	size.height = surface->h;
	m_sdl_texture.reset(texture);
	
	if (destroy_surface)
		SDL_DestroySurface(surface);
}

