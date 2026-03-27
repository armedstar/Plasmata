cmake_minimum_required(VERSION 3.11)
project(VCONoiseDelay VERSION 1.0.0)

# Find Rack
find_path(RACK_DIR plugin.hpp PATHS
	../Rack
	../../Rack
	../../../Rack
	~/Rack
	/usr/local
	/usr
	${CMAKE_SOURCE_DIR}/Rack
	ENV RACK_DIR
)

include_directories(${RACK_DIR}/include)
include_directories(${RACK_DIR}/dep/include)

# Source files
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.hpp)

# Create plugin library
add_library(${PROJECT_NAME} SHARED
	plugin.cpp
	${SOURCES}
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# Link libraries
find_library(RACK_LIBRARY rack PATHS ${RACK_DIR}/build)
target_link_libraries(${PROJECT_NAME} ${RACK_LIBRARY})

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()

# Installation
set(PLUGIN_NAME ${PROJECT_NAME})
set(TARGET_FILE $<TARGET_FILE:${PROJECT_NAME}>)
if(APPLE)
	set(PLUGIN_EXT .dylib)
elseif(WIN32)
	set(PLUGIN_EXT .dll)
else()
	set(PLUGIN_EXT .so)
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin
	COMMAND ${CMAKE_COMMAND} -E copy ${TARGET_FILE} ${CMAKE_SOURCE_DIR}/bin/${PLUGIN_NAME}${PLUGIN_EXT}
	COMMENT "Copying plugin to bin/"
)

