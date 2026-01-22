#include "Layers/GameLayer.hpp"
#include "Core/Application.hpp"
#include "Core/Logger.hpp"
#include "Events/Events.hpp"
#include "Platform/OpenGL/GLContext.hpp"
#include <expected>
#include <string_view>
#include <vector>

namespace {

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

void main() {
    gl_Position = aPos;
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.5f, 0.2f, 1.0f);
}
)";

} // namespace

void GameLayer::onAttach() {
  auto shaderResult =
      Shader::FromSource(vertexShaderSource, fragmentShaderSource);
  if (!shaderResult) {
    CORE_ERROR("Failed to load shader: {}", shaderResult.error());
    Application::Get().Stop();
    return;
  }
  m_Shader = std::make_unique<Shader>(std::move(*shaderResult));

  std::vector<Vertex> vertices = {
      {{-1.0f, 0.0f, -0.2f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
      {{0.0f, 1.0f, -0.2f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
      {{1.0f, 0.0f, -0.2f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
      {{0.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}};

  std::vector<uint32_t> indices = {0, 1, 2};

  m_Mesh = std::make_unique<Mesh>(vertices, indices);
}

void GameLayer::onUpdate() {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  if (m_Shader) {
    m_Shader->Bind();
  }

  if (m_Mesh) {
    m_Mesh->Draw();
  }
}

void GameLayer::onDetach() {
  m_Mesh.reset();
  m_Shader.reset();
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
