#include "Layers/GameLayer.hpp"
#include "Core/Application.hpp"
#include "Core/Logger.hpp"
#include "Events/Events.hpp"
#include "Platform/OpenGL/GLContext.hpp"
#include "Renderer/Model.hpp"
#include <expected>
#include <memory>
#include <string_view>

namespace {

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 u_Model;

void main() {
    gl_Position = u_Model * aPos;
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
  auto model = Model::Load("resources/42.obj");
  if (!model) {
    CORE_ERROR("Failed to load model: {}", model.error());
    Application::Get().Stop();
    return;
  }
  m_Model = std::make_unique<Model>(std::move(*model));

  auto shaderResult =
      Shader::FromSource(vertexShaderSource, fragmentShaderSource);
  if (!shaderResult) {
    CORE_ERROR("Failed to load shader: {}", shaderResult.error());
    Application::Get().Stop();
    return;
  }
  m_Shader = std::make_unique<Shader>(std::move(*shaderResult));

  // Enable depth testing for 3D rendering
  glEnable(GL_DEPTH_TEST);
}

void GameLayer::onUpdate() {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  // Clear both color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (m_Shader) {
    m_Shader->Bind();

    // Create a transformation matrix: Scale down and Rotate
    // OBJ files are often large, so we scale it to fit Clip Space (-1 to 1)
    static float rotation = 0.0f;
    rotation += 0.01f;

    Mat4 scale = Mat4::Scale(Vec3(0.05f, 0.05f, 0.05f));
    Mat4 rotate = Mat4::RotateY(rotation);
    Mat4 transform = rotate * scale;

    m_Shader->SetUniformMat4("u_Model", transform);
  }

  if (m_Model) {
    for (const auto &mesh : m_Model->GetMeshes()) {
      mesh.Draw();
    }
  }
}

void GameLayer::onDetach() {
  m_Model.reset();
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
