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

#include <utility>
#include "box.hpp"
#include "video/video_system.hpp"

BoxWidget::BoxWidget(const Rectf& props, float padding_w, float padding_h) :
	Rectf{props},
	m_padding_w{padding_w},
	m_padding_h{padding_h},
	m_is_hovered{false}
{
}

BoxWidget::BoxWidget(const Rectf& props, float padding) :
  BoxWidget(std::move(props), std::move(padding), std::move(padding))
{ }

const Rectf
BoxWidget::box()
{
	return *this;
}

void
BoxWidget::draw()
{
	g_video_system->get_painter()->draw_fill_rect(*this, {0, 255, 0, 150});
}

