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

#include <charconv>
#include <format>
#include <iomanip>
#include <iostream>
#include <functional>
#include <optional>
#include <variant>

using namespace std::string_literals;

void print_help(int, char**, struct Arguments*, int opt_width = 30);

struct Arguments {
	const char* longarg;
	char shortarg;
	const char* paramhint;
	const char* desc;
	std::variant<std::monostate, std::function<void(int,char**)>, std::function<void(int,char**,std::string)>> cb;
};


Arguments st_args[] = {
	{ nullptr, 0, nullptr, "General Options:", {} },
	
	{ "help", 'h', nullptr, "Show this help message",
		[](int argc, char** argv){ print_help(argc, argv, st_args); } },
	{ "version", 'v', nullptr, "Show SuperTux version", {} },
	{ nullptr, 0, nullptr, nullptr, {} }
};

bool check_arg(Arguments& arg, char* argument)
{
	return "--"s+arg.longarg == argument || (arg.shortarg != 0 ? "-"s+arg.shortarg == argument : false);
}

int parse_arguments(int argc, char** argv, Arguments args[])
{
	if (argc > 1)
	{
		for (int i = 0;; ++i)
		{
			if (args[i].longarg == nullptr && args[i].shortarg == 0 && args[i].desc == nullptr)
				break;
		
			// std::monostate
			if (args[i].cb.index() == 0)
				continue;
			
			try {
				auto fun = std::get<std::function<void(int,char**)>>(args[i].cb);
				if (check_arg(args[i], argv[1]))
				{
					fun(argc, argv);
					return 1;
				}
			} catch (std::bad_variant_access const& ex) { }
			
			try {
				auto fun = std::get<std::function<void(int,char**,std::string)>>(args[i].cb);
				if (check_arg(args[i], argv[1]))
				{
					fun(argc, argv, argv[1]);
					return 1;
				}
			} catch (std::bad_variant_access const& ex) {  }
		}
	}
	
	if (argc > 1)
	{
		std::cout << std::format("Invalid argument \"{}\".", argv[1]) << "\n" << std::endl;
		print_help(argc, argv, args);
		return -1;
	}
	
	return 0;
}

void print_help(int argc, char** argv, Arguments args[], int opt_width /*= 30*/)
{
	std::cout << std::left;
	std::cout << std::format("Usage: {} [OPTIONS] [LEVELFILE]", argv[0]) << "\n";
	// Get 
	for (int i = 0;; ++i)
	{
		bool is_header = args[i].paramhint == nullptr && args[i].longarg == nullptr && args[i].shortarg == 0;
		if (args[i].longarg == nullptr && args[i].shortarg == 0 && args[i].desc == nullptr)
			break;
		
		else if (is_header)
			std::cout << args[i].desc;
			
		else {
			std::string arg_str = "";
			std::cout << "  ";
			if (args[i].shortarg)
				arg_str += "-"s + args[i].shortarg;
			if (args[i].shortarg && args[i].longarg)
				arg_str += ", ";
			if (args[i].longarg)
				arg_str += "--"s + args[i].longarg;
			std::cout << std::setw(opt_width) << arg_str << std::setw(0) << args[i].desc;
		}
		std::cout << "\n";
	}
	
	std::cout << std::endl;
	std::cout << std::resetiosflags(std::ios_base::left);
}

	
int main(int argc, char** argv)
{
	if (parse_arguments(argc, argv, st_args) == 0)
		std::cout << "Start game" << std::endl;
	//print_help(argc, argv, st_args);
	return 0;
}
