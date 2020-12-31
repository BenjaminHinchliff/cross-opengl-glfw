#include <array>
#include <iostream>

#ifdef _WIN32
#include "windows.h"
#endif // _WIN32
#ifdef EMSCRIPTEN
#include "emscripten.h"
#define GLFW_INCLUDE_ES2
#else
#include "glad/gl.h"
#endif // EMSCRIPTEN
#include "GLFW/glfw3.h"

#include "shaders.hpp"

constexpr std::array<float, 9> vertices{-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                                        0.0f,  0.0f,  0.5f, 0.0f};

struct FrameState {
  GLFWwindow *window;
  int shaderProgram;
#ifndef EMSCRIPTEN
  uint32_t VAO;
#endif // !EMSCRIPTEN
};

void draw(void *voidState);

#ifdef _WIN32
INT WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PSTR, _In_ INT) {
#else
int main() {
#endif // _WIN32
  if (!glfwInit()) {
    std::cerr << "Failed to init glfw" << '\n';
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_API, true);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif // __APPLE__

  GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create glfw window" << '\n';
    return 1;
  }

  glfwMakeContextCurrent(window);

#ifndef EMSCRIPTEN
  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << '\n';
    return 1;
  }
#endif // EMSCRIPTEN

  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &sources::triangle_vertex, nullptr);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cerr << "Vertex shader compilation failed: " << infoLog << '\n';
    return 1;
  }

  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &sources::triangle_fragment, nullptr);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    std::cerr << "Fragment shader compilation failed: " << infoLog << '\n';
    return 1;
  }

  int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

#ifndef EMSCRIPTEN
  uint32_t VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
#endif // EMSCRIPTEN

  uint32_t VBO;
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(),
               vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float),
                        static_cast<void *>(0));
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
#ifndef EMSCRIPTEN
  glBindVertexArray(0);
#endif // EMSCRIPTEN

  FrameState state{
    window,
    shaderProgram,
#ifndef EMSCRIPTEN
    VAO
#endif // EMSCRIPTEN
  };

  #ifdef EMSCRIPTEN
  emscripten_set_main_loop_arg(draw, &state, 0, true);
  #else
  while (!glfwWindowShouldClose(window)) {
    draw(&state);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  #endif // EMSCRIPTEN

  glfwTerminate();
  return 0;
}

void draw(void *voidState) {
  FrameState *state = reinterpret_cast<FrameState *>(voidState);
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(state->shaderProgram);
#ifndef EMSCRIPTEN
  glBindVertexArray(state->VAO);
#endif // EMSCRIPTEN
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
