#include <iostream>
#include "assert.h"
#include "config.h"
#include "util/sexp.hpp"

int
main()
{
	SexpParser parser;
	SexpElt s;
	s = parser.read_file(NEOTUX_UNITTESTS_DIR "/test_expressions");
	
	assert(s.is_list() == true);
	s = s.get_list().value();
	assert(s.get_value() == "enter");
	
	return 0;
}
