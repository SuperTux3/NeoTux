//  SuperTux
//  Copyright (C) 2009 Ingo Ruhnke <grumbel@gmail.com>a
//                2025 Hyland B. <me@ow.swag.toys>
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

#ifndef HEADER_SUPERTUX_MATH_RECT_HPP
#define HEADER_SUPERTUX_MATH_RECT_HPP

#include <SDL3/SDL_rect.h>
#include "size.hpp"

template <typename T>
class Rect_t
{
public:
	Rect_t() :
		left(), top(), right(), bottom()
	{}
		
	static Rect_t<T> from_center(T center_x, T center_y, T width, T height)
	{
		return {
			center_x - width / 2,
			center_y - height / 2,
			center_x + width / 2,
			center_y + height / 2
		};
	}
	
	Rect_t(T left, T top, T right, T bottom) :
		left(left), top(top), right(right), bottom(bottom)
	{}
	
	Rect_t(T left, T top, const Size_t<T>& size) :
		left(left),
		top(top),
		right(left + size.width),
		bottom(top + size.height)
	{}
	
	Rect_t(SDL_FRect frect) :
		left(frect.x),
		top(frect.y),
		right(frect.x + frect.w),
		bottom(frect.y + frect.h)
	{}
	
	T get_width() const
	{
		return right - left;
	}
	T get_height() const
	{
		return bottom - top;
	}
	
	SDL_FRect to_sdl_frect()
	{
		return { left, top, get_width(), get_height() };
	}
	
	SDL_Rect to_sdl_rect()
	{
		return { left, top, get_width(), get_height() };
	}
	
public:
	T left;
	T top;
	T right;
	T bottom;
};

using Rect = Rect_t<int>;
using Rectf = Rect_t<float>;

#endif // HEADER_SUPERTUX_MATH_RECT_HPP
