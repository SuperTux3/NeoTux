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

#ifndef SUPERTUX_SRC_AUDIO_MIXER_HPP
#define SUPERTUX_SRC_AUDIO_MIXER_HPP

#include <memory>
#include <vector>
#include <unordered_map>
#include <SDL3_mixer/SDL_mixer.h>

class Mixer
{
public:
	Mixer();
	~Mixer() = default;
	
	void shutdown();
	void play_sound(const std::string &filename);
	void play_music(const std::string &filename);
private:
	std::unordered_map<std::string, std::unique_ptr<Mix_Chunk>> m_cache;
	std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)> m_music;
	//std::vector<std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>> m_soundcache;
};

extern Mixer g_mixer;

#endif
