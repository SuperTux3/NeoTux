cmake_minimum_required(VERSION 3.14)
project(NeoTux LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
	add_compile_options(-O0 -g -ggdb -fsanitize=address -fno-omit-frame-pointer)
	add_link_options(-O0 -g -ggdb -fsanitize=address -fno-omit-frame-pointer)
endif()

set(DATA_DIR "${PROJECT_SOURCE_DIR}/data" CACHE PATH "Location of the data directory (shaders, levels, images...)")

# TODO: my module function
find_package(SDL3 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3_image REQUIRED IMPORTED_TARGET sdl3-image)
pkg_check_modules(SDL3_ttf REQUIRED IMPORTED_TARGET sdl3-ttf)

include(FetchContent)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
	cmake_policy(SET CMP0135 NEW)
endif()

set(BGFX_VERSION "v1.129.8914-493" CACHE STRING "Version of bgfx.cmake to use")
FetchContent_Declare(
	bgfx
	URL https://github.com/bkaradzic/bgfx.cmake/releases/download/${BGFX_VERSION}/bgfx.cmake.${BGFX_VERSION}.tar.gz
)

set(BGFX_BUILD_EXAMPLES OFF)
set(BGFX_BUILD_TOOLS ON)
FetchContent_MakeAvailable(bgfx)
include("${CMAKE_BINARY_DIR}/_deps/bgfx-src/cmake/bgfxToolUtils.cmake")

file(GLOB_RECURSE NEOTUX_SRC CONFIGURE_DEPENDS src/*.cpp src/*.hpp src/*/*.cpp src/*/*.hpp)

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

message("${CMAKE_BINARY_DIR}")

add_executable(NeoTux ${NEOTUX_SRC} "${DATA_DIR}/shaders/frag.glsl" "${DATA_DIR}/shaders/vert.glsl")
target_include_directories(NeoTux PRIVATE src)
target_compile_definitions(NeoTux PUBLIC
	DATA_DIR="${DATA_DIR}"
)
target_link_libraries(NeoTux PUBLIC
	SDL3::SDL3
	PkgConfig::SDL3_image
	PkgConfig::SDL3_ttf
	bgfx
)

# TODO: change this to neotux
set_target_properties(NeoTux PROPERTIES OUTPUT_NAME supertux3)

set_property(TARGET NeoTux PROPERTY CXX_STANDARD 20)
