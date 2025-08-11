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

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <format>
#include <fstream>
#include <string>
#include <format>
#include "sexp.hpp"

SexpElt::SexpElt() :
	m_elt(nullptr)
{}

SexpElt::SexpElt(sexp_t *me) :
	m_elt(me)
{}

SexpElt
SexpElt::copy()
{
	return SexpElt(m_elt);
}


SexpElt
SexpElt::next()
{
	if (m_elt && m_elt->next)
		return SexpElt(m_elt->next);
		
	return SexpElt();
}

bool
SexpElt::next_inplace()
{
	if (!m_elt || !m_elt->next)
	{
		m_elt = nullptr;
		return false;
	}
	
	m_elt = m_elt->next;
	return true;
}

long
SexpElt::get_int()
{
	return std::stol(get_value());
}

double
SexpElt::get_number()
{
	// Using istringstream due to issues with std::from_chars with FreeBSD/clang(?)
	std::istringstream iss(get_value());
	iss.imbue(std::locale::classic());
	double result;
	iss >> result;
	return result;
}

// max_depth is decreased each recursion until it hits 0
static SexpElt
find_in_list(SexpElt curr, const std::string &name, int max_depth)
{
	SexpElt tmp;
	bool is_car = true;
	if (max_depth == 0)
		return SexpElt();
	while (curr)
	{
		if (is_car && curr.is_value() && curr.get_value() == name)
			return curr;
		if (curr.is_list())
		{
			tmp = find_in_list(curr.get_list(), name, max_depth - 1);
			if (tmp)
				return tmp;
		}
		
		curr.next_inplace();
		is_car = false;
	}
	
	return SexpElt();
}


SexpElt
SexpElt::find_car(const std::string &name, int max_depth)
{
	return find_in_list(*this, name, max_depth < 0 ? max_depth : max_depth + 1);
}


// ============================================= //

SexpParser::SexpParser() :
	m_sexp(nullptr, destroy_sexp)
{
}

SexpElt
SexpParser::read_data(const std::vector<char> &data)
{
	sexp_t *sexp = parse_sexp((char*)data.data(), data.size());
	if (!sexp)
		throw std::runtime_error("Failed to parse S-Expression");
	m_sexp.reset(sexp);
	
	return SexpElt(sexp);
}

SexpElt
SexpParser::read_file(const std::string &filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open())
		throw std::runtime_error(std::format("Couldn't open file: \"{}\"", filename));
	auto size = file.tellg();
	file.seekg(0, std::ios::beg);
		
	std::vector<char> buffer(size);
	if (file.read(buffer.data(), size))
		;
	//	return false;
	
	try
	{
		return read_data(buffer);
	}
	catch (std::exception &e)
	{
		throw std::runtime_error(std::format("{} for file \"{}\"", e.what(), filename));
	}
	
}
