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

SDLTexture::SDLTexture(std::string filename) : 
	m_sdl_texture{nullptr, SDL_DestroyTexture}
{
	SDL_Surface *surface = create_surface(filename);
	SDLVideoSystem *video = static_cast<SDLVideoSystem*>(g_video_system.get());
	
	m_sdl_texture.reset(SDL_CreateTextureFromSurface(video->m_sdl_renderer.get(), surface));
	
	SDL_DestroySurface(surface);	
}

SDLTexture::~SDLTexture()
{}

