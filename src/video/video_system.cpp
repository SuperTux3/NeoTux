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
#include "math/size.hpp"
#include <SDL3/SDL_video.h>

std::unique_ptr<VideoSystem> g_video_system;

const char *
VideoSystem::video_system_to_str(VideoSystem::Backend backend)
{
	switch (backend)
	{
	case VideoSystem::VIDEO_BGFX_OPENGL:
		return "OpenGL (BGFX)";
	case VideoSystem::VIDEO_BGFX_OPENGLES:
		return "OpenGLES (BGFX)";
	case VideoSystem::VIDEO_BGFX_VULKAN:
		return "Vulkan (BGFX)";
	case VideoSystem::VIDEO_BGFX_METAL:
		return "Metal (BGFX)";
	case VideoSystem::VIDEO_SDL:
		return "SDL3";
	case VideoSystem::VIDEO_NULL:
		return "null";
	}
	return "???";
}

VideoSystem::Backend
VideoSystem::str_to_video_system(const std::string &video)
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

void
VideoSystem::create_window(int flags)
{
	window.create_window(flags | SDL_WINDOW_RESIZABLE, "SuperTux");
}

Painter*
VideoSystem::get_painter() const
{
	return m_painter.get();
}

Size
VideoSystem::get_window_size() const
{
	Size size = { };
	SDL_Window *win = window.m_sdl_window.get();
	SDL_GetWindowSize(win, &size.width, &size.height);
	
	return size;
}

