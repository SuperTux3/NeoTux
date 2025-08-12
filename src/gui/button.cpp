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

#include "button.hpp"
#include "util/logger.hpp"
#include "video/font_manager.hpp"
#include "video/video_system.hpp"
#include <format>

ButtonWidget::ButtonWidget(const Rectf& props, BoxWidget *widget) :
	BoxWidget(props),
	m_widget(widget)
{}

void
ButtonWidget::draw()
{
#ifndef NDEBUG
	if (!m_widget)
		Logger::warn("No widget in button!");
#endif
	if (m_widget)
	{
		m_widget->draw();
	}
}

void
ButtonWidget::update()
{
	if (m_widget)
	{
		m_widget->left = left;
		m_widget->top = top;
		m_widget->right = right;
		m_widget->bottom = bottom;
		std::cout << *this << std::endl;
		m_widget->update();
	}
}

Widget*
ButtonWidget::construct(SexpElt elt)
{
	SexpElt telt;
	Rectf props = BoxWidget::parse_sexp(elt);
	
	Widget *widget = nullptr;
	if (elt.is_list())
		widget = Widget::create(elt);
		
	return new ButtonWidget(props, reinterpret_cast<ButtonWidget*>(widget));
}
