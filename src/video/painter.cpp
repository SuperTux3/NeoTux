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
	
	double zoomed_left = (dest->left - m_camera->x) * m_camera->zoom;
	double zoomed_top = (dest->top - m_camera->y) * m_camera->zoom;
	double zoomed_right = (dest->right - m_camera->x) * m_camera->zoom;
	double zoomed_bottom = (dest->bottom - m_camera->y) * m_camera->zoom;
	
	if (zoomed_bottom + ((m_camera->height - (m_camera->height * m_camera->zoom)) / 2) < 0 ||
	    zoomed_top + ((m_camera->height - (m_camera->height * m_camera->zoom)) / 2) > m_camera->height ||
		zoomed_right + ((m_camera->width - (m_camera->width * m_camera->zoom)) / 2) < 0 ||
		zoomed_left + ((m_camera->width - (m_camera->width * m_camera->zoom)) / 2) > m_camera->width)
		return false;
	
	return true;
}

void
Painter::register_camera(Camera *camera)
{
	m_camera = camera;
}

Camera*
Painter::unregister_camera()
{
	Camera *old_camera = m_camera;
	m_camera = nullptr;
	return old_camera;
}

void
Painter::begin_clip(Rect clip)
{
	m_do_clip = true;
	m_clip = std::move(clip);
}

void
Painter::end_clip()
{
	m_do_clip = false;
}
