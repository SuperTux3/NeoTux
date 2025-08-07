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
	m_filename(std::move(sprite_file)),
	m_action(nullptr),
	m_action_timer(0, 0)
{
	m_parent_dir = FS::parent_dir(m_filename);
	parse_sprite();
}

void
MovingSprite::parse_sprite()
{
	SexpElt root, elt, aelt;
	
	root = m_parser.read_file(FS::path(m_filename));
	if (!root.is_list())
		return;
	
	root = root.get_list();
	
	if (root.get_value() == "supertux-sprite")
		Logger::debug("Claims to be a supertux sprite .... ^_^");
	
	while (root.next_inplace())
	{
		std::string name;
		double fps;
		std::vector<std::string> images;
		int hitboxes[4];
		SpriteAction *sprite_action;
		
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
		
		sprite_action = new SpriteAction(fps, images, hitboxes);
		m_actions.emplace(name, std::unique_ptr<SpriteAction>(sprite_action));
	}
}

void
MovingSprite::set_action(const std::string &action)
{
	m_action = m_actions.at(action).get();
	
	TextureRef tex = g_texture_manager.load(m_parent_dir + "/" + m_action->get_image(m_action_timer));
	
	m_rect.right = m_rect.left + tex->get_size().width;
	m_rect.bottom = m_rect.top + tex->get_size().height;
	m_colbox.left = m_action->hitboxes[0];
	m_colbox.top = m_action->hitboxes[1];
	m_colbox.right = m_action->hitboxes[2];
	m_colbox.bottom = m_action->hitboxes[3];
	
	m_action_timer.set_duration((1.0/m_action->fps)*1000);
	m_action_timer.set_loops(-1);
}

void
MovingSprite::update(Tilemap &tilemap)
{
	MovingObject::update(tilemap);
	if (m_action)
		m_action_timer.tick();
}

void
MovingSprite::draw()
{
	if (!m_action)
		return;
	TextureRef tex = g_texture_manager.load(m_parent_dir + "/" + m_action->get_image(m_action_timer));
	g_video_system->get_painter()->draw(tex, std::nullopt, m_rect);
	MovingObject::draw();
}
