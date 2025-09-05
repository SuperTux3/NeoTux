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
	r = rand();
	g = rand();
	b = rand();
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
BoxWidget::draw(ViewContext& ctx)
{
	g_video_system->get_painter()->draw_fill_rect(*this, {r, g, b, 150});
}

Rectf
BoxWidget::parse_sexp(SexpElt &elt)
{
	SexpElt root;
	bool is_sdl = false;;
	//elt.next_inplace();
	if (!elt)
		return {};
	if (elt.is_list())
		root = elt.get_list();
	if (!root || root.get_value() != "rectf")
		return {};
	
	long a, b, c, d;

	// TODO better arg parsing
	root.next_inplace();
	try {
		root.get_int();
	}
	catch (const std::invalid_argument &e) {
		if (!(root.is_value() && root.get_value() == "sdl"))
			return {};
		is_sdl = true;
		root.next_inplace();
	}
	a = root.get_int();
	root.next_inplace();
	b = root.get_int();
	root.next_inplace();
	c = root.get_int();
	root.next_inplace();
	d = root.get_int();
	
	elt.next_inplace();
	if (is_sdl)
		return Rectf(SDL_FRect{(float)a, (float)b, (float)c, (float)d});
	return Rectf(a, b, c, d);
}

Widget*
BoxWidget::construct(SexpElt elt)
{
	Rectf props = BoxWidget::parse_sexp(elt);
	return new BoxWidget(props);
}

