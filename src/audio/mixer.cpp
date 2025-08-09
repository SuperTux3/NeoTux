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

#include <SDL3_mixer/SDL_mixer.h>
#include <format>
#include "sdl_exception.hpp"
#include "util/filesystem.hpp"
#include "util/logger.hpp"
#include "mixer.hpp"

Mixer g_mixer;

Mixer::Mixer() :
	m_music(nullptr, Mix_FreeMusic),
	m_cache(),
	m_current_channel(0)
{
	SDL_AudioSpec spec;
	spec.freq = MIX_DEFAULT_FREQUENCY;
	spec.format = MIX_DEFAULT_FORMAT;
	spec.channels = MIX_DEFAULT_CHANNELS;
	
	SDL_Init(SDL_INIT_AUDIO);
	Mix_Init(MIX_INIT_OGG | MIX_INIT_WAVPACK);
	
	Mix_OpenAudio(0, &spec);
	Logger::info(std::format("Opened audio at {}Hz, {} bit{}, {} audio buffer",
		spec.freq,
		spec.format & 0xFF,
		SDL_AUDIO_ISFLOAT(spec.format) ? " (float)" : "",
		(spec.channels > 2) ? "surround" : (spec.channels > 1) ? "stereo" : "mono")); 
}

void
Mixer::shutdown()
{
	m_music.reset();
	m_cache.clear();
	//m_soundcache.clear();
}

bool
Mixer::is_playing_music()
{
	return Mix_PlayingMusic();
}

void
Mixer::stop_playing_music()
{
	Mix_HaltMusic();
}

// TODO Cache sounds
void
Mixer::play_sound(const std::string &filename)
{
	Mix_Chunk *chunk;
	if (m_cache.contains(filename))
	{
		chunk = m_cache.at(filename).get();
	}
	else {
		chunk = Mix_LoadWAV(FS::path(filename).c_str());
		m_cache.insert({filename, std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>(chunk, Mix_FreeChunk)});
	}
	
	if (!chunk)
		throw SDLException("Couldn't load chunk");
	
	++m_current_channel;
	if (m_current_channel == 4)
		m_current_channel = 0;
	Mix_PlayChannel(m_current_channel, chunk, false);
}

void
Mixer::play_music(const std::string &filename)
{
	Mix_Music *music;
	music = Mix_LoadMUS(FS::path(filename).c_str());
	
	if (!music)
	{
		throw SDLException("Couldn't load music");
	}
	
	Mix_FadeInMusic(music, true, 2000);
	
	m_music.reset(music);
}

