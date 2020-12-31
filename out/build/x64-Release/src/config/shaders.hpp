#ifndef SHADERS_HPP
#define SHADERS_HPP

namespace sources {
const char *const triangle_vertex = R"(#version 330 core

layout (location = 0) in vec3 aPos;

void main()
{
	gl_Position = vec4(aPos, 1.0);
}
)";
const char *const triangle_fragment = R"(#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)";
}

#endif // !SHADERS_HPP
