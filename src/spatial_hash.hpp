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

#ifndef SUPERTUX_SRC_SPATIAL_HASH_HPP
#define SUPERTUX_SRC_SPATIAL_HASH_HPP

#include <unordered_map>

template <typename T>
class SpatialHash
{
public:
	SpatialHash() :
		m_hash()
	{}
	~SpatialHash() = default;
	
	void emplace(long x, long y, T t)
	{
		if (!m_hash.contains(x))
			m_hash.emplace(x, std::unordered_map<long, T>());
		
		//if (!m_hash[x].exists(y))
		m_hash[x].emplace(y, std::move(t));
	}
	
	T& get_or_create(long x, long y, T if_not_exists)
	{
		T* t = at(x, y);
		if (t)
			return *t;
		else
		{
			emplace(x, y, std::move(if_not_exists));
			return m_hash[x][y];
		}
	}
	
	
	T* at(long x, long y)
	try {
		return &m_hash.at(x).at(y);
	}
	catch (...) {
		return nullptr;
	}
	
public:
	// HACK: Don't feel like writing a hash function for std::pair<long, long>
	std::unordered_map<long, std::unordered_map<long, T>> m_hash;
};

#endif
