//  SuperTux
//  Copyright (C) 2006 Matthias Braun <matze@braunis.de>
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

#include "video_system.hpp"

std::unique_ptr<VideoSystem> g_video_system;

VideoSystem::Backend VideoSystem::get_video_system(const std::string& video)
{
	if (video == "opengl")
		return VideoSystem::VIDEO_BGFX_OPENGL;
	else if (video == "opengles")
		return VideoSystem::VIDEO_BGFX_OPENGLES;
	else if (video == "vulkan")
		return VideoSystem::VIDEO_BGFX_VULKAN;
	else if (video == "metal")
		return VideoSystem::VIDEO_BGFX_METAL;
	else if (video == "sdl")
		return VideoSystem::VIDEO_SDL;
	else if (video == "null")
		return VideoSystem::VIDEO_NULL;
	else
		return VideoSystem::VIDEO_AUTO;
}
