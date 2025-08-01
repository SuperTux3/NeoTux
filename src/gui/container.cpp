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
#include "util/logger.hpp"
#include <format>

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
	const Rectf& that = box();
	float height = that.get_height() / m_widgets.size();
	float y = 0;
	for (auto&& widget : m_widgets)
	{
		const Rectf& other = widget->box();
		widget->top = y;
		widget->bottom = y + height;
		widget->left = that.left;
		widget->right = that.right;
		// TODO: other->set_{width,height}(m_spacing)
		y += height;
	}
}

void
ContainerWidget::add(BoxWidget* box)
{
	m_widgets.emplace_back(box);
}

Widget*
ContainerWidget::construct(SexpElt elt)
{
	Rectf props = BoxWidget::parse_sexp(elt);
	ContainerWidget *widget = new ContainerWidget(props);
	do
	{
		BoxWidget *box_widget = dynamic_cast<BoxWidget*>(Widget::create(elt));
		Logger::debug(std::format("Pushing {} widget: {}", box_widget->obj_name(), static_cast<void*>(box_widget)));
		if (box_widget)
			widget->add(box_widget);
	} while (elt.next_inplace());
	return widget;
}
