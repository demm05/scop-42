#include "Layers/GameLayer.hpp"
#include "Core/Application.hpp"
#include "Core/Logger.hpp"
#include "Events/Events.hpp"
#include "Platform/OpenGL/GLContext.hpp"
#include <expected>
#include <math.h>
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

  std::vector<Vertex> vertices = {
      {{-1.0f, 0.0f, -0.2f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
      {{0.0f, 1.0f, -0.2f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
      {{1.0f, 0.0f, -0.2f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
      {{0.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}};

  std::vector<uint32_t> indices = {0, 1, 2};

  m_Mesh = std::make_unique<Mesh>(vertices, indices);
}

void GameLayer::onUpdate() {
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(m_ShaderProgram);
  if (m_Mesh) {
    m_Mesh->Draw();
  }
}

void GameLayer::onDetach() {
  m_Mesh.reset();
  glDeleteProgram(m_ShaderProgram);
}

void GameLayer::onEvent(IEvent &event) {
  EventDispatcher dispatcher(event);

  dispatcher.Dispatch<KeyPressEvent>([](KeyPressEvent &e) {
    if (e.Code == KeyCode::Escape) {
      Application::Get().Stop();
      return true;
    }
    return false;
  });
}
