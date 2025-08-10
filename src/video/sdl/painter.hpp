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

#ifndef SUPERTUX_SRC_VIDEO_SDL_PAINTER_HPP
#define SUPERTUX_SRC_VIDEO_SDL_PAINTER_HPP

#include "video/painter.hpp"
#include "math/rect.hpp"
#include "texture.hpp"

struct VideoSystem;

class SDLPainter : public Painter
{
public:
	SDLPainter(VideoSystem *video);
	~SDLPainter() = default;
	
	void draw(TextureRef texture,
	          std::optional<Rectf> src,
	          std::optional<Rectf> dest,
	          Flip flip = FLIP_NONE,
			  float alpha = 1.0);
	void draw_fill_rect(Rectf dest, SDL_Color color);
	void draw_line(Vec2 l_start, Vec2 l_end);
	void flip();
	void clear();
	
	void begin_clip(Rect clip);
	void end_clip();
public:
	VideoSystem *m_video_system;
	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_sdl_renderer;
};

#endif
