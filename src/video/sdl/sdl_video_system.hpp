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

#ifndef HEADER_SUPERTUX_VIDEO_SDL_SDL_VIDEO_SYSTEM_HPP
#define HEADER_SUPERTUX_VIDEO_SDL_SDL_VIDEO_SYSTEM_HPP

#include <string>
#include <vector>
#include <string_view>
#include <bgfx/bgfx.h>
#include <SDL3/SDL.h>
#include "math/size.hpp"
#include "../video_system.hpp"
#include "video/bgfx/bgfx_program.hpp"
//#include "video/sampler.hpp"
//#include "video/texture_ptr.hpp"

class Rect;
class Renderer;
class SDLSurfacePtr;
//class Sampler;
class Surface;
class SurfaceData;
class Viewport;

class SDLVideoSystem : public VideoSystem
{

public:
	SDLVideoSystem(VideoSystem::Backend backend);
	virtual ~SDLVideoSystem();
	

	/** Return a human readable name of the current video system */
	std::string_view get_name() const override;

	Renderer* get_back_renderer() const {};
	Renderer& get_renderer() const {};
	Renderer& get_lightmap() const {};

	//virtual TexturePtr new_texture(const SDL_Surface& image, const Sampler& sampler = Sampler()) = 0;

	const Viewport& get_viewport() const {};
	void apply_config() {};
	void flip();
	void on_resize(int w, int h);
	Size get_window_size() const {};

	void set_vsync(int mode) {};
	int get_vsync() const {};
	void set_title(const std::string& title) {};
	void set_icon(const SDL_Surface& icon) {};
	//SDLSurfacePtr make_screenshot() {};
	
	void shutdown();

	void do_take_screenshot();

private:
	enum VideoSystem::Backend m_backend;
	
	void init(VideoSystem::Backend backend);
	
private:
	bool m_is_shutdown;
	
	SDLVideoSystem(const SDLVideoSystem&) = delete;
	SDLVideoSystem& operator=(const SDLVideoSystem&) = delete;
};

#endif
