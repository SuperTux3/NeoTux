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

#include "text.hpp"
#include "util/logger.hpp"
#include "video/font_manager.hpp"
#include "video/video_system.hpp"
#include <format>

TextWidget::TextWidget(const Rectf& props, std::string message, unsigned font_size, SDL_Color color) :
  BoxWidget(props),
  m_message(std::move(message)),
  m_font_size(font_size),
  m_color(color)
{}

void
TextWidget::draw()
{
	TextureRef tex = g_font_manager.load(SUPERTUX_MEDIUM, m_font_size, m_message, m_color);
	g_video_system->get_painter()->draw(tex,
		tex->get_size(),
		box().from_alignment(0.5, 0.5, (float)tex->get_size().width, (float)tex->get_size().height));
}

void
TextWidget::update()
{
}

void
TextWidget::add(BoxWidget* box)
{
}

Widget*
TextWidget::construct(SexpElt elt)
{
	SexpElt telt;
	SDL_Color color{255, 255, 255, 255};
	unsigned font_size = 32;
	std::string message;
	Rectf props = BoxWidget::parse_sexp(elt);
	do {
		telt = elt.get_list();
		if (!telt.is_value())
			continue;
		if (telt.get_value() == "font-size")
			font_size = telt.next().get_int();
		else if (telt.get_value() == "color")
			Widget::parse_color(color, telt.next());
		else
			break;
	}
	while (elt.next_inplace() && elt.is_list());
	
	if (elt.is_value())
		message = elt.get_value();
	Logger::debug(std::format("Parsed message: \"{}\", [fs: {}]", message, font_size));
		
	TextWidget *widget = new TextWidget(props, message, font_size, color);
	return widget;
}
