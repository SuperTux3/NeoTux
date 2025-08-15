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

#include "thread_worker.hpp"
#include <iostream>

ThreadWorker::ThreadWorker(size_t max) :
	m_max_threads(max),
	m_threads()
{
}

ThreadWorker::~ThreadWorker()
{
	for (auto &threadinfo : m_threads)
	{
		if (threadinfo.thread.joinable())
			threadinfo.thread.join();
	}
}

std::optional<ThreadWorker::id_t>
ThreadWorker::poll()
{
	for (auto it = m_threads.begin(); it != m_threads.end(); ++it)
	{
		if (*it->info.m_done == true)
		{
			ThreadWorker::id_t id = it->thread.get_id();
			if (it->thread.joinable())
				it->thread.join();
			std::cout << "Thread finished :D" << std::endl;
			// TODO delete thread
			m_threads.erase(it);
			return id;
		}
	}
	return std::nullopt;
}

