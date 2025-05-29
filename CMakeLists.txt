cmake_minimum_required(VERSION 3.14)
project(SuperTux LANGUAGES CXX)
#set_property(TARGET SuperTux PROPERTY CXX_STANDARD 20

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(bgfx)

find_package(SDL3 REQUIRED)
#find_package(SDL3_ttf REQUIRED)
pkg_check_modules(SDL3_ttf sdl3-ttf)

file(GLOB_RECURSE SUPERTUX_SRC src/*.cpp src/*.hpp src/*/*.cpp src/*/*.hpp)
	
add_executable(SuperTux ${SUPERTUX_SRC})
target_include_directories(SuperTux PRIVATE src BGFX BIMG BX)
target_link_libraries(SuperTux PRIVATE BGFX SDL3::SDL3 ${SDL3_ttf_LINK_LIBRARIES} BIMG BX)
message("${harfbuzz_COMPILE_OPTIONS} ${plutosvg_FOUND}")
set_target_properties(SuperTux PROPERTIES OUTPUT_NAME supertux3)

set_property(TARGET SuperTux PROPERTY CXX_STANDARD 20)
