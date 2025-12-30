#include "Layers/GameLayer.hpp"
#include "Core/Application.hpp"
#include "Core/Logger.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/KeyEvent.hpp"
#include "Platform/OpenGL/GLContext.hpp"
#include <expected>
#include <iostream>
#include <math.h>
#include <print>
#include <string_view>

std::expected<unsigned int, std::string>
GameLayer::createShader(std::string_view source, unsigned int type) {
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

std::expected<unsigned int, std::string>
GameLayer::createProgram(unsigned int vShader, unsigned int fShader) {
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
layout (location = 1) in vec3 aColor; // Attribute 1: Color

out vec3 vColor; // Output to Fragment Shader

uniform mat4 u_Rotation; // The rotation matrix we send every frame

void main() {
    // Note: OpenGL uses column-major matrices, so order is u_Rotation * Position
    gl_Position = u_Rotation * vec4(aPos, 1.0);
    vColor = aColor;
}
)";
const char *fragmentShaderSource = R"(
#version 330 core
layout (location = 0) out vec4 FragColor;

in vec3 vColor; // Input from Vertex Shader

void main() {
    FragColor = vec4(vColor, 1.0);
}
)";

void GameLayer::onAttach() {
  auto vShader = createShader(vertexShaderSource, GL_VERTEX_SHADER);
  if (!vShader) {
    CORE_ERROR(vShader.error());
    return;
  }
  auto fShader = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
  if (!fShader) {
    CORE_ERROR(fShader.error());
    return;
  }
  auto x = createProgram(*vShader, *fShader);
  if (!x) {
    CORE_ERROR(x.error());
    return;
  }
  m_ShaderProgram = *x;

  // . Define Vertex Data (Position X,Y,Z + Color R,G,B)
  // This is called "Interleaved Data" because pos and color are next to each
  // other
  float vertices[] = {
      // Positions         // Colors
      -0.6f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f, // Top (Red)
      0.6f,  -0.4f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom Right (Green)
      0.f,   0.6f,  0.0f, 0.0f, 0.0f, 1.0f  // Bottom Left (Blue)
  };

  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);

  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // --- STRIDE AND OFFSET EXPLAINED ---
  // Each vertex now has 6 floats (3 pos, 3 color).
  // Stride = 6 * sizeof(float)

  // Position Attribute (Location 0)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Color Attribute (Location 1)
  // Offset = 3 * sizeof(float) (because color starts after the 3 position
  // floats)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

void GameLayer::onUpdate() {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(m_ShaderProgram);

  // --- ROTATION LOGIC ---
  // We create a rotation matrix manually (Z-axis rotation)
  float time = (float)glfwGetTime();
  float s = std::sin(time);
  float c = std::cos(time);

  // A 4x4 Rotation Matrix around the Z axis
  float rotationMatrix[16] = {c,    s,    0.0f, 0.0f, -s,   c,    0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

  // Find the "u_Rotation" variable in the shader
  int location = glGetUniformLocation(m_ShaderProgram, "u_Rotation");
  // Upload the matrix to the GPU
  glUniformMatrix4fv(location, 1, GL_FALSE, rotationMatrix);

  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
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
