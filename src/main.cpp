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

#include <atomic>
#include <format>
#include <iomanip>
#include <iostream>
#include <functional>
#include <variant>
#include "video/sdl/window.hpp"

using namespace std::string_literals;

void print_help(std::ostream& cout, int, char**, struct Arguments*, int opt_width = 30);

struct Arguments {
	/// You MUST provide a longarg if you provide a shortarg.
	const char* longarg;
	char shortarg;
	const char* paramhint;
	const char* desc;
	
	/// Callback 
	std::variant<std::monostate, std::function<int(int,char**)>, std::function<int(int,char**,std::string)>> cb;
};

Arguments st_args[] = {
	{ nullptr, 0, nullptr, "General Options:", {} },
	{ "help", 'h', nullptr, "Show this help message",
		[](int argc, char** argv){ print_help(std::cout, argc, argv, st_args); return 1; } },
	{ "version", 'V', nullptr, "Show SuperTux version", {} },
	{ "verbose", 'v', nullptr, "Show verbose messages",
		[](int argc, char** argv){ std::cout << "Enabling verbose mode..." << std::endl; return 0; } },
	{ nullptr, 0, nullptr, nullptr, {} }
};

bool check_arg(Arguments& arg, char* argument)
{
	// Note: Any method with a short argument requires a long argument
	return "--"s+arg.longarg == argument || (arg.shortarg != 0 ? "-"s+arg.shortarg == argument : false);
}

/**
 * Parses command line arguments.
 *
 * @param argc main argc parameter.
 * @param argv main argv parameter.
 * @param args Array of Arguments. Last member of array must have
 *  arguments set to null terminated values for strings, 0 for chars,
 *  and std::monostate for the callback, or else you will loop forever.
 * @return -1 on an invalid argument, 0 on success, 1 if the callback of an argument executed successfully
 */
int parse_arguments(int argc, char** argv, Arguments args[])
{
	bool invalid_argument = false;
	if (argc > 1)
	{
		for (int argv_idx = 1; argv_idx < argc; ++argv_idx)
		{
			for (int i = 0;; ++i)
			{
				if (args[i].longarg == nullptr && args[i].shortarg == 0 && args[i].desc == nullptr)
				{
					argv_idx = argc+1;
					invalid_argument = true;
					break;
				}

				// std::monostate
				if (args[i].cb.index() == 0)
					continue;

				try {
					auto fun = std::get<std::function<int(int,char**)>>(args[i].cb);
					if (check_arg(args[i], argv[argv_idx]))
					{
						if (fun(argc, argv) == 1)
							return 1;
						break;
					}
				} catch (std::bad_variant_access const& ex) { }

				try {
					auto fun = std::get<std::function<int(int,char**,std::string)>>(args[i].cb);
					if (check_arg(args[i], argv[argv_idx]))
					{
						if (fun(argc, argv, argv[argv_idx]) == 1)
							return 1;
						argv_idx++;
						break;
					}
				} catch (std::bad_variant_access const& ex) {  }
			}
		}
	}
	
	if (argc > 1 && invalid_argument)
	{
		std::cout << std::format("Invalid argument \"{}\".", argv[1]) << "\n" << std::endl;
		print_help(std::cerr, argc, argv, args);
		return -1;
	}
	
	return 0;
}

void print_help(std::ostream& cout, int argc, char** argv, Arguments args[], int opt_width /*= 30*/)
{
	cout << std::left;
	cout << std::format("Usage: {} [OPTIONS] [LEVELFILE]", argv[0]) << "\n";
	// Get 
	for (int i = 0;; ++i)
	{
		bool is_header = args[i].paramhint == nullptr && args[i].longarg == nullptr && args[i].shortarg == 0;
		if (args[i].longarg == nullptr && args[i].shortarg == 0 && args[i].desc == nullptr)
			break;
		
		else if (is_header)
			cout << args[i].desc;
			
		else {
			std::string arg_str = "";
			cout << "  ";
			if (args[i].shortarg)
				arg_str += "-"s + args[i].shortarg;
			if (args[i].shortarg && args[i].longarg)
				arg_str += ", ";
			if (args[i].longarg)
				arg_str += "--"s + args[i].longarg;
			if (args[i].paramhint)
				arg_str += " "s + args[i].paramhint;
			cout << std::setw(opt_width) << arg_str << std::setw(0) << args[i].desc;
		}
		cout << "\n";
	}
	
	cout << std::endl;
	cout << std::resetiosflags(std::ios_base::left);
}

	
int main(int argc, char** argv)
{
	SDLWindow window;
	if (parse_arguments(argc, argv, st_args) == 0)
	{
		window.create_window(0, "SuperTux Milestone 3");
		std::cout << "Start game" << std::endl;
	}

	return 0;
}
