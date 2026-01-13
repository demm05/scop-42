#include "Core/Application.hpp"
#include "Core/Logger.hpp"
#include "Core/Window.hpp"
#include "Events/Events.hpp"
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

  m_ViewportWidth = window_->FramebufferWidth();
  m_ViewportHeight = window_->FramebufferHeight();
  glViewport(0, 0, static_cast<int>(m_ViewportWidth),
             static_cast<int>(m_ViewportHeight));
  CORE_TRACE("Initial viewport size: {}x{} (Logical: {}x{})", 
             m_ViewportWidth, m_ViewportHeight, config.Width, config.Height);
  CORE_INFO("Application initialized successfully.");
  gameLayer_ = std::make_unique<GameLayer>();
  gameLayer_->onAttach();
  return true;
}

void Application::Run() {
  CORE_INFO("Starting main application loop.");
  while (running_ && !window_->ShouldClose()) {
    window_->PollEvents();
    if (m_ViewportDirty) {
      CORE_TRACE("Updating glViewport to {}x{}", m_ViewportWidth, m_ViewportHeight);
      glViewport(0, 0, static_cast<int>(m_ViewportWidth),
                 static_cast<int>(m_ViewportHeight));
      m_ViewportDirty = false;
    }
    gameLayer_->onUpdate();
    window_->SwapBuffers();
  }
  CORE_INFO("Main application loop terminated.");
}

void Application::Stop() {
  running_ = false;
  window_->Close();
}

void Application::Shutdown() {
  CORE_TRACE("Shutting down Application...");
  gameLayer_->onDetach();
  window_.reset();
  glfwTerminate();
  CORE_INFO("Application shutdown complete.");
}

void Application::OnEvent(IEvent &event) {
  EventDispatcher dispatcher(event);

  dispatcher.Dispatch<WindowCloseEvent>([this](auto &) {
    Stop();
    return true;
  });

  dispatcher.Dispatch<WindowResizeEvent>([](WindowResizeEvent &e) {
    CORE_TRACE("{}: {}x{}", e.ToString(), e.Width, e.Height);
    return true;
  });

  dispatcher.Dispatch<WindowFrameBufferSizeEvent>(
      [this](WindowFrameBufferSizeEvent &e) {
        CORE_TRACE("{}: {}x{}", e.ToString(), e.Width, e.Height);
        m_ViewportWidth = e.Width;
        m_ViewportHeight = e.Height;
        m_ViewportDirty = true;
        return true;
      });

  dispatcher.Dispatch<KeyPressEvent>([this](KeyPressEvent &e) {
    // clang-format off
    switch (e.Code) {
      case KeyCode::M: window_->Maximize(); break;
      case KeyCode::H: window_->Minimize(); break;
      case KeyCode::F11: window_->ToggleFullscreen(); break;
      default: return false;
    }
    // clang-format on
    return true;
  });

  if (!event.Handled) {
    gameLayer_->onEvent(event);
  }
}

void Application::errorCallback(int error, const char *description) {
  CORE_ERROR("GLFW({}): {}", error, description);
}
