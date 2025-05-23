cmake_minimum_required(VERSION 3.14)
project(SuperTux LANGUAGES CXX)
#set_property(TARGET SuperTux PROPERTY CXX_STANDARD 20

file(GLOB_RECURSE SUPERTUX_SRC src/*.cpp src/*.hpp)

add_executable(SuperTux ${SUPERTUX_SRC})
set_target_properties(SuperTux PROPERTIES OUTPUT_NAME supertux3)

set_property(TARGET SuperTux PROPERTY CXX_STANDARD 20)
