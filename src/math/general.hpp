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

	
#ifndef SUPERTUX_SRC_MATH_GENERAL_HPP
#define SUPERTUX_SRC_MATH_GENERAL_HPP

namespace Math
{

// Convert `X == std::clamp(X, i, j)` to `between(X, i, j)`
template <typename T>
bool between(T i, T beg, T end)
{
	return i >= beg && i <= end;
}

}

#endif
