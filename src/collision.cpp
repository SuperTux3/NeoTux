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

#include "collision.hpp"
#include "util/logger.hpp"

namespace Collision
{

Vec2
get_normal_from_slope_metas(uint16_t meta)
{
	uint16_t deform = (uint16_t)((float)meta / 16.f);
	Vec2 line;
	switch (deform)
	{
	case 0:
		//line = { 0.f, -1.f };
		line = { 1, 1 };
		break;
	case 1:	
	case 2:
		line = { 1, .5 };
		break;
	case 3:
	case 4:
		line = { 0.5, 1 };
		break;
	default:
		Logger::warn("Invalid slope deformation!");
		break;
	}
	
	uint8_t dir_mask = meta & DIRECTION_MASK;
	if (dir_mask > 1)
		line.y = -line.y;
	
	return line;
}

bool
get_line_from_slope_metas(uint16_t meta, Vec2 *lines)
{
	uint16_t deform = (uint16_t)((float)meta / 16.f);
	switch (deform)
	{
	case 0:
		lines[0] = {  0.f,  0.f };
		lines[1] = {  1.f,  1.f };
		break;
	case 1:
		lines[0] = {  0.f, 0.5f };
		lines[1] = {  1.f,  1.f };
		break;
	case 2:
		lines[0] = {  0.f,  0.f };
		lines[1] = {  1.f, 0.5f };
		break;
	case 3:
		lines[0] = {  0.f,  0.f };
		lines[1] = { 0.5f,  1.f };
		break;
	case 4:
		lines[0] = { 0.5f,  0.f };
		lines[1] = {  1.f,  1.f };
		break;
	default:
		Logger::warn("Invalid slope deformation!");
		break;
	}
	
	uint8_t dir_mask = meta & DIRECTION_MASK;
	
	if (dir_mask > 1)
	{
		Vec2::type tmp = lines[1].y;
		lines[1].y = lines[0].y;
		lines[0].y = tmp;
	}
	return true;
}

} // namespace Collision
