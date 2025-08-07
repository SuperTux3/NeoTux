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

#ifndef SUPERTUX_SRC_OBJECT_MOVING_SPRITE_HPP
#define SUPERTUX_SRC_OBJECT_MOVING_SPRITE_HPP

#include "timer.hpp"
#include "util/sexp.hpp"
#include "object/moving_object.hpp"
#include <cstring>
#include <unordered_map>

struct SpriteAction
{
	SpriteAction(double fps,
	             int loops,
	             std::vector<std::string> images,
				 int *_hitboxes) :
		fps(std::move(fps)),
		loops(loops),
		images(std::move(images)),
		hitboxes()
	{
		std::memcpy(hitboxes, _hitboxes, sizeof(decltype(*hitboxes))*4);
	}
	
	const std::string&
	get_image(Timer &timer)
	{
		return images[timer.get_iterations() % (images.size())];
	}
	
	double fps;
	int loops;
	std::vector<std::string> images;
	int hitboxes[4];
};

class MovingSprite : public MovingObject
{
public:
	MovingSprite(std::string sprite_dir, std::string_view name);
	virtual ~MovingSprite() = default;
	
	virtual void update(Tilemap &tilemap) override;
	void draw();
	
	void set_action(const std::string &action);

protected:
	void parse_sprite();
	
	std::string m_parent_dir;
	std::string m_filename;
	SpriteAction *m_action;
	std::unordered_map<std::string, std::unique_ptr<SpriteAction>> m_actions;
	SexpParser m_parser;
	Timer m_action_timer;
	Flip m_flip;
};

#endif
