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

#include "bgfx_program.hpp"

BGFXProgram::BGFXProgram(const std::string& frag_shader, const std::string& vert_shader) :
	m_files{frag_shader, vert_shader}
{
	load_program();
}

void
BGFXProgram::load_program()
{
	const std::string& frag_shader = m_files[0];
	const std::string& vert_shader = m_files[1];
	m_program = bgfx::createProgram();
}

void
BGFXProgram::add_shader(const std::string& filename)
{
	
}
