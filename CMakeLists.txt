cmake_minimum_required(VERSION 3.14)
project(NeoTux LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(DATA_DIR "${PROJECT_SOURCE_DIR}/data" CACHE PATH "Location of the data directory (shaders, levels, images...)")

# TODO: my module function
find_package(SDL3 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3_image REQUIRED IMPORTED_TARGET sdl3-image)
pkg_check_modules(SDL3_ttf REQUIRED IMPORTED_TARGET sdl3-ttf)
#pkg_check_modules(SDL3_mixer REQUIRED IMPORTED_TARGET sdl3-mixer)

include(FetchContent)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
	cmake_policy(SET CMP0135 NEW)
endif()

set(SFSEXP_VERSION "1.4.1" CACHE STRING "Version of SfSexp to use")
FetchContent_Declare(
	sfsexp
	URL https://github.com/mjsottile/sfsexp/releases/download/v${SFSEXP_VERSION}/sfsexp-${SFSEXP_VERSION}.tar.gz
)
FetchContent_MakeAvailable(sfsexp)
include("${PROJECT_SOURCE_DIR}/cmake/sfsexp.cmake")

FetchContent_Declare(
	SDL3_mixer
	URL https://github.com/libsdl-org/SDL_mixer/archive/68764f35899e133b402336843c046d75136eaf08.tar.gz
)
FetchContent_MakeAvailable(SDL3_mixer)

set(SSQ_BUILD_STATIC_ONLY Off)

if(EXISTS "${PROJECT_SOURCE_DIR}/external/simplesquirrel")
	message(STATUS "Using external/simplesquirrel")
	add_subdirectory("${PROJECT_SOURCE_DIR}/external/simplesquirrel")
else()
	FetchContent_Declare(
		simplesquirrel
		GIT_REPOSITORY "https://github.com/swagtoy/simplesquirrel"
		GIT_BRANCH "master"
	)
	FetchContent_MakeAvailable(simplesquirrel)
endif()

option(NEOTUX_BGFX "Use BGFX library for NeoTux." OFF)
if(NEOTUX_BGFX)
	set(BGFX_VERSION "v1.129.8914-493" CACHE STRING "Version of bgfx.cmake to use")
	FetchContent_Declare(
		bgfx
		URL https://github.com/bkaradzic/bgfx.cmake/releases/download/${BGFX_VERSION}/bgfx.cmake.${BGFX_VERSION}.tar.gz
	)

	set(BGFX_BUILD_EXAMPLES OFF)
	set(BGFX_BUILD_TOOLS ON)
	FetchContent_MakeAvailable(bgfx)
	include("${CMAKE_BINARY_DIR}/_deps/bgfx-src/cmake/bgfxToolUtils.cmake")

	bgfx_compile_shaders(
		TYPE VERTEX
		SHADERS "${DATA_DIR}/shaders/vert.glsl"
		VARYING_DEF "${DATA_DIR}/shaders/varying.def.sc"
		OUTPUT_DIR "${CMAKE_BINARY_DIR}/include/shaders"
	)

	bgfx_compile_shaders(
		TYPE FRAGMENT
		SHADERS "${DATA_DIR}/shaders/frag.glsl"
		VARYING_DEF "${DATA_DIR}/shaders/varying.def.sc"
		OUTPUT_DIR "${CMAKE_BINARY_DIR}/include/shaders"
	)
endif()

set(BUILD_TESTING OFF CACHE BOOL Enable tests)
if(BUILD_TESTING)
  #include(SuperTux/BuildTests)
  set(CMAKE_CTEST_ARGUMENTS "--output-on-failure")
  enable_testing()
  add_subdirectory(tests)
endif()

set(NEOTUX_DATA_DIR "${PROJECT_SOURCE_DIR}/data" CACHE PATH "Where NeoTux will look for game assets.")

file(GLOB_RECURSE NEOTUX_SRC CONFIGURE_DEPENDS
	src/*.cpp src/*.hpp src/*/*.cpp src/*/*.hpp
	tests/game_tests/*.cpp tests/game_tests/*.hpp)

if(NOT NEOTUX_BGFX)
	list(FILTER NEOTUX_SRC EXCLUDE REGEX bgfx/)
endif()

add_executable(NeoTux ${NEOTUX_SRC} )
target_include_directories(NeoTux PRIVATE src tests/game_tests)
target_link_libraries(NeoTux PUBLIC
	SFSEXP
	SDL3::SDL3
	PkgConfig::SDL3_image
	PkgConfig::SDL3_ttf
	SDL3_mixer::SDL3_mixer
	simplesquirrel
)

if(NEOTUX_BGFX)
	target_sources(NeoTux PRIVATE "${DATA_DIR}/shaders/frag.glsl" "${DATA_DIR}/shaders/vert.glsl")
	target_link_libraries(NeoTux PUBLIC bgfx)
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug")
	target_compile_options(NeoTux PUBLIC -O0 -g -ggdb -fsanitize=address -fno-omit-frame-pointer)
	target_link_options(NeoTux PUBLIC -O0 -g -ggdb -fsanitize=address -fno-omit-frame-pointer)
endif()

configure_file(config.h.in ${PROJECT_BINARY_DIR}/config.h)
target_include_directories(NeoTux PRIVATE ${PROJECT_BINARY_DIR})

# TODO: change this to neotux
set_target_properties(NeoTux PROPERTIES OUTPUT_NAME supertux3)

set_property(TARGET NeoTux PROPERTY CXX_STANDARD 20)
