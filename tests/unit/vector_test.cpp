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

#include <cassert>
#include <iostream>
#include "math/vector.hpp"

int
main()
{
	Vec2 vec{15, -4};
	Vec2 vec2{10, -10};
	
	Vec2 vec_bad{0, 0};
	std::cout << "Magnitude: " << vec.magnitude() << std::endl;
	std::cout << "Distance from (10, -10): " << vec.distance(vec2) << std::endl;
	assert(vec.normalize());
	assert(!vec_bad.normalize());
	std::cout << "Normalized vector: " << vec.to_string() << std::endl;
	return 0;
}
