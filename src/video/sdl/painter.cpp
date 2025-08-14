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
#include "config.h"
#include "camera.hpp"
#include "painter.hpp"
#include "video/sdl/sdl_video_system.hpp"
#include "video/sdl/texture.hpp"

SDLPainter::SDLPainter(VideoSystem *video) :
	Painter(),
	m_video_system(video),
	m_sdl_renderer(SDL_CreateRenderer(video->window.m_sdl_window.get(), NULL),
		&SDL_DestroyRenderer)
{
	SDL_SetRenderDrawBlendMode(m_sdl_renderer.get(), SDL_BLENDMODE_BLEND);
#ifdef NEOTUX_PSP
	SDL_SetRenderVSync(m_sdl_renderer.get(), 1);
#endif
}

void
SDLPainter::draw(TextureRef texture,
                 std::optional<Rectf> src,
                 std::optional<Rectf> dest,
                 Flip flip,
				 float alpha)
{
	SDLTexture *sdltex = static_cast<SDLTexture*>(texture);
	SDL_Texture *tex = sdltex->get_sdl_texture();
	float last_alpha;
	
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
	if (m_do_clip)
	{
		dest_sdl.x -= m_clip.left;
		dest_sdl.y -= m_clip.top;
	}
	
	if (alpha != 1.0)
	{
		SDL_GetTextureAlphaModFloat(tex, &last_alpha);
		SDL_SetTextureAlphaModFloat(tex, alpha);
	}
	if (flip != FLIP_NONE)
	{
		SDL_RenderTextureRotated(m_sdl_renderer.get(), tex,
		    src ? &src_sdl : NULL, dest ? &dest_sdl : NULL,
		    0.0, NULL, (SDL_FlipMode)flip);
	}
	else {
		SDL_RenderTexture(m_sdl_renderer.get(), tex,
		    src ? &src_sdl : NULL, dest ? &dest_sdl : NULL);
	}
	if (alpha != 1.0)
		SDL_SetTextureAlphaModFloat(tex, last_alpha);
	bump_draw_count();
}

void
SDLPainter::draw_fill_rect(Rectf dest, SDL_Color color)
{
	if (!in_camera_bounds(dest))
		return;
	
	SDL_FRect dest_sdl = dest.to_sdl_frect();
	if (m_camera)
	{
		dest_sdl.x -= m_camera->x;
		dest_sdl.y -= m_camera->y;
	}
	if (m_do_clip)
	{
		dest_sdl.x -= m_clip.left;
		dest_sdl.y -= m_clip.top;
	}
	
	SDL_SetRenderDrawColor(m_sdl_renderer.get(), color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(m_sdl_renderer.get(), &dest_sdl);
	bump_draw_count();
}

void
SDLPainter::draw_line(Vec2 l_start, Vec2 l_end, SDL_Color color)
{
	if (m_camera)
	{
		l_start.x -= m_camera->x;
		l_start.y -= m_camera->y;
		
		l_end.x -= m_camera->x;
		l_end.y -= m_camera->y;
	}
	if (m_do_clip)
	{
		l_start.x -= m_clip.left;
		l_start.y -= m_clip.top;
		
		l_end.x -= m_clip.left;
		l_end.y -= m_clip.top;
	}
	SDL_SetRenderDrawColor(m_sdl_renderer.get(), color.r, color.g, color.b, color.a);
	SDL_RenderLine(m_sdl_renderer.get(),
		l_start.x, l_start.y,
		l_end.x, l_end.y);
	bump_draw_count();
}

void
SDLPainter::begin_clip(Rect clip)
{
	Painter::begin_clip(std::move(clip));
	const SDL_Rect sdl_clip = m_clip.to_sdl_rect();
	
	SDL_SetRenderViewport(m_sdl_renderer.get(), &sdl_clip);
}

void
SDLPainter::end_clip()
{
	SDL_SetRenderViewport(m_sdl_renderer.get(), nullptr);
	Painter::end_clip();
}


void
SDLPainter::flip()
{
	SDL_RenderPresent(m_sdl_renderer.get());
	reset_draw_count();
}

void
SDLPainter::clear()
{
	SDL_SetRenderDrawColor(m_sdl_renderer.get(), 0, 0, 40, 255);
	SDL_RenderClear(m_sdl_renderer.get());
}

