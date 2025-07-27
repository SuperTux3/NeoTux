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

#include "painter.hpp"
#include "camera.hpp"

bool
Painter::in_camera_bounds(std::optional<Rectf> dest)
{
	if (!m_camera || !dest)
		return true;
	
	if (dest->bottom - m_camera->y < 0 ||
	    dest->top - m_camera->y > m_camera->height ||
		dest->right - m_camera->x < 0 ||
		dest->left - m_camera->x > m_camera->width)
		return false;
	
	return true;
}

void
Painter::register_camera(Camera *camera)
{
	m_camera = camera;
}

void
Painter::unregister_camera()
{
	m_camera = nullptr;
}

