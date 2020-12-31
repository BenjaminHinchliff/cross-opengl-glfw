# OpenGL GLFW All Platforms Example

an example of using opengl (es) with glfw that works on windows, linux, mac, and the web, with perhaps the most overenginneered cmake build I've ever done.

# Usage

I've included some compiled staticically linked binaries in the releases for windows, linux, and the web (via emscripten). I can't compile for Mac, but it should still work.

# Compilation

Make sure you have python (for glad2 w/o emscripten), cmake, your compiler of choice, and your OS' SDK installed.

run cmake (e.g. `cd <clone directory> && cmake . -B build && cmake --build build`).
It will automatically determine if glfw is installed on your system, or otherwise will clone and build it from the 3.3.2 release.
It will also clone the glad2 branch of glad and use cmake to build the appropriate opengl bindings (if needed). The executable can be found in the src directory.
