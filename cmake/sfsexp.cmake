file(GLOB_RECURSE SFSEXP_SRC
	${sfsexp_SOURCE_DIR}/src/*.c
	${sfsexp_SOURCE_DIR}/src/*.h)

add_library(SFSEXP STATIC ${SFSEXP_SRC})
target_include_directories(SFSEXP PUBLIC ${sfsexp_SOURCE_DIR}/src)
