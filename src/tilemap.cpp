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

#include "tilemap.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "math/rect.hpp"
#include "object/moving_object.hpp"
#include "tiles_reader.hpp"
#include "util/logger.hpp"
#include "video/texture_manager.hpp"
#include "video/video_system.hpp"
#include <algorithm>
#include <format>

Tilemap::Tilemap(SexpElt root) :
	m_size(),
	m_chunks(),
	m_zpos()
{
	SexpElt elt;
	elt = root.find_car("width", 1);
	if (elt.next_inplace())
		m_size.width = elt.get_int();
	elt = root.find_car("height", 1);
	if (elt.next_inplace())
		m_size.height = elt.get_int();
	elt = root.find_car("z-pos", 1);
	if (elt.next_inplace())
		m_zpos = elt.get_int();
	
	elt = root.find_car("tiles", 1);
	for (int i = 0; elt.next_inplace(); ++i)
	{
		long tile_id = elt.get_int();
		//if (tile_id == 0)
		//	continue;
		int x = i % m_size.width;
		int y = i / m_size.width;
		int chunk_x = x / TileChunk::CHUNK_SIZE;
		int chunk_y = y / TileChunk::CHUNK_SIZE;
		int rel_x = x % TileChunk::CHUNK_SIZE;
		int rel_y = y % TileChunk::CHUNK_SIZE;
		TileChunk &t = m_chunks.get_or_create(chunk_x, chunk_y, TileChunk());
		t.get_tile(rel_x, rel_y).set_id(tile_id);
	}
	
	
	Logger::debug(std::format("Tilemap info\n\t"
	                          "Width: {}\n\t"
							  "Height: {}\n\t"
							  "z-pos: {}\n\t"
							  "# of tiles: {}", m_size.width, m_size.height, m_zpos, 0));
}

Tile&
Tilemap::get_tile(unsigned long x, unsigned long y)
{
	int chunk_x = x / TileChunk::CHUNK_SIZE;
	int chunk_y = y / TileChunk::CHUNK_SIZE;
	int rel_x = x % TileChunk::CHUNK_SIZE;
	int rel_y = y % TileChunk::CHUNK_SIZE;
	TileChunk *chunk = m_chunks.at(chunk_x, chunk_y);
	if (!chunk)
		return m_chunks.at(0,0)->get_tile(0,0);
	return chunk->get_tile(rel_x, rel_y);
}

std::optional<std::vector<Collision::CollideInfo<float>>>
Tilemap::try_object_collision(MovingObject& obj)
{
	std::vector<Collision::CollideInfo<float>> colvec;
	Rectf obj_rect = obj.get_colbox();
	
	obj.m_on_slope = false;
	auto cols = Collision::get_chunk_collisions(obj_rect, 32.f);
	for (long x = cols.left; x <= cols.right; ++x)
	{
		for (long y = cols.top; y <= cols.bottom; ++y)
		{
			Tile &tile = get_tile(x, y);
			if (tile.get_id() == 0)
				continue;
			try {
			const TileMeta &meta = g_tiles_reader.m_tiles.at(tile.get_id());
			if ((meta.attrs & TileMeta::SOLID) == 0 &&
			    (meta.attrs & TileMeta::UNISOLID) == 0 &&
				(meta.attrs & TileMeta::BRICK) == 0)
				continue;
			
			Tile &tile_below = get_tile(x, y+1);
			
			if ((meta.attrs & TileMeta::SLOPE) == TileMeta::SLOPE)
			{
				bool is_slope_roof = (meta.datas & Collision::SLOPE_NORTHEAST) == Collision::SLOPE_NORTHEAST ||
				(meta.datas & Collision::SLOPE_NORTHWEST) == Collision::SLOPE_NORTHWEST;
				bool is_slope_roof_above_tile = tile_below.get_id() != 0 && is_slope_roof;
				/* Handle edge case (pun not intended) where object is on edge of slope from below
				 *  Where O == object && \ or / == slope (NE/NW)
				 *          _________
				 * stuck    |  , . '|
				 *  \--->  O|' , ___|
				 *          \___/
				 */
					
				// Slope collision
				Vec2 lines[2];
				bool success = Collision::get_line_from_slope_metas(meta.datas, lines);
				Vec2 normal = Collision::get_normal_from_slope_metas(meta.datas);
				std::cout << normal.to_string() << std::endl;
				
				lines[0] *= 32.f;
				lines[1] *= 32.f;
				lines[0].x += x*32;
				lines[0].y += y*32;
				lines[1].x += x*32;
				lines[1].y += y*32;
				// TODO: Move this to draw
				if (g_settings->show_hitboxes)
					g_video_system->get_painter()->draw_line(lines[0], lines[1], {255, 0, 0, 255});
				obj.do_slope_collision(meta.datas, lines[0], lines[1]);
			}
			else {
				// Tile collision
				Rectf rrect{(float)(x*32), (float)(y*32), {32.f, 32.f}};
				auto col = obj.do_collision(rrect);
				if (col)
					colvec.emplace_back(col);
			}
			}
			catch (...)
			{
				std::cout << "No: " << tile.get_id() << std::endl;
			}
		}
	}
	
	if (!colvec.empty())
	{
		return std::make_optional<std::vector<Collision::CollideInfo<float>>>(std::move(colvec));
	}
	
	return std::nullopt;
}

void
Tilemap::draw(const Camera &camera)
{
	Painter *painter = g_video_system->get_painter();
	float cam_tx = camera.x / 32.f;
	float cam_ty = camera.y / 32.f;
	float cam_tw = camera.width / 32.f;
	float cam_th = camera.height / 32.f;
	
	for (int x = std::max<long>(0, cam_tx); x < cam_tx + cam_tw && x < m_size.width; ++x)
	{
		for (int y = std::max<long>(0, cam_ty); y < cam_ty + cam_th && y < m_size.height; ++y)
		{
			int rx = x * 32;
			int ry = y * 32;
			Rectf rrect{(float)rx, (float)ry, {32.f, 32.f}};
			const Tile &tile = get_tile(x, y);
			if (tile.get_id() != 0)
			{
				try{
					TileMeta &tilemeta = g_tiles_reader.m_tiles.at(tile.get_id());
					if (tilemeta.info->images.empty())
						continue;
					tilemeta.info->timer.tick();
					TextureRef tex = g_texture_manager.load("images/"+tilemeta.info->get_image());
					painter->draw(tex, tilemeta.get_src_rect(tex), rrect);
				}catch(...){
					continue;
				}
			}
		}
	}
}
