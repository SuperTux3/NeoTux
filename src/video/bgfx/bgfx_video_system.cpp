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

#include "bgfx_video_system.hpp"
#include "bgfx/bgfx.h"
#include "bgfx/defines.h"
#include "bgfx/platform.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_video.h>
#include "types.hpp"
#include "video/video_system.hpp"
#include <iostream>
#include <stdexcept>

using namespace std::string_literals;

BGFXVideoSystem::BGFXVideoSystem(BGFXVideoSystem::Backend backend) :
	m_backend{VideoSystem::Backend::VIDEO_NULL},
	m_program{"data/shaders/frag.glsl", "data/shaders/vert.glsl"},
	m_is_shutdown{false}
{
	init(backend);
}

BGFXVideoSystem::~BGFXVideoSystem()
{
	shutdown();
}

void
BGFXVideoSystem::shutdown()
{
	if (!m_is_shutdown)
	{
		bgfx::shutdown();
		m_is_shutdown = true;
	}
}

std::string_view BGFXVideoSystem::get_name() const
{
	switch (m_backend)
	{
	case VideoSystem::Backend::VIDEO_BGFX_OPENGL:
		return "OpenGL";
	case VideoSystem::Backend::VIDEO_BGFX_METAL:
		return "Metal";
	case VideoSystem::Backend::VIDEO_BGFX_VULKAN:
		return "Vulkan";
	case VideoSystem::Backend::VIDEO_NULL:
	default:
		return "Null";
	}
}

void
BGFXVideoSystem::init(VideoSystem::Backend backend)
{
	bgfx::Init binit;
	u8 flags = SDL_WINDOW_RESIZABLE;
	switch (backend) {
		case VideoSystem::Backend::VIDEO_BGFX_OPENGL:
		{
			binit.type = bgfx::RendererType::OpenGL;
			flags |= SDL_WINDOW_OPENGL;
		}
			break;
		case VideoSystem::Backend::VIDEO_BGFX_OPENGLES:
		{
			binit.type = bgfx::RendererType::OpenGLES;
			flags |= SDL_WINDOW_OPENGL;
		}
			break;
		case VideoSystem::Backend::VIDEO_BGFX_VULKAN:
		{
			binit.type = bgfx::RendererType::Vulkan;
			flags |= SDL_WINDOW_VULKAN;
		}
			break;
		case VideoSystem::Backend::VIDEO_BGFX_METAL:
		{
			binit.type = bgfx::RendererType::Metal;
			flags |= SDL_WINDOW_METAL;
		}
			break;
		default:
			// Undesirable
			binit.type = bgfx::RendererType::Count;
			flags |= SDL_WINDOW_OPENGL | SDL_WINDOW_METAL | SDL_WINDOW_VULKAN;
			break;
	}
	// later
	binit.resolution.width = 800;
	binit.resolution.height = 600;
	
	window.create_window(flags, "SuperTux");
	
#ifdef SDL_PLATFORM_UNIX
	SDL_Window* sdlwin = window.m_sdl_window.get();
	std::string video_driver = SDL_GetCurrentVideoDriver();
	SDL_PropertiesID sdlprops = SDL_GetWindowProperties(sdlwin);
	if (video_driver == "x11") {
		void* xdisplay = SDL_GetPointerProperty(sdlprops,
		                                        SDL_PROP_WINDOW_X11_DISPLAY_POINTER,
		                                        NULL);
		void* xwindow = reinterpret_cast<void*>(SDL_GetNumberProperty(sdlprops,
		                                                            SDL_PROP_WINDOW_X11_WINDOW_NUMBER,
		                                                            0));

		if (!(xdisplay && xwindow))
			throw std::runtime_error("Error with X11");

		binit.platformData.ndt = xdisplay;
		binit.platformData.nwh = xwindow;
	}
	else if (video_driver == "wayland") {
		binit.platformData.type = bgfx::NativeWindowHandleType::Wayland;

		void* wdisplay = SDL_GetPointerProperty(sdlprops,
		                                        SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER,
		                                        NULL);

		// void* wsurface = SDL_GetPointerProperty(sdlprops,
		//                                         SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER,
		//                                         NULL);

		if (!wdisplay)
			throw std::runtime_error("Error with Wayland");

		binit.platformData.ndt = wdisplay;
		//binit.platformData.nwh = wsurface;
	}
#endif
	bgfx::renderFrame(); // Doing this before init does all gpu stuff in a single thread
	if (!bgfx::init(binit))
		throw std::runtime_error("bgfx::init() failed :-(");
	
	bgfx::setViewClear(0, BGFX_CLEAR_COLOR, 0x505050ff);
	on_resize(800, 600);
	
	//bgfx::VertexLayout

	m_backend = backend;
}

void
BGFXVideoSystem::on_resize(int w, int h)
{
	// Surely there is a better way?
	bgfx::reset(w, h);
	bgfx::setViewRect(0, 0, 0, w, h);
	bgfx::touch(0);
	flip();
}

void
BGFXVideoSystem::flip()
{
	bgfx::frame();
	
	bgfx::touch(0);
	// ---
	bgfx::frame();
	bgfx::renderFrame();
}
