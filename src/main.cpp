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

#include <format>
#include <string>
#include <iomanip>
#include <iostream>
#include <functional>
#include <variant>
#include "util/logger.hpp"
#include "video/bgfx/bgfx_video_system.hpp"
#include "video/sdl/window.hpp"
#include "video/video_system.hpp"
#include "settings.hpp"
#include "game.hpp"

using namespace std::string_literals;

void print_help(std::ostream& cout, int, char**, struct Argument*, int opt_width = 30);

struct Argument {
	Argument():
		longarg(nullptr),
		shortarg(0),
		desc(nullptr),
		paramhint(nullptr)
	{

	}

	Argument(const char* longarg_,
			  char shortarg_,
			  const char* desc_,
			  const char* paramhint_ = nullptr):
		longarg(longarg_),
		shortarg(shortarg_),
		desc(desc_),
		paramhint(paramhint_)
	{
	}

	/// You MUST provide a longarg if you provide a shortarg.
	const char* longarg;
	char shortarg;
	const char* paramhint;
	const char* desc;
};

Argument st_args[] = {
	{ nullptr, 0, "General Options:" },
	{ "help", 'h', "Show this help message" },
	{ "version", 'V', "Show SuperTux version" },
	{ "verbose", 'v', "Show verbose messages" },
	{ "renderer", 'r',
	  "Use specific renderer ('auto', 'opengl', 'opengles', 'vulkan', 'metal', 'sdl', 'null')",
	  "<option>"},
	{}
};

bool check_arg(Argument& arg, char* argument)
{
	// Note: Any method with a short argument requires a long argument
	return "--"s+arg.longarg == argument || (arg.shortarg != 0 ? "-"s+arg.shortarg == argument : false);
}

int apply_argument(int argc, char** argv, int argvidx, Argument args[], int idx) {
	switch (idx) {
		case 1:
			g_settings->show_help = true;
			break;

		case 3:
			g_settings->verbose = true;
			break;

		case 4: {
			if (argvidx - 1 >= argc)
				return 1;

			std::string renderer = argv[argvidx + 1];
			g_settings->renderer = VideoSystem::str_to_video_system(renderer);
			break;
		}
	}

	return 0;
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
int parse_arguments(int argc, char** argv, Argument args[])
{
	if (argc <= 1)
		return 0;

	bool invalid_argument = false;
	int argv_idx = 1;
	for (argv_idx = 1; argv_idx < argc; ++argv_idx)
	{
		for (int i = 0;; ++i)
		{
			if (args[i].longarg == nullptr && args[i].shortarg == 0 && args[i].desc == nullptr)
			{
				//invalid_argument = true;
				break;
			}

			if (args[i].longarg == nullptr)
				continue;

			if (!check_arg(args[i], argv[argv_idx]))
				continue;

			apply_argument(argc, argv, argv_idx, args, i);
		}

		if (invalid_argument)
			break;
	}
	
	if (invalid_argument)
	{
		std::cout << std::format("Invalid argument \"{}\".", argv[argv_idx]) << "\n" << std::endl;
		print_help(std::cerr, argc, argv, args);
		return 1;
	}
	
	return 0;
}

void print_help(std::ostream& cout, int argc, char** argv, Argument args[], int opt_width /*= 30*/)
{
	cout << std::left;
	cout << std::format("Usage: {} [OPTIONS] [LEVELFILE]", argv[0]) << "\n";

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
	g_settings = std::make_unique<Settings>();

	int result = parse_arguments(argc, argv, st_args);
	if (result != 0)
		return result;

	if (g_settings->show_help)
	{
		print_help(std::cout, argc, argv, st_args);
		return 0;
	}

	g_game.run();
	// Simple loop for now
	//window.create_window(0, "SuperTux Milestone 3");
	
	return 0;
}
