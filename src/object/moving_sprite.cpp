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

#include "moving_sprite.hpp"
#include "object/moving_object.hpp"
#include "util/filesystem.hpp"
#include "util/logger.hpp"
#include "video/texture_manager.hpp"
#include "video/video_system.hpp"

MovingSprite::MovingSprite(std::string sprite_file, std::string_view name) :
	MovingObject({0, 0, 0, 0}, {0, 0, 0, 0}, std::move(name)),
	m_filename(std::move(sprite_file))
{
	parse_sprite();
}

void
MovingSprite::parse_sprite()
{
	SexpElt root, elt, aelt;
	std::string name;
	double fps;
	std::vector<std::string> images;
	int hitboxes[4];
	
	root = m_parser.read_file(FS::path(m_filename));
	if (!root.is_list())
		return;
	
	root = root.get_list();
	
	if (root.get_value() == "supertux-sprite")
		Logger::debug("Claims to be a supertux sprite .... ^_^");
	
	while (root.next_inplace())
	{
		if (!root.is_list())
			continue;
		elt = root.get_list();
		if (!elt.is_value())
			continue;
		
		aelt = elt.find_car("name");
		if (aelt)
			name = aelt.next().get_value();
		
		aelt = elt.find_car("fps");
		if (aelt)
			fps = aelt.next().get_number();
		
		aelt = elt.find_car("images");
		if (aelt)
		{
			while (aelt.next_inplace())
				images.push_back(aelt.get_value());
		}
		
		aelt = elt.find_car("hitbox");
		if (aelt)
		{
			int i = 0;
			for (i = 0; i < 4 && aelt.next_inplace(); ++i)
			{
				hitboxes[i] = aelt.get_int();
			}
			if (i != 4)
				Logger::warn("A sprite hitbox is missing values! Expect bugs...");
		}
		
		m_actions.emplace(name, SpriteAction{fps, images, hitboxes});
	}
}

void
MovingSprite::update(Tilemap &tilemap)
{
	MovingObject::update(tilemap);
}

void
MovingSprite::draw()
{
	//MovingObject::draw();
	TextureRef tex = g_texture_manager.load("images/creatures/tux/big/stand-0.png");
	g_video_system->get_painter()->draw(tex, std::nullopt, m_rect);
}
