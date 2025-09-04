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
	if (!m_context || !dest)
		return true;
	
	double zoomed_left = (dest->left - m_context->x) * m_context->zoom;
	double zoomed_top = (dest->top - m_context->y) * m_context->zoom;
	double zoomed_right = (dest->right - m_context->x) * m_context->zoom;
	double zoomed_bottom = (dest->bottom - m_context->y) * m_context->zoom;
	
	if (zoomed_bottom + ((m_context->height - (m_context->height * m_context->zoom)) / 2) < 0 ||
	    zoomed_top + ((m_context->height - (m_context->height * m_context->zoom)) / 2) > m_context->height ||
		zoomed_right + ((m_context->width - (m_context->width * m_context->zoom)) / 2) < 0 ||
		zoomed_left + ((m_context->width - (m_context->width * m_context->zoom)) / 2) > m_context->width)
		return false;
	
	return true;
}

void
Painter::register_camera(ViewContext *camera)
{
	m_context = camera;
}

ViewContext*
Painter::unregister_camera()
{
	ViewContext *old_camera = m_context;
	m_context = nullptr;
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
