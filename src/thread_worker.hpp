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

#ifndef SUPERTUX_SRC_THREAD_WORKER_HPP
#define SUPERTUX_SRC_THREAD_WORKER_HPP

#include <iostream>
#include <list>
#include <memory>
#include <optional>
#include <vector>
#include <thread>
#include <atomic>
#include <functional>

struct ThreadInfo
{
	ThreadInfo() :
		m_done{std::make_shared<std::atomic_bool>(false)}
	{}
	~ThreadInfo() = default;
	
	void mark_as_done() { *m_done = true; }
private:
	std::shared_ptr<std::atomic_bool> m_done = nullptr;

	friend class ThreadWorker;
};

struct ThreadPair {
	ThreadInfo info;
	std::thread thread{};
};

// Some operations benefit from being threaded (for certain systems).
// Asynchronous operations would be preferred whenever, though.
class ThreadWorker
{
public:
	using id_t = std::thread::id;
	ThreadWorker(size_t max);
	~ThreadWorker();
	
	template <typename F, typename ...Args>
	bool spawn(id_t &id, F &&f, Args &&...args)
	{
		if (m_threads.size() >= m_max_threads)
			return false;
		
		// Safe, since m_threads is std::list
		ThreadPair &pair = m_threads.emplace_back();
		auto bound = std::bind(std::forward<F>(f), std::forward<Args>(args)..., std::ref(pair.info));
		pair.thread = std::move(std::thread(std::move(bound)));
		id = pair.thread.get_id();
		return true;
	}
	
	std::optional<id_t> poll();
	void remove_thread(id_t id);
public:
	ThreadInfo info;
	size_t m_max_threads;
	std::list<ThreadPair> m_threads;
	//std::atomic_uint32_t m_running_threads;
};

#endif
