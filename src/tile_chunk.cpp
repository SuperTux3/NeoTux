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
#include "tiles_reader.hpp"
#include "video/sdl/surface_blitter.hpp"
#include "video/texture_manager.hpp"
#include <stdexcept>

Tile&
TileChunk::get_tile(uint8_t x, uint8_t y)
{
	// TODO bounds checking
	if (x >= TileChunk::CHUNK_SIZE || y >= TileChunk::CHUNK_SIZE)
		throw std::runtime_error("BAD TILE");
	return m_tiles[x + (y * TileChunk::CHUNK_SIZE)];
}

void
TileChunk::update_texture()
{
	SurfaceBlitter tileset({(int)32 * CHUNK_SIZE, (int)32.f * CHUNK_SIZE});
	for (int i = 0; i < CHUNK_SIZE; ++i)
	{
		for (int j = 0; j < CHUNK_SIZE; ++j)
		{
			SDL_Rect dest{i * 32, j * 32, 32, 32};
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
	m_texture.reset(tileset.to_texture());
	tileset.destroy();
}
