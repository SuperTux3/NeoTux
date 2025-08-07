set(SFSEXP_DIRECTORY "${CMAKE_BINARY_DIR}/_deps/sfsexp-src")
file(GLOB_RECURSE SFSEXP_SRC
	${SFSEXP_DIRECTORY}/src/*.c
	${SFSEXP_DIRECTORY}/src/*.h)

add_library(SFSEXP STATIC ${SFSEXP_SRC})
target_include_directories(SFSEXP PUBLIC ${SFSEXP_DIRECTORY}/src)
