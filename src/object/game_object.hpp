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

#ifndef SUPERTUX_SRC_OBJECT_GAME_OBJECT_HPP
#define SUPERTUX_SRC_OBJECT_GAME_OBJECT_HPP

#include <functional>
#include <string>
#include "util/sexp.hpp"

struct GameObject;
extern std::unordered_map<std::string_view, std::function<GameObject*(SexpElt)>> _registered_gobjects;

class Tilemap;

class GameObject
{
public:
	using factory_functor = std::function<GameObject*(SexpElt)>;

	GameObject(std::string_view name) :
		m_name(name),
		m_wants_destruction(false)
	{};
	virtual ~GameObject() = default;
	
	template <typename T>
	static void register_object()
	{
		std::string_view id = T::class_id();
		_registered_gobjects.emplace(id, &T::construct);
	}
	static factory_functor get_gobject_cstor(const std::string &name);
	static GameObject* create(SexpElt elt);
	
	void mark_for_destruction() { m_wants_destruction = true; }
	bool destroy_me() const { return m_wants_destruction; }
	
	virtual void update(Tilemap &tilemap) = 0;
	virtual void draw() = 0;
	virtual std::string_view get_name() const { return m_name; }
private:
	std::string_view m_name;
	bool m_wants_destruction;
};

#endif
