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

#ifndef SUPERTUX_SRC_UTIL_SEXP_HPP
#define SUPERTUX_SRC_UTIL_SEXP_HPP

#include <cstdint>
#include <memory>
#include <variant>
#include <optional>
#include <string_view>
#include <vector>
#include <string>
#include "sexp.h"

class SexpElt
{
public:
	// std::string_view doesn't support stringstream, so use std::string for now
	using string_t = std::string;
	using value_t = std::variant<SexpElt, string_t, std::monostate>;
	SexpElt();
	//SexpElt(SexpElt &) {};
	SexpElt(sexp_t *);
	~SexpElt() = default;
	
	bool is_list() const { return m_elt->ty == SEXP_LIST; }
	bool is_value() const { return m_elt->ty == SEXP_VALUE; }
	bool is_valid() const { return m_elt; }
	operator bool() const { return is_valid(); }
	SexpElt get_list()
	{
		if (m_elt->list)
			return SexpElt(m_elt->list);
		return SexpElt();
	}
	SexpElt find_car(const std::string& name, int max_depth = -1);
	
	string_t get_value() { return m_elt->ty == SEXP_VALUE ? m_elt->val : string_t(); }
	
	double get_number();
	long get_int();
	long get_int_or(long that);
	
	value_t get()
	{
		if (m_elt->ty == SEXP_VALUE)
			return get_value();
		else if (m_elt->list)
			return SexpElt(m_elt->list);
		return std::monostate{};
	}
	SexpElt copy();
	SexpElt next();
	bool next_inplace();
private:
	sexp_t *m_elt;
};


/* 
 * Only supports 1 s-exp (typically the root one) at the moment
 *  because the game never uses multiple s-exp in a file anyway
 *
 * So (one) \n (two (three four)) is not valid yet.
 */
class SexpParser
{
public:
	SexpParser();
	~SexpParser() = default;
	
	SexpElt read_data(const std::vector<char> &data);
	SexpElt read_file(const std::string &filename);
private:
	std::unique_ptr<sexp_t, decltype(&destroy_sexp)> m_sexp;
};

#endif
