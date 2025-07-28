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

#ifndef SUPERTUX_SRC_VIDEO_BGFX_BGFX_SHADER_HPP
#define SUPERTUX_SRC_VIDEO_BGFX_BGFX_SHADER_HPP

#include "config.h"

#ifdef NEOTUX_BGFX
#include <vector>
#include <string>
#include <bgfx/bgfx.h>

class BGFXProgram
{
public:
	BGFXProgram(const std::string& frag_shader, const std::string& vert_shader);
	~BGFXProgram() = default;
	
	void load_program();
	void add_shader(const std::string& filename);
private:
	bgfx::ProgramHandle m_program;
	// [0] Frag shader
	// [1] Vert shader
	std::vector<std::string> m_files;
};

#endif
#endif
