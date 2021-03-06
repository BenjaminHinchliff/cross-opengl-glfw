﻿cmake_minimum_required(VERSION 3.11)

set(SHADERS_PREFIX "${PROJECT_SOURCE_DIR}/res/shaders")
if(EMSCRIPTEN)
file(READ "${SHADERS_PREFIX}/triangle_es.vert" TRIANGLE_VERTEX_SOURCE)
file(READ "${SHADERS_PREFIX}/triangle_es.frag" TRIANGLE_FRAGMENT_SOURCE)
else()
file(READ "${SHADERS_PREFIX}/triangle.vert" TRIANGLE_VERTEX_SOURCE)
file(READ "${SHADERS_PREFIX}/triangle.frag" TRIANGLE_FRAGMENT_SOURCE)
endif()
configure_file("${CMAKE_CURRENT_LIST_DIR}/shaders.in.hpp" "${CMAKE_CURRENT_BINARY_DIR}/config/shaders.hpp")

add_executable(triangle WIN32 "main.cpp")

target_link_libraries(triangle PUBLIC glfw)
if(NOT EMSCRIPTEN)
	target_link_libraries(triangle PUBLIC glad_33)
endif()
target_include_directories(triangle PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/config")
