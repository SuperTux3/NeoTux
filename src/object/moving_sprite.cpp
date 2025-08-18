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

#include "moving_sprite.hpp"
#include "object/moving_object.hpp"
#include "util/filesystem.hpp"
#include "util/logger.hpp"
#include "video/painter.hpp"
#include "video/sdl/surface_blitter.hpp"
#include "video/texture_manager.hpp"
#include "video/video_system.hpp"
#include "config.h"
#include <SDL3/SDL_surface.h>

// PSP has 512x512 texture limit, so to bypass that we blit new textures from the source spritesheet
#ifdef PSP
#	define NEOTUX_BLIT_TEXTURES
#endif

#ifdef NEOTUX_BLIT_TEXTURES
#	include "video/texture_store.hpp"

	static SurfaceBlitter _blitter;
	static TextureStore<Rect> _tstore;
#endif

MovingSprite::MovingSprite(std::string sprite_file, std::string_view name) :
	MovingObject({0, 0, 0, 0}, {0, 0, 0, 0}, std::move(name)),
	m_filename(std::move(sprite_file)),
	m_action(nullptr),
	m_action_timer(0, 0),
	m_flip(0),
	m_alpha(1.0)
{
	if (!m_filename.empty())
	{
		m_parent_dir = FS::parent_dir(m_filename);
		parse_sprite();
	}
}

bool
MovingSprite::parse_sexp(SexpElt elt)
{
	SexpElt telt;
	std::string sprite;
	
	MovingObject::parse_sexp(elt);
	
	telt = elt.find_car("sprite");
	if (telt)
		sprite = telt.next().get_value();
	
	if (!sprite.empty())
	{
		set_sprite(sprite);
	}
	
	return true;
}

void
MovingSprite::set_sprite(const std::string &filename)
{
	m_filename = filename;
	m_parent_dir = FS::parent_dir(filename);
	parse_sprite();
}

void
MovingSprite::parse_sprite()
{
#ifdef NEOTUX_BLIT_TEXTURES
	int count = 0;
#endif
	SexpElt root, elt, aelt, telt;
	bool not_new = false;
	std::unique_ptr<SDL_Surface, decltype(&SDL_DestroySurface)> surface{nullptr, SDL_DestroySurface};
	
	if (m_actions.size() != 0)
	{
		m_actions.clear();
		m_action = nullptr;
		m_action_timer.reset();
		not_new = true;
	}
	
	root = m_parser.read_file(FS::path(m_filename));
	if (!root.is_list())
		return;
	
	root = root.get_list();
	
	if (root.get_value() == "supertux-sprite")
		Logger::debug("Claims to be a supertux sprite .... ^_^");
	
	while (root.next_inplace())
	{
		std::string name, spritesheet;
		double fps = 0.0;
		int loops = -1;
		float scale = 1.0f;
		int offset[] = { 0, 0 };
		int stride[] = { 1, 1 };
		std::vector<std::string> images;
		std::vector<Rectf> spritesheets;
		Rectf sheet;
		int hitboxes[4] = { -1, -1, -1, -1 };
		SpriteAction *sprite_action;
		
		if (!root.is_list())
			continue;
		elt = root.get_list();
		if (!elt.is_value())
			continue;
		
		if (elt.get_value() == "spritesheet")
		{
			aelt = elt.find_car("image");
			if (aelt)
				spritesheet = aelt.next().get_value();
			m_spritesheet = spritesheet;
#ifdef NEOTUX_BLIT_TEXTURES
			surface.reset(Texture::create_surface(FS::path(m_parent_dir + '/' + m_spritesheet)));
#endif
		}
		if (elt.get_value() == "action") {
			aelt = elt.find_car("name");
			if (aelt)
				name = aelt.next().get_value();

			aelt = elt.find_car("fps");
			if (aelt)
				fps = aelt.next().get_number();
			
			aelt = elt.find_car("scale");
			if (aelt)
				scale = aelt.next().get_number();

			aelt = elt.find_car("loops");
			if (aelt)
				loops = aelt.next().get_int();

			aelt = elt.find_car("images");
			if (aelt)
			{
				while (aelt.next_inplace())
				{
					//Logger::info(std::format("Trying to cache {}...", aelt.get_value()));
					//g_texture_manager.load(m_parent_dir + "/" + aelt.get_value());
					images.push_back(aelt.get_value());
				}
			}
			
			aelt = elt.find_car("offset");
			if (aelt)
				for (int i = 0; i < 2 && aelt.next_inplace(); ++i)
					offset[i] = aelt.get_int();
			
			aelt = elt.find_car("stride");
			if (aelt)
				for (int i = 0; i < 2 && aelt.next_inplace(); ++i)
					stride[i] = aelt.get_int();
			
			aelt = elt.find_car("spritesheets");
			if (aelt && !m_spritesheet.empty())
			{
				while (aelt.next_inplace())
				{
					if (!aelt.is_list())
						continue;
					telt = aelt.get_list();
					
					sheet.left = (telt.get_int() * stride[0]) + offset[0];
					telt.next_inplace();
					sheet.top = (telt.get_int() * stride[1]) + offset[1];
					telt.next_inplace();
					sheet.right = sheet.left + telt.get_int();
					telt.next_inplace();
					sheet.bottom = sheet.top + telt.get_int();
					
#ifdef NEOTUX_BLIT_TEXTURES
					_blitter.reset({ sheet.get_width(), sheet.get_height() });
					// Uncomment to debug if it's being blitted or not
					//_blitter.fill({0, 255, 0, 255});
					_blitter.blit(surface.get(),
						sheet.to_sdl_rect(),
						SDL_Rect{ 0, 0, sheet.get_width(), sheet.get_height() });
					_tstore.store(_blitter.to_texture(), m_spritesheet, Rect{sheet});
#endif
					
					//Logger::info(std::format("Trying to cache {}...", telt.get_value()));
					//g_texture_manager.load(m_parent_dir + "/" + telt.get_value());
					spritesheets.push_back(sheet);
				}
			}

			aelt = elt.find_car("hitbox");
			if (aelt)
			{
				int i = 0;
				for (i = 0; i < 4 && aelt.next_inplace(); ++i)
				{
					hitboxes[i] = aelt.get_int();
				}
				if (i != 4)
					Logger::warn("A sprite hitbox is missing values! Expect bugs...");
			}

			sprite_action = new SpriteAction(fps, loops, images, spritesheets, scale, hitboxes);
			m_actions.emplace(name, std::unique_ptr<SpriteAction>(sprite_action));
		}
	}
	
	if (!m_action_string.empty())
		set_action(m_action_string);
}

void
MovingSprite::set_action(const std::string &action)
{
	m_action_string = action;
	SpriteAction *s_action;
	try
	{
		s_action = m_actions.at(action).get();
		if (s_action == m_action)
			return;
	}
	catch (const std::out_of_range &err)
	{
		return;
	}
	m_action = s_action;
	
	if (!m_action->is_spritesheet())
	{
		std::string img = m_action->get_image(m_action_timer);
		if (!img.empty())
		{
			TextureRef tex = g_texture_manager.load(m_parent_dir + "/" + img);

			m_rect.right = m_rect.left + tex->get_size().width;
			m_rect.bottom = m_rect.top + tex->get_size().height;
		}
	}
	
	m_colbox.left = m_action->hitboxes[0] != -1 ? m_action->hitboxes[0] : 0;
	m_colbox.top = m_action->hitboxes[1] != -1 ? m_action->hitboxes[1] : 0;
	m_colbox.right = m_action->hitboxes[2] != -1 ? m_action->hitboxes[2] : m_rect.get_width();
	m_colbox.bottom = m_action->hitboxes[3] != -1 ? m_action->hitboxes[3] : m_rect.get_height();

	m_action_timer.set_duration((1.0/m_action->fps)*1000);
	m_action_timer.set_loops(m_action->loops);
}

void
MovingSprite::update(Sector &sector, Tilemap &tilemap)
{
	MovingObject::update(sector, tilemap);
	if (m_action)
		m_action_timer.tick();
}

void
MovingSprite::draw()
{
	if (!m_action)
		return;
	if (m_action->is_spritesheet())
	{
#ifdef NEOTUX_BLIT_TEXTURES
		Rect src = m_action->get_sprite(m_action_timer);
		TextureRef tex = _tstore.load(m_spritesheet, src);
		assert(tex);
		m_rect.right = m_rect.left + ((float)src.get_width() / m_action->scale);
		m_rect.bottom = m_rect.top + ((float)src.get_height() / m_action->scale);
		g_video_system->get_painter()->draw(tex, std::nullopt, m_rect, m_flip, m_alpha);
#else			
		TextureRef tex = g_texture_manager.load(m_parent_dir + "/" + m_spritesheet);
		Rectf src = m_action->get_sprite(m_action_timer);
		m_rect.right = m_rect.left + ((float)src.get_width() / m_action->scale);
		m_rect.bottom = m_rect.top + ((float)src.get_height() / m_action->scale);
		g_video_system->get_painter()->draw(tex, src, m_rect, m_flip, m_alpha);
#endif
	}
	else {
		std::string img = m_action->get_image(m_action_timer);
		if (img.empty())
			return;
		TextureRef tex = g_texture_manager.load(m_parent_dir + "/" + img);
		Rectf m_scaled_rect = m_rect;
		//m_scaled_rect.right
		g_video_system->get_painter()->draw(tex, std::nullopt, m_scaled_rect, m_flip, m_alpha);
	}
	MovingObject::draw();
}
