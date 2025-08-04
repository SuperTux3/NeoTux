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

#ifndef TESTS_GAME_TESTS_GAME_TEST_HPP
#define TESTS_GAME_TESTS_GAME_TEST_HPP

#include "game.hpp"
#include <functional>
#include <unordered_map>

#define GAME_TEST_CSTOR(klassname) public: static Game* construct() { return new klassname; }
#define GAME_TEST_DECLARE(klassname, name) struct klassname : public Game { \
	GAME_TEST_CSTOR(klassname) \
	static std::string_view test_name() { return name; } \
	klassname() = default; \
	~klassname() = default; \
	virtual void run(); \
};

extern std::unordered_map<std::string, std::function<Game*()>> g_game_tests;

namespace GameTest
{
	using factory_functor = std::function<Game*()>;
	
	template <typename T>
	static void register_game_test()
	{
		std::string_view name = T::test_name();
		g_game_tests.emplace(name, &T::construct);
	}
	
	static Game*
	get_game_test(const std::string& name)
	{
		return g_game_tests.contains(name) ? g_game_tests[name]() : nullptr;
	}
};


#endif
