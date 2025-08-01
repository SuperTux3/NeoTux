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

#include "container.hpp"

ContainerWidget::ContainerWidget(const Rectf& props) :
  BoxWidget{props},
  m_widgets(),
  m_spacing(),
  m_is_horizontal(false)
{ }

void
ContainerWidget::draw()
{
	float pos = 0.f;
	for (const auto& widget : m_widgets)
	{
		const Rectf& r = widget->box();
		widget->draw();
	}
}

void
ContainerWidget::update()
{
	m_spacing = static_cast<float>(m_widgets.size());
	for (auto&& widget : m_widgets)
	{
		const Rectf& other = widget->box();
		// TODO: other->set_{width,height}(m_spacing)
	}
}

void
ContainerWidget::add(BoxWidget* box)
{
	m_widgets.emplace_back(box);
}

