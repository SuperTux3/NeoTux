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

#ifndef SUPERTUX_SRC_GUI_BOX_HPP
#define SUPERTUX_SRC_GUI_BOX_HPP

#include "gui/widget.hpp"
#include "math/rect.hpp"

class BoxWidget : public Widget, public Rectf
{
public:
	BoxWidget(const Rectf& box_props, float padding = 0.0f);
	BoxWidget(const Rectf& box_props, float padding_w, float padding_h);
	BoxWidget(BoxWidget&&) = default;
	virtual ~BoxWidget() = default;
	
	static std::string_view class_id() { return "box"; }
	std::string_view obj_name() override { return "Box"; }
	
	static Widget *construct(SexpElt elt);
	
	static Rectf parse_sexp(SexpElt &elt);

	virtual void draw() override;
	virtual void update() override {};

	//virtual void setup() override {};
	//virtual void on_window_resize() override {};

	//virtual bool on_mouse_button_up(const SDL_MouseButtonEvent& button) override;
	//virtual bool on_mouse_button_down(const SDL_MouseButtonEvent& button) override;
	//virtual bool on_mouse_motion(const SDL_MouseMotionEvent& motion) override;

	/** @return Return box with applied properties, meaning the paddings are calculated, etc. */
	const Rectf box();
	inline bool hovered() { return m_is_hovered; }
  
private:
	uint8_t r, g, b;
	float m_padding_w, m_padding_h;
	bool m_is_hovered;
	
	friend class ContainerWidget;
};

#endif
