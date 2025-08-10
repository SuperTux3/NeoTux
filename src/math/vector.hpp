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

#ifndef SUPERTUX_SRC_MATH_VECTOR_HPP
#define SUPERTUX_SRC_MATH_VECTOR_HPP

#include <cmath>
#include <tgmath.h>
#include <string>

template <typename T>
class Vec2_t
{
public:
	T x;
	T y;
	
	Vec2_t(T val = T()) :
		x{val},
		y{val}
	{}
	Vec2_t(T x, T y) :
		x{x},
		y{y}
	{}
	~Vec2_t() = default;
	
	T magnitude()
	{
		return std::sqrt(std::pow<T>(x, 2) + std::pow<T>(y, 2));
	}
	
	bool normalize()
	{
		T mag = magnitude();
		if (mag <= 0)
			return false;
		*this /= mag;
		return true;
	}
	
	Vec2_t<T> get_normal()
	{
		Vec2_t<T> tmp = *this;
		tmp.normalize();
		return tmp;
	}
	
	
	T distance(Vec2_t<T> other)
	{
		return std::sqrt(
			std::pow<T>(x - other.x, 2) + std::pow<T>(y - other.y, 2));
	}

	bool operator<(Vec2_t<T> other)
	{
		return x < other.x && y < other.y;
	}
#define operatorX(op, opinc) \
	Vec2_t<T> operator op (Vec2_t<T> other) \
	{ \
		return { x op other.x, y op other.y }; \
	} \
	void operator opinc (Vec2_t<T> other) \
	{ \
		x opinc other.x; \
		y opinc other.y; \
	}
	
	operatorX(+, +=)
	operatorX(-, -=)
	operatorX(*, *=)
	operatorX(/, /=)
#undef operatorX
	
	std::string
	to_string()
	{
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}
};

using Vec2 = Vec2_t<double>;
using IVec2 = Vec2_t<long>;
using FVec2 = Vec2_t<float>;

#endif
