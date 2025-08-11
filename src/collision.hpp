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

#ifndef SUPERTUX_SRC_COLLISION_HPP
#define SUPERTUX_SRC_COLLISION_HPP

#include <algorithm>
#include <cassert>
#include "math/rect.hpp"
#include "math/vector.hpp"

namespace Collision
{

enum SlopeInfo
{
	SLOPE_SOUTHWEST = 0,
	SLOPE_NORTHEAST,
	SLOPE_SOUTHEAST,
	SLOPE_NORTHWEST,
	DIRECTION_MASK = 0x0003,
	DEFORM_BOTTOM = 0x0010,
	DEFORM_TOP = 0x0020,
	DEFORM_LEFT = 0x0030,
	DEFORM_RIGHT = 0x0040,
	DEFORM_MASK = 0x0070
};

bool
get_line_from_slope_metas(uint16_t meta, Vec2 *lines);

static Rectf
get_chunk_collisions(Rectf box, long chunk_size)
{
	long bl = box.left / chunk_size;
	long bt = box.top / chunk_size;
	long br = box.right / chunk_size;
	long bb = box.bottom / chunk_size;
	return Rectf{
		(float)bl, (float)bt, (float)br, (float)bb
	};
}

template <typename T>
struct CollideInfo
{
	bool is_colliding() const { return left || right || top || bottom; }
	operator bool() const { return is_colliding(); }
	
	T left_constraint;
	T right_constraint;
	T top_constraint;
	T bottom_constraint;
	
	bool left;
	bool right;
	bool top;
	bool bottom;
};

template <typename T>
CollideInfo<T> aabb(Rect_t<T> r1, Rect_t<T> r2)
{
	T itop = r1.bottom - r2.top;
	T ibottom = r2.bottom - r1.top;
	T ileft = r1.right - r2.left;
	T iright = r2.right - r1.left;
	
	if (!(r1.left < r2.right &&
		  r2.left < r1.right &&
		  r1.top < r2.bottom &&
		  r2.top < r1.bottom))
	{
		return CollideInfo<T>{T(), T(), T(), T(), false, false, false, false};
	}
	
	CollideInfo<T> ret{iright, ileft, ibottom, itop};
	if (std::min(itop, ibottom) < std::min(ileft, iright))
	{
		if (itop < ibottom)
			ret.bottom = true;
		else
			ret.top = true;
	}
	else {
		if (ileft < iright)
			ret.right = true;
		else
			ret.left = true;
	}
	return ret;
}

template <typename T>
std::optional<Vec2_t<T>> line_line(Vec2_t<T> l1_beg, Vec2_t<T> l1_end,
                                   Vec2_t<T> l2_beg, Vec2_t<T> l2_end)
{
	const T dnom = ((l2_end.y - l2_beg.y) * (l1_end.x - l1_beg.x) - (l2_end.x - l2_beg.x) * (l1_end.y - l1_beg.y)); 
	T dist1 = 
		((l2_end.x - l2_beg.x) * (l1_beg.y - l2_beg.y) - (l2_end.y - l2_beg.y) * (l1_beg.x - l2_beg.x)) /
		dnom;
	T dist2 = 
		((l1_end.x - l1_beg.x) * (l1_beg.y - l2_beg.y) - (l1_end.y - l1_beg.y) * (l1_beg.x - l2_beg.x)) /
		dnom;
	
	// Lines are between 0 and 1
	if (dist1 == std::clamp<T>(dist1, 0, 1) &&
	    dist2 == std::clamp<T>(dist2, 0, 1))
	{
		T intersect_x = l1_beg.x + (dist1 * (l1_end.x - l1_beg.x));
		T intersect_y = l1_beg.y + (dist1 * (l1_end.y - l1_beg.y));
		
		return Vec2_t<T>( intersect_x, intersect_y );
	}
	return std::nullopt;
}

template <typename T>
using col_side_t = std::pair<RectSide, Vec2_t<T>>;

// Sometimes there are reports of 3 collisions between a line. To be safe, I set this to 4 (all sides), though
//  I don't believe it is possible...
constexpr size_t COL_SIZE_MAX = 4;

template <typename T>
size_t line_rect(col_side_t<T> col_list[], Vec2_t<T> l_beg, Vec2_t<T> l_end, Rect_t<T> rect)
{
	size_t cols = 0;
	auto left = line_line(l_beg, l_end, { rect.left, rect.top }, { rect.left, rect.bottom });
	auto right = line_line(l_beg, l_end, { rect.right, rect.top }, { rect.right, rect.bottom });
	auto top = line_line(l_beg, l_end, { rect.left, rect.top }, { rect.right, rect.top });
	auto bottom = line_line(l_beg, l_end, { rect.left, rect.bottom }, { rect.right, rect.bottom });
	
	if (left)
		col_list[cols++] = {RECT_LEFT, *left};
	if (right)
		col_list[cols++] = {RECT_RIGHT, *right};
	if (top)
		col_list[cols++] = {RECT_TOP, *top};
	if (bottom)
		col_list[cols++] = {RECT_BOTTOM, *bottom};
	
	assert(cols < COL_SIZE_MAX+1);
	
	return cols;
}

}; // namespace Collision

#endif
