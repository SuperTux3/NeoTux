# Will eventually handle game tests here too
include(CTest)

function(make_unit_test test_name)
  cmake_parse_arguments(PARSE_ARGV 1 mtargs
    "NO_PREPEND_SRC" "" "SOURCE;EXTERNAL;LIBRARIES;DEFINITIONS;INCLUDES")
  # EXTERNAL is just more readable for external source files we use
  if (NOT mtargs_NO_PREPEND_SRC)
    list(TRANSFORM mtargs_EXTERNAL PREPEND ${NeoTux_SOURCE_DIR}/src/)
  endif()
  add_executable(${test_name} ${mtargs_SOURCE} ${mtargs_EXTERNAL})
  target_compile_features(${test_name} PRIVATE cxx_std_17)
  target_include_directories(${test_name} PUBLIC ${NeoTux_SOURCE_DIR}/src ${mtargs_INCLUDES} ${PROJECT_BINARY_DIR})
  if (mtargs_DEFINITIONS)
    target_compile_definitions(${test_name} PUBLIC ${mtargs_DEFINITIONS})
  endif()
  if (mtargs_LIBRARIES)
    target_link_libraries(${test_name} PUBLIC ${mtargs_LIBRARIES})
  endif()
  set_property(TARGET ${test_name} PROPERTY CXX_STANDARD 20)
  set(all_test_targets "${all_test_targets};${test_name}" CACHE INTERNAL "")
  add_test(NAME ${test_name}
    COMMAND ${test_name})
endfunction(make_unit_test)

make_unit_test(SexpTest SOURCE sexp_test.cpp
	LIBRARIES SFSEXP
	EXTERNAL util/sexp.cpp)

make_unit_test(FilesystemTest SOURCE filesystem_test.cpp
	EXTERNAL util/filesystem.cpp)
	
make_unit_test(VectorTest SOURCE vector_test.cpp
	EXTERNAL math/vector.hpp)

make_unit_test(BindingsTest SOURCE bindings_test.cpp
	EXTERNAL input_manager.hpp)

#message("ALL TESTS: ${all_test_targets}")

set(NEOTUX_UNITTESTS_DIR "${PROJECT_SOURCE_DIR}/tests/unit" CACHE PATH "Where NeoTux will look for unit test data.")

add_custom_target(tests DEPENDS ${all_test_targets})
add_custom_command(TARGET tests POST_BUILD COMMAND ctest -C $<CONFIGURATION> --output-on-failure)
#add_dependencies(test_builds ${all_test_targets})
