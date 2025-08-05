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
#include <iostream>
#include "math/rect.hpp"

namespace Collision
{

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

};

#endif
