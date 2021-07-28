cmake_minimum_required(VERSION 3.8)

project(
	vaeg
	VERSION 0.0.1
	DESCRIPTION "Godot Plugin"
	LANGUAGES CXX
)

if(MSVC)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/demo/addons/vaeg/bin/win64/)
endif()
if(APPLE)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/demo/addons/vaeg/bin/osx/)
else()
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/demo/addons/vaeg/bin/x11/)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_POSITION_INDEPENDENT_CODE ON) # -fPIC needed for clang to link

file(GLOB SRC_CORE "src/*.cpp")

# we want everything to be linked into a single file
set(VAE_BUILD_STATIC_LIBS ON)

add_subdirectory(VAE)

add_library(vaeg SHARED ${SRC_CORE})

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/godot_headers
	vae
)

target_link_libraries(vaeg vae)
