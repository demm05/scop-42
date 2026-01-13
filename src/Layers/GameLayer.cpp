#include "Layers/GameLayer.hpp"
#include "Core/Application.hpp"
#include "Core/Logger.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/KeyEvent.hpp"
#include "Platform/OpenGL/GLContext.hpp"
#include <expected>
#include <math.h>
#include <print>
#include <string_view>

std::expected<unsigned int, std::string> createShader(std::string_view source,
                                                      unsigned int type) {
  unsigned int shader = glCreateShader(type);
  const char *src = source.data();
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[1024];
    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
    glDeleteShader(shader);
    return std::unexpected(std::string(infoLog));
  }
  return shader;
}

std::expected<unsigned int, std::string> createProgram(unsigned int vShader,
                                                       unsigned int fShader) {
  unsigned int program = glCreateProgram();
  glAttachShader(program, vShader);
  glAttachShader(program, fShader);
  glLinkProgram(program);

  int success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[1024];
    glGetProgramInfoLog(program, 1024, NULL, infoLog);
    glDeleteProgram(program);
    return std::unexpected(std::string(infoLog));
  }
  return program;
}

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;   // Attribute 0: Position

void main() {
    gl_Position = vec4(aPos, 1.0);
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
layout (location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.5f, 0.2f, 1.0f);
}
)";

void GameLayer::onAttach() {
  auto vShader = createShader(vertexShaderSource, GL_VERTEX_SHADER);
  if (!vShader) {
    CORE_ERROR(vShader.error());
    Application::Get().Stop();
    return;
  }
  auto fShader = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
  if (!fShader) {
    CORE_ERROR(fShader.error());
    Application::Get().Stop();
    return;
  }
  auto x = createProgram(*vShader, *fShader);
  if (!x) {
    CORE_ERROR(x.error());
    Application::Get().Stop();
    return;
  }
  m_ShaderProgram = *x;

  float vertices[] = {
      0.5f,  0.5f,  0.0f, // top right
      0.5f,  -0.5f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f  // top left
  };
  unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  unsigned int EBO;

  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
  glGenBuffers(1, &EBO);

  // 1. bind Vertex Array Object
  glBindVertexArray(m_VAO);
  // 2. copy our vertices array in a vertex buffer for OpenGL to use
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // 3. copy our index array in a element buffer for OpenGL to use
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  // 4. then set the vertex attributes pointers
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
}

void GameLayer::onUpdate() {
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(m_ShaderProgram);
  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
  // glBindVertexArray(0);
}
void GameLayer::onDetach() {
  glDeleteVertexArrays(1, &m_VAO);
  glDeleteBuffers(1, &m_VBO);
  glDeleteProgram(m_ShaderProgram);
}

void GameLayer::onEvent(IEvent &event) {
  EventDispatcher dispatcher(event);
  std::println("{}", event.GetName());

  dispatcher.Dispatch<KeyPressEvent>([](KeyPressEvent &e) {
    if (e.Code == KeyCode::Escape) {
      Application::Get().Stop();
      return true;
    }
    return false;
  });
}
