cmake_minimum_required(VERSION 3.9.4)

project(
	vaeg
	VERSION 0.0.1
	DESCRIPTION "Godot Plugin"
	LANGUAGES CXX
)

if(MSVC)
	# SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /showIncludes")
	set(VAEG_WIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/demo/addons/vaeg/bin/win64/)
	# Don't put the dll in subfolders based on build config
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${VAEG_WIN_PATH})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${VAEG_WIN_PATH})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${VAEG_WIN_PATH})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${VAEG_WIN_PATH})
	# Don't add d at the debug dll name
	set(CMAKE_DEBUG_POSTFIX "")
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

set(VAE_PROFILER OFF)

add_subdirectory(VAE)

message("VAEG building with build type ${CMAKE_BUILD_TYPE}")

add_subdirectory(godot-cpp)

add_library(vaeg SHARED ${SRC_CORE})

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/godot-cpp/include
	${CMAKE_CURRENT_SOURCE_DIR}/godot-cpp/include/core
	${CMAKE_CURRENT_SOURCE_DIR}/build/godot-cpp/include/gen # the genrated source files are in the cmake build folder
	${CMAKE_CURRENT_SOURCE_DIR}/build/godot-cpp/include/
	vae
)

target_link_libraries(vaeg
	vae
	godot-cpp
)

include(CheckIPOSupported)
check_ipo_supported(RESULT lto_supported OUTPUT lto_error)

if(lto_supported)
	message(STATUS "IPO / LTO enabled")
	set_property(TARGET vaeg PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
	message(STATUS "IPO / LTO not supported: <${lto_error}>")
	# set_property(TARGET vaeg APPEND_STRING PROPERTY COMPILE_FLAGS "-flto -s")
endif()
