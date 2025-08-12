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

#include "snowball.hpp"
#include "collision.hpp"
#include "collision_system.hpp"
#include "object/player.hpp"
#include "object/retro/retro_player.hpp"
#include "video/painter.hpp"

Snowball::Snowball() :
	MovingSprite("images/creatures/snowball/snowball.sprite", "snowball"),
	m_dir(false),
	m_squished(false),
	m_squish_timer(3000.0, 1)
{
	enable_gravity();
	set_collidable(true);
	
	set_action("left");
}

GameObject*
Snowball::construct(SexpElt elt)
{
	Snowball *that = new Snowball();
	that->parse_sexp(elt);
	return that;
}

void
Snowball::on_collision(Sector &sector, MovingObject &obj, Collision::CollideInfo<float> collide)
{
	Player *player = dynamic_cast<Player*>(&obj);
	if (!player || m_squished)
		return;
	if (collide.top)
	{
		g_mixer.play_sound("sounds/retro/squish.wav");
		m_squish_timer.reset();
		m_squished = true;
		set_action("squished-left");
		RetroPlayer *retroplayer = dynamic_cast<RetroPlayer*>(&obj);
		if (retroplayer)
			retroplayer->m_jumped = false;
		obj.set_y_vel(0.4);
		return;
	}
	if (collide.left || collide.right)
	{
		player->damage();
	}
}

void
Snowball::update(Sector &sector, Tilemap &tilemap)
{
	if (!m_squished)
		move((m_dir ? 1.0 : -1.0) * 80 * g_dtime, 0);
	MovingSprite::update(sector, tilemap);
	if (m_squished)
	{
		m_alpha = 1.0 - (m_squish_timer.get_percentage() / 100.0);
		if (m_squish_timer.tick())
			mark_for_destruction();
		return;
	}
	
	
	for (auto &colinfo : m_colinfo)
	{
		if (colinfo.left && m_dir == false)
		{
			m_dir = true;
			m_flip = FLIP_HORIZONTAL;
		}
		if (colinfo.right && m_dir == true)
		{
			m_dir = false;
			m_flip = 0;
		}
	}
}

void
Snowball::draw()
{
	//MovingObject::draw();
	MovingSprite::draw();
	//TextureRef tex = g_texture_manager.load("images/creatures/tux/big/stand-0.png");
	//g_video_system->get_painter()->draw(tex, std::nullopt, m_rect);
}

