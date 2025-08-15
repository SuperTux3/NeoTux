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

#ifndef SUPERTUX_SRC_TILE_CHUNK_HPP
#define SUPERTUX_SRC_TILE_CHUNK_HPP

#include <cstdint>
#include <vector>
#include "thread_worker.hpp"
#include "config.h"
#include "tile.hpp"
#include "video/texture.hpp"

#ifdef NEOTUX_PSP
// temporary
constexpr int texture_size = 10;
#else
constexpr int texture_size = 32;
#endif

struct Tilemap;

class TileChunk
{
public:
	static constexpr uint8_t CHUNK_SIZE = 8;
	
	TileChunk();
	~TileChunk();
	
	void update_texture(Tilemap *parent);
	void update_texture_worker(Tilemap *parent, ThreadInfo &info);
	Texture *get_texture() const { return m_texture.get(); }
	
	Tile& get_tile(uint8_t x, uint8_t y);
	void set_tile(uint8_t x, uint8_t y, unsigned long id);
	
private:
	bool m_empty;
	ThreadWorker::id_t m_thread_id;
	bool texture_updating;
	std::shared_ptr<Texture> m_texture;
	Tile m_tiles[CHUNK_SIZE * CHUNK_SIZE];
};

#endif
