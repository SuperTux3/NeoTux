#include <iostream>
#include "assert.h"
#include "config.h"
#include "util/sexp.hpp"

using namespace std::string_literals;

int
main()
{
	SexpParser parser, bad_parser;
	SexpElt s;
	long l;
	double d;
	s = parser.read_file(NEOTUX_UNITTESTS_DIR "/test_expressions");
	
	assert(s.is_list() == true);
	s = s.get_list().value();
	assert(s.is_list() == true);
	s = s.get_list().value();
	assert(s.get_value() == "enter");
	s.next_inplace();
	
	l = s.get_int();
	assert(l == 4023);
	
	s.next_inplace();
	
	d = s.get_number();
	assert(d == 49232.325);
	
	// Bad test
	try
	{
		s = bad_parser.read_file(NEOTUX_UNITTESTS_DIR "/bad_expressions");
	}
	catch (std::exception &e)
	{
		assert(e.what() == "Failed to parse S-Expression for file \"" NEOTUX_UNITTESTS_DIR "/bad_expressions\""s);
	}
	
	return 0;
}
