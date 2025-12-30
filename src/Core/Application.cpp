#include "Window.h"
#include <exception>
#include <print>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Application.h"
#include "Events/IEvent.h"
#include "Logger.h"
#include "sanatizer_suppresions.h"

namespace {
static void error_callback(int error, const char *description) {
  CORE_ERROR("GLFW({}): {}", error, description);
}
} // namespace

Application &Application::Get() {
  static Application instance;
  return instance;
}

bool Application::Init(const Window::Config &config) {
  Logger::init();
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    return false;
  }

  try {
    window_ = std::make_unique<Window>(config);
    window_->SetEventCallback([this](IEvent &e) { OnEvent(e); });
  } catch (const std::exception &e) {
    CORE_ERROR(e.what());
    return false;
  }

  if (!gladLoadGL(glfwGetProcAddress)) {
    CORE_ERROR("Failed to load OpenGL using glad");
    return false;
  }

  return true;
}

void Application::Run() {
  while (running_ && !window_->ShouldClose()) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    window_->OnUpdate();
  }
}

void Application::Shutdown() {
  window_.reset();
  glfwTerminate();
}

void Application::OnEvent(IEvent &e) { std::println("{}", e.GetName()); }
