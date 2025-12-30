#include "Core/Application.hpp"
#include "Core/Logger.hpp"
#include "Core/Window.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/IEvent.hpp"
#include "Events/WindowEvents.hpp"
#include "Layers/GameLayer.hpp"
#include "Platform/OpenGL/GLContext.hpp"
#include <exception>
#include <memory>

Application &Application::Get() {
  static Application instance;
  return instance;
}

bool Application::Init(const Window::Config &config) {
  Logger::init();
  glfwSetErrorCallback(errorCallback);
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

  gameLayer_ = std::make_unique<GameLayer>();
  gameLayer_->onAttach();
  return true;
}

void Application::Run() {
  while (running_ && !window_->ShouldClose()) {
    gameLayer_->onUpdate();
    window_->OnUpdate();
  }
}

void Application::Stop() { running_ = false; }

void Application::Shutdown() {
  gameLayer_->onDetach();
  window_.reset();
  glfwTerminate();
}

void Application::OnEvent(IEvent &event) {
  EventDispatcher dispatcher(event);

  dispatcher.Dispatch<WindowCloseEvent>([this](auto &) {
    Stop();
    return true;
  });

  if (!event.Handled) {
    gameLayer_->onEvent(event);
  }
}

void Application::errorCallback(int error, const char *description) {
  CORE_ERROR("GLFW({}): {}", error, description);
}
