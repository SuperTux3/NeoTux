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

#include "collision.hpp"
#include "video/video_system.hpp"
#include "collision_system.hpp"

CollisionSystem g_collision_system{};

CollisionSystem::CollisionSystem() :
	m_object_shash()
{
}

void
CollisionSystem::debug_draw()
{
	for (auto &x : m_object_shash.m_hash)
	{
		int count = x.second.size();
		
		for (auto &y : x.second)
		{
			if (y.second.size() != 0)
				g_video_system->get_painter()->draw_fill_rect(
					Rectf{(float)x.first * COL_HASH_SIZE, (float)y.first * COL_HASH_SIZE, {COL_HASH_SIZE, COL_HASH_SIZE}},
					{(unsigned char)std::min<int>(200, 0 + (5 * y.second.size())),
					 0, 255, (unsigned char)std::min<int>(200, 60 + (5 * y.second.size()))});
		}
	}
}

void
CollisionSystem::add(int x, int y, MovingObject *object)
{
	std::vector<MovingObject*> &objs = m_object_shash.get_or_create(x, y, {});
	auto it = std::find(objs.begin(), objs.end(), object);
	
	if (it == objs.end())
		objs.push_back(object);
}

void
CollisionSystem::remove(int x, int y, MovingObject *object)
{
	std::vector<MovingObject*> &objs = m_object_shash.get_or_create(x, y, {});
	auto it = std::find(objs.begin(), objs.end(), object);
	
	if (it != objs.end())
		objs.erase(it);
}

void
CollisionSystem::register_object(MovingObject *obj)
{
	//m_object_shash.emplace(0, 0, obj->
}

