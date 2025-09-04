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

#ifndef SUPERTUX_SRC_TILEMAP_HPP
#define SUPERTUX_SRC_TILEMAP_HPP

#include <optional>
#include <vector>
#include <string>
#include "collision.hpp"
#include "spatial_hash.hpp"
#include "math/size.hpp"
#include "thread_worker.hpp"
#include "tile.hpp"
#include "tile_chunk.hpp"
#include "util/sexp.hpp"
#include "video/sdl/surface_blitter.hpp"

struct ViewContext;
struct TilesReader;
struct MovingObject;

class Tilemap
{
public:
	Tilemap(SexpElt elt);
	~Tilemap() = default;
	
	Tile& get_tile(unsigned long x, unsigned long y);
	long get_zpos() const { return m_zpos; }
	Size get_size() const { return m_size; }
	
	// TODO: Make tiles_reader a member	
	std::optional<std::vector<Collision::CollideInfo<float>>> try_object_collision(MovingObject &obj);
	
	void draw(const ViewContext &camera);
	
protected:
	std::shared_ptr<ThreadWorker> m_threads;
	std::shared_ptr<SurfaceBlitter> m_tileset;
	
private:
	Size m_size;
	SpatialHash<TileChunk> m_chunks;
	long m_zpos;
	
	friend class TileChunk;
};

#endif
