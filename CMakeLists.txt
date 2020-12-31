cmake_minimum_required(VERSION 3.11)

project("gggllllgllglllglfw")

# find or get glfw if not on web
if(NOT EMSCRIPTEN)
	find_package(glfw3 3.3.2 CONFIG QUIET)
	if(NOT glfw3_FOUND)
		message(STATUS "No installed glfw found, getting from git")
		include(FetchContent)
		FetchContent_Declare(
			glfw3
			GIT_REPOSITORY https://github.com/glfw/glfw.git
			GIT_TAG 3.3.2
		)

		FetchContent_GetProperties(glfw3)
		if(NOT glfw3_POPULATED)
			set(FETCHCONTENT_QUIET NO)
			FetchContent_Populate(glfw3)

			set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
			set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
			set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

			add_subdirectory(${glfw3_SOURCE_DIR} ${glfw3_BINARY_DIR})
		endif()
	else()
		message(STATUS "Found installed glfw")
	endif()
endif()

# get glad2 branch of glad for `glad_add_library`
set(glad_TAG "1d7b6e9e908e32e3237caad55836134af92a32f6")
include(FetchContent)
FetchContent_Declare(
	glad
	GIT_REPOSITORY https://github.com/Dav1dde/glad.git
	GIT_TAG ${glad_TAG}
)

FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
	set(FETCHCONTENT_QUIET NO)
	FetchContent_Populate(glad)

	add_subdirectory("${glad_SOURCE_DIR}/cmake" "${glad_BINARY_DIR}")
endif()

if (CMAKE_HOST_WIN32)
add_custom_command(
	OUTPUT glad_installed
	COMMAND type nul >> glad_installed & copy glad_installed +,,
	COMMAND python -m pip install git+https://github.com/Dav1dde/glad.git@${glad_TAG}
)
else()
add_custom_command(
	OUTPUT glad_installed
	COMMAND touch glad_installed
	COMMAND python -m pip install git+https://github.com/Dav1dde/glad.git@${glad_TAG}
)
endif()

# install pip dependencies
add_custom_target(glad_install DEPENDS glad_installed)

if(EMSCRIPTEN)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
	glad_add_library(glad_33 REPRODUCIBLE API gles2=2.0)
else()
	glad_add_library(glad_33 REPRODUCIBLE API gl:core=3.3)
endif()

add_dependencies(glad_33 glad_install)

add_subdirectory("src")
