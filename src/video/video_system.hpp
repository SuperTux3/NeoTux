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

#ifndef HEADER_SUPERTUX_VIDEO_VIDEO_SYSTEM_HPP
#define HEADER_SUPERTUX_VIDEO_VIDEO_SYSTEM_HPP

#include <string>
#include <vector>
#include <SDL3/SDL.h>
#include "math/size.hpp"
#include "sdl/window.hpp"
//#include "video/sampler.hpp"
//#include "video/texture_ptr.hpp"

class Rect;
class Renderer;
class SDLSurfacePtr;
class Sampler;
class Surface;
class SurfaceData;
class Viewport;

class VideoSystem
{
public:
	enum Backend {
		VIDEO_SDL,
		VIDEO_BGFX_OPENGL,
		VIDEO_BGFX_OPENGLES,
		VIDEO_BGFX_VULKAN,
		VIDEO_BGFX_METAL,
		VIDEO_NULL,

		VIDEO_AUTO = VIDEO_BGFX_VULKAN
	};

	static std::unique_ptr<VideoSystem> create(VideoSystem::Backend video_system);

	static Backend get_video_system(const std::string &video);
	static std::string get_video_string(Backend video);

	struct Info final
	{
		const Backend value;
		const std::string name;
		const std::string description;
	};
	static std::vector<Info> get_available_video_systems();

public:
	VideoSystem() = default;
	virtual ~VideoSystem() = default;

	/** Return a human readable name of the current video system */
	virtual std::string_view get_name() const = 0;

	virtual Renderer* get_back_renderer() const = 0;
	virtual Renderer& get_renderer() const = 0;
	virtual Renderer& get_lightmap() const = 0;

	//virtual TexturePtr new_texture(const SDL_Surface& image, const Sampler& sampler = Sampler()) = 0;

	virtual const Viewport& get_viewport() const = 0;
	virtual void apply_config() = 0;
	virtual void flip() = 0;
	virtual void on_resize(int w, int h) = 0;
	virtual Size get_window_size() const = 0;

	virtual void set_vsync(int mode) = 0;
	virtual int get_vsync() const = 0;
	virtual void set_title(const std::string& title) = 0;
	virtual void set_icon(const SDL_Surface& icon) = 0;
	//virtual SDLSurfacePtr make_screenshot() = 0;
	
	virtual void shutdown() = 0;

	void do_take_screenshot();

protected:
	SDLWindow window;

private:
	VideoSystem(const VideoSystem&) = delete;
	VideoSystem& operator=(const VideoSystem&) = delete;
};

extern std::unique_ptr<VideoSystem> g_video_system;

#endif

/* EOF */
