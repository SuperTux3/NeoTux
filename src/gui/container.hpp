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

#ifndef SUPERTUX_SRC_GUI_CONTAINER_HPP
#define SUPERTUX_SRC_GUI_CONTAINER_HPP

#include <vector>
#include <memory>
#include "gui/box.hpp"

class ContainerWidget : public BoxWidget
{
public:
	ContainerWidget(const Rectf &props, bool is_horizontal = false);
	~ContainerWidget() = default;

	static std::string_view class_id() { return "container"; }
	std::string_view obj_name() override { return "Container"; }
	
	static Widget* construct(SexpElt elt);
	
	
	void add(BoxWidget *box);
	
	virtual void draw() override;
	virtual void update() override;
	
private:
	std::vector<std::unique_ptr<BoxWidget>> m_widgets;
	float m_spacing;
	bool m_is_horizontal;
};

#endif
