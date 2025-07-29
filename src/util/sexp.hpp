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
	SexpElt() = default;
	//SexpElt(SexpElt &) {};
	SexpElt(sexp_t *);
	~SexpElt() = default;
	
	bool is_list() { return m_elt->ty == SEXP_LIST; }
	bool is_value() { return m_elt->ty == SEXP_VALUE; }
	std::optional<SexpElt> get_list()
	{
		if (m_elt->list)
			return SexpElt(m_elt->list);
		return std::nullopt;
	}
	
	std::string_view get_value() { return m_val; }
	
	std::variant<SexpElt, std::string_view, std::monostate> get()
	{
		if (m_elt->ty == SEXP_VALUE)
			return m_val;
		else if (m_elt->list)
			return SexpElt(m_elt->list);
		return std::monostate{};
	}
	std::optional<SexpElt> next();
private:
	std::string_view m_val;
	sexp_t *m_elt;
};

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
