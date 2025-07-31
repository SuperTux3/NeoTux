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

#include <SDL3/SDL_render.h>
#include "camera.hpp"
#include "painter.hpp"
#include "video/sdl/sdl_video_system.hpp"
#include "video/sdl/texture.hpp"

SDLPainter::SDLPainter(VideoSystem *video) :
	m_video_system(video),
	m_sdl_renderer(SDL_CreateRenderer(video->window.m_sdl_window.get(), NULL),
		&SDL_DestroyRenderer)
{}

void
SDLPainter::draw(TextureRef texture, std::optional<Rectf> src, std::optional<Rectf> dest)
{
	SDLTexture *sdltex = static_cast<SDLTexture*>(texture);
	
	if (!in_camera_bounds(dest))
		return;
	
	SDL_FRect src_sdl;
	if (src)
		src_sdl = src->to_sdl_frect();
	SDL_FRect dest_sdl;
	if (dest)
		dest_sdl = dest->to_sdl_frect();
	if (m_camera)
	{
		dest_sdl.x -= m_camera->x;
		dest_sdl.y -= m_camera->y;
	}
	//SDLVideoSystem *video = static_cast<SDLVideoSystem*>(m_
	SDL_RenderTexture(m_sdl_renderer.get(), sdltex->get_sdl_texture(),
		src ? &src_sdl : NULL, dest ? &dest_sdl : NULL);
	bump_draw_count();
}

void
SDLPainter::flip()
{
	SDL_RenderPresent(m_sdl_renderer.get());
	reset_draw_count();
}
