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

#include "input_manager.hpp"
#include <cassert>

int
main()
{
	Binding b1(Binding::Key('w') | Binding::Gamepad(4));
	assert(b1.is_keyboard() == true);
	assert(b1.is_gamepad() == true);
	assert(b1.alt() == false);
	assert(b1.get_gamepad_button() == 4);
	assert(b1.get_key() == 'w');
	
	Binding b2(Binding::Key(Binding::CTRL | Binding::ALT | 'c'));
	assert(b2.is_gamepad() == false);
	assert(b2.is_keyboard() == true);
	assert(b2.get_key() == 'c');
	assert(b2.ctrl() == true);
	assert(b2.alt() == true);
	
	return 0;
}
