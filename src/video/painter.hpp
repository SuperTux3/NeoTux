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

#ifndef SUPERTUX_SRC_VIDEO_PAINTER_HPP
#define SUPERTUX_SRC_VIDEO_PAINTER_HPP

#include <SDL3/SDL_pixels.h>
#include <optional>
//#include "camera.hpp"
#include "math/rect.hpp"
#include "texture.hpp"

enum
{
	FLIP_NONE       = 0,
	FLIP_HORIZONTAL = 1,
	FLIP_VERTICAL   = 2,
};
using Flip = uint8_t;

class Camera;

class Painter
{
public:
	Painter() = default;
	virtual ~Painter() = default;
	
	virtual void draw(TextureRef texture,
	                  std::optional<Rectf> src,
	                  std::optional<Rectf> dest,
	                  Flip flip = FLIP_NONE) = 0;
	virtual void draw_fill_rect(Rectf dest, SDL_Color color) = 0;
	virtual void flip() = 0;
	virtual void clear() = 0;
	
	bool in_camera_bounds(std::optional<Rectf> dest);
	
	virtual void begin_clip(Rect clip);
	virtual void end_clip();
	
	void register_camera(Camera *camera);
	void unregister_camera();
	
//#ifndef NDEBUG
	inline unsigned long get_draw_count() const { return m_draw_count; }
	inline unsigned long get_total_draw_count() const { return m_total_draw_count; }
	inline void bump_draw_count() { ++m_draw_count; ++m_total_draw_count; }
	inline void reset_draw_count() { m_draw_count = 0; }
	unsigned long m_draw_count = 0;
	unsigned long m_total_draw_count = 0;
//#endif

protected:
	Camera *m_camera;
	bool m_do_clip;
	Rect m_clip;
};

#endif
