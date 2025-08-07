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

#include "game_object.hpp"

std::unordered_map<std::string_view, std::function<GameObject*(SexpElt)>> _registered_gobjects{};

GameObject::factory_functor
GameObject::get_gobject_cstor(const std::string &name)
{
	return _registered_gobjects.at(name);
}

GameObject*
GameObject::create(SexpElt elt)
{
	//elt.next_inplace();
	if (!elt.is_list())
		return nullptr;
	elt = elt.get_list();
	if (!elt.is_value())
		return nullptr;
	
	std::string gameobject_type = elt.get_value();
	return _registered_gobjects.at(gameobject_type)(elt.next());
}
