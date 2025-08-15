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

#include "tile_chunk.hpp"
#include "thread_worker.hpp"
#include "tiles_reader.hpp"
#include "video/sdl/surface_blitter.hpp"
#include "video/texture_manager.hpp"
#include "config.h"
#include "tilemap.hpp"
#include <atomic>
#include <stdexcept>
#include <thread>

#ifdef NEOTUX_PSP
// temporary
constexpr int texture_size = 10;
#else
constexpr int texture_size = 32;
#endif

static SurfaceBlitter tileset({
	(int)texture_size * TileChunk::CHUNK_SIZE,
	(int)texture_size * TileChunk::CHUNK_SIZE});
static std::thread texture_worker;
static std::atomic_bool texture_worker_running = false;
static std::atomic_bool just_finished_texture = false;


TileChunk::~TileChunk()
{
	if (texture_worker.joinable())
		texture_worker.join();
}

Tile&
TileChunk::get_tile(uint8_t x, uint8_t y)
{
	// TODO bounds checking
	if (x >= TileChunk::CHUNK_SIZE || y >= TileChunk::CHUNK_SIZE)
		throw std::runtime_error("BAD TILE");
	return m_tiles[x + (y * TileChunk::CHUNK_SIZE)];
}

void
TileChunk::update_texture_worker(ThreadInfo &info)
{
	tileset.reset();
	for (int i = 0; i < CHUNK_SIZE; ++i)
	{
		for (int j = 0; j < CHUNK_SIZE; ++j)
		{
			SDL_Rect dest{i * texture_size, j * texture_size, texture_size, texture_size};
			try{
			TileMeta &meta = g_tiles_reader.m_tiles.at(get_tile(i, j).get_id());
			
			const std::string &img = meta.info->get_image();
			SDL_Rect src{(int)meta.x * 32, (int)meta.y * 32, 32, 32};
			if (img.empty())
				continue;
			TextureRef tile = g_texture_manager.load("images/" + img, true);
#ifndef NDEBUG
			if (!tile->get_sdl_surface())
			{
				Logger::warn(img + " doesn't have an SDL Surface... Expect bad!");
				continue;
			}
#endif
			tileset.blit(tile->get_sdl_surface(), src, dest);
			} 
			catch (const std::out_of_range&) {
				continue;
			}
		}
	}
	info.mark_as_done();
}

void
TileChunk::update_texture(Tilemap *parent)
{
	if (!m_texture.get())
	{
		//if (m_parent)
		if (!texture_updating)
		{
			if (parent->m_threads->spawn(m_thread_id, &TileChunk::update_texture_worker, this))
				texture_updating = true;
			
			return;
		}
		
		auto id = parent->m_threads->poll();
		if (id && *id == m_thread_id && texture_updating)
		{
			parent->m_threads->remove_thread(m_thread_id);
			m_texture.reset(tileset.to_texture());
			texture_updating = false;
		}
	}
}
