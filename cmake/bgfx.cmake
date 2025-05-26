# BX
project(BX LANGUAGES CXX C)

set(BX_DIRECTORY ${SuperTux_SOURCE_DIR}/external/bx)

file(GLOB_RECURSE BX_SRC
	${BX_DIRECTORY}/src/*.cpp
	${BX_DIRECTORY}/include/bx/*.h
	${BX_DIRECTORY}/include/bx/inline/*.inl)

list(REMOVE_ITEM BX_SRC
	${BX_DIRECTORY}/src/amalgamated.cpp
	${BX_DIRECTORY}/src/crtnone.cpp)

add_library(BX ${BX_SRC})
target_include_directories(BX PUBLIC
	${BX_DIRECTORY}/include 
	${BX_DIRECTORY}/include/compat
	${BX_DIRECTORY}/include/compat/linux
	${BX_DIRECTORY}/3rdparty)
target_compile_definitions(BX PUBLIC
	BX_CONFIG_DEBUG=0)

# BIMG
project(BIMG LANGUAGES CXX C)

set(BIMG_DIRECTORY ${SuperTux_SOURCE_DIR}/external/bimg)

file(GLOB_RECURSE BIMG_SRC
	${BIMG_DIRECTORY}/include/bimg/*.h
	${BIMG_DIRECTORY}/src/image.cpp
	${BIMG_DIRECTORY}/src/image_gnf.cpp
	${BIMG_DIRECTORY}/src/*.h
	${BIMG_DIRECTORY}/3rdparty/astc-encoder/source/*.cc)

add_library(BIMG ${BIMG_SRC})
target_link_libraries(BIMG BX)
target_include_directories(BIMG PUBLIC
	${BX_DIRECTORY}/include
	${BIMG_DIRECTORY}/include
	${BIMG_DIRECTORY}/3rdparty/astc-encoder/include)

# BGFX	
project(BGFX LANGUAGES CXX C)

set(BGFX_DIRECTORY ${SuperTux_SOURCE_DIR}/external/bgfx)

file(GLOB_RECURSE BGFX_SRC
	${BGFX_DIRECTORY}/src/*.cpp
	${BGFX_DIRECTORY}/src/*.h
	${BGFX_DIRECTORY}/include/bgfx/**.h
	${BGFX_DIRECTORY}/include/bgfx/)

list(REMOVE_ITEM BGFX_SRC ${BGFX_DIRECTORY}/src/amalgamated.cpp)

add_library(BGFX ${BGFX_SRC})
#add_dependencies(BGFX BX BIMG)
target_link_libraries(BGFX BX BIMG)
target_include_directories(BGFX PRIVATE
	${BGFX_DIRECTORY}/include 
	${BGFX_DIRECTORY}/3rdparty
	${BGFX_DIRECTORY}/3rdparty/dxsdk/include
	${BGFX_DIRECTORY}/3rdparty/khronos
	${BGFX_DIRECTORY}/3rdparty/directx-headers/include/directx
	${BGFX_DIRECTORY}/3rdparty/directx-headers/include/directx/include
	${BGFX_DIRECTORY}/3rdparty/directx-headers/include/wsl/stubs)
