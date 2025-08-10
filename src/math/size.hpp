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

#ifndef HEADER_SUPERTUX_MATH_SIZE_HPP
#define HEADER_SUPERTUX_MATH_SIZE_HPP

template <typename T>
class Size_t
{
public:
	Size_t() :
		width(), height()
	{}
	
	Size_t(T width, T height) :
		width(width),
		height(height)
	{}
	
	explicit Size_t(const Size_t<float>& rhs) :
		width(static_cast<int>(rhs.width)),
		height(static_cast<int>(rhs.height))
	{}

#define operatorX(op, type, param, param_width, param_height) \
	Size_t& operator op (type param) \
	{ \
		width op param_width; \
		height op param_height; \
		return *this; \
	}
	
	operatorX(+=, T, inc, inc, inc)
	operatorX(-=, T, dec, dec, dec)	
	operatorX(*=, T, inc, inc, inc)
	operatorX(/=, T, dec, dec, dec)
	
	operatorX(+=, const Size_t<T>&, rhs, rhs.width, rhs.height)
	operatorX(-=, const Size_t<T>&, rhs, rhs.width, rhs.height)
	operatorX(*=, const Size_t<T>&, rhs, rhs.width, rhs.height)
	operatorX(/=, const Size_t<T>&, rhs, rhs.width, rhs.height)
#undef operatorX
	
	bool is_valid() const
	{
		return width > static_cast<T>(0) && height > static_cast<T>(0);
	}
		
public:
	T width;
	T height;
};

using Size = Size_t<int>;
using Sizef = Size_t<float>;

#endif // HEADER_SUPERTUX_MATH_SIZE_HPP
