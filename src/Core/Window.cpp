#include "Window.hpp"
#include "Core/Logger.hpp"
#include "Events/Events.hpp"
#include "Platform/OpenGL/GLContext.hpp"
#include "WindowExceptions.hpp"
#include "sanatizer_suppresions.h" // IWYU pragma: keep

namespace {

static KeyCode GLFWToEngineKey(int glfwKey) {
  switch (glfwKey) {
  case GLFW_KEY_TAB:
    return KeyCode::Tab;
  case GLFW_KEY_LEFT:
    return KeyCode::Left;
  default:
    return static_cast<KeyCode>(glfwKey);
  }
}
static int GLFWToEngineMods(int glfwMods) {
  int mods = KeyMod::None;
  if (glfwMods & GLFW_MOD_SHIFT)
    mods |= KeyMod::Shift;
  if (glfwMods & GLFW_MOD_CONTROL)
    mods |= KeyMod::Control;
  if (glfwMods & GLFW_MOD_ALT)
    mods |= KeyMod::Alt;
  if (glfwMods & GLFW_MOD_SUPER)
    mods |= KeyMod::Super;
  return mods;
}

} // namespace

Window::Window(const Window::Config &config) {
  data_ = {};
  init(config);
}
Window::~Window() { shutDown(); }

void Window::init(const Config &config) {
  data_.Title = config.Title;
  data_.Width = config.Width;
  data_.WindowedWidth = data_.Width;
  data_.Height = config.Height;
  data_.WindowedHeight = data_.Height;
  data_.VSync = config.VSync;

  CORE_TRACE("Window: {}:{}", data_.Width, data_.Height);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.OpenGLVersionMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.OpenGLVersionMinor);
  if (config.UseCoreProfile) {
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef PLATFORM_MACOS
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
  }

  if (config.CustomTitlebar) {
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  }

  windowHandle_ = glfwCreateWindow(data_.Width, data_.Height,
                                   config.Title.c_str(), nullptr, nullptr);

  if (!windowHandle_) {
    const char *description;
    int error = glfwGetError(&description);

    if (error == GLFW_NOT_INITIALIZED) {
      throw Exceptions::GlfwInitializationFailed();
    }
    throw Exceptions::WindowCreationFailed(data_.Title, data_.Width,
                                           data_.Height);
  }

  glfwGetWindowSize(windowHandle_, &data_.Width, &data_.Height);
  glfwGetFramebufferSize(windowHandle_, &data_.FramebufferWidth,
                         &data_.FramebufferHeight);
  glfwMakeContextCurrent(windowHandle_);
  glfwSetWindowUserPointer(windowHandle_, &data_);
  SetVSync(data_.VSync);

  glfwSetWindowUserPointer(windowHandle_, this);
  glfwSetKeyCallback(windowHandle_, KeyCallback);
  glfwSetMouseButtonCallback(windowHandle_, MouseButtonCallback);
  glfwSetCursorPosCallback(windowHandle_, MouseMoveCallback);
  glfwSetScrollCallback(windowHandle_, ScrollCallback);
  glfwSetWindowSizeCallback(windowHandle_, WindowResizeCallback);
  glfwSetWindowCloseCallback(windowHandle_, WindowCloseCallback);
  glfwSetWindowIconifyCallback(windowHandle_, WindowIconifyCallback);
  glfwSetFramebufferSizeCallback(windowHandle_, WindowFrameBufferSizeCallback);
}

void Window::shutDown() {
  if (windowHandle_) {
    glfwDestroyWindow(windowHandle_);
    windowHandle_ = nullptr;
  }
}

void Window::SetEventCallback(const EventCallbackFn &callback) {
  data_.EventCallback = callback;
}

void Window::SetVSync(bool enabled) {
  if (enabled)
    glfwSwapInterval(1);
  else
    glfwSwapInterval(0);
  data_.VSync = enabled;
}

void Window::SetTitle(std::string_view title) {
  glfwSetWindowTitle(windowHandle_, title.data());
}

bool Window::IsVSync() const { return data_.VSync; }
void Window::Minimize() { glfwIconifyWindow(windowHandle_); }
void Window::Maximize() { glfwMaximizeWindow(windowHandle_); }
void Window::Restore() { glfwRestoreWindow(windowHandle_); }
void Window::Close() { glfwSetWindowShouldClose(windowHandle_, 1); }
bool Window::ShouldClose() const {
  return glfwWindowShouldClose(windowHandle_);
}
bool Window::IsVisible() const {
  return glfwGetWindowAttrib(windowHandle_, GLFW_VISIBLE) &&
         !glfwGetWindowAttrib(windowHandle_, GLFW_ICONIFIED);
}

void Window::OnUpdate() {
  PollEvents();
  SwapBuffers();
}
void Window::PollEvents() { glfwPollEvents(); }
void Window::SwapBuffers() { glfwSwapBuffers(windowHandle_); }

// clang-format off
void Window::SetFullScreen(bool enable) {
  GLFWmonitor *monitor = glfwGetWindowMonitor(windowHandle_);
  bool currentlyFull = (monitor != nullptr);

  if (currentlyFull == enable)
    return;

  if (enable) {
    glfwGetWindowSize(windowHandle_, &data_.WindowedWidth, &data_.WindowedHeight);
    if (glfwGetPlatform() != GLFW_PLATFORM_WAYLAND) {
      glfwGetWindowPos(windowHandle_, &data_.WindowedPosX, &data_.WindowedPosY);
    }
    GLFWmonitor *targetMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(targetMonitor);

    glfwSetWindowMonitor(windowHandle_, targetMonitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
    data_.isFullScreen = true;

  } else {
    int targetX = data_.WindowedPosX;
    int targetY = data_.WindowedPosY;

    if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND) {
      glfwSetWindowMonitor(windowHandle_, nullptr, 0, 0, data_.WindowedWidth, data_.WindowedHeight, 0);
    } else {
      glfwSetWindowMonitor(windowHandle_, nullptr, targetX, targetY, data_.WindowedWidth, data_.WindowedHeight, 0);
    }
    data_.isFullScreen = false;
  }
}
// clang-format on

void Window::ToggleFullscreen() { SetFullScreen(!data_.isFullScreen); }
int Window::Width() const { return data_.Width; }
int Window::Height() const { return data_.Height; }
int Window::FramebufferWidth() const { return data_.FramebufferWidth; }
int Window::FramebufferHeight() const { return data_.FramebufferHeight; }

#define GET_WINDOW_INSTANCE                                                    \
  Window *thisWindow =                                                         \
      static_cast<Window *>(glfwGetWindowUserPointer(window));                 \
  if (!thisWindow) {                                                           \
    return;                                                                    \
  }

void Window::KeyCallback(GLFWwindow *window, int key, int, int action,
                         int mode) {
  GET_WINDOW_INSTANCE;
  KeyCode key_code = GLFWToEngineKey(key);
  int mods = GLFWToEngineMods(mode);

  switch (action) {
  case GLFW_PRESS: {
    KeyPressEvent event(key_code, mods);
    if (thisWindow->data_.EventCallback)
      thisWindow->data_.EventCallback(event);
    return;
  }
  case GLFW_RELEASE: {
    KeyReleaseEvent event(key_code, mods);
    if (thisWindow->data_.EventCallback)
      thisWindow->data_.EventCallback(event);
    return;
  }
  case GLFW_REPEAT: {
    KeyRepeatEvent event(key_code, mods);
    if (thisWindow->data_.EventCallback)
      thisWindow->data_.EventCallback(event);
    break;
  }
  }
}

void Window::MouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
  GET_WINDOW_INSTANCE;
  MouseMovedEvent event(static_cast<float>(xpos), static_cast<float>(ypos));
  if (thisWindow->data_.EventCallback)
    thisWindow->data_.EventCallback(event);
}

void Window::MouseButtonCallback(GLFWwindow *window, int button, int action,
                                 int) {
  GET_WINDOW_INSTANCE
  MouseCode mouse_code = static_cast<MouseCode>(button);

  if (action == GLFW_PRESS) {
    MouseButtonPressedEvent event(mouse_code);
    if (thisWindow->data_.EventCallback)
      thisWindow->data_.EventCallback(event);
  } else {
    MouseButtonReleasedEvent event(mouse_code);
    if (thisWindow->data_.EventCallback)
      thisWindow->data_.EventCallback(event);
  }
}
void Window::ScrollCallback(GLFWwindow *window, double xoffset,
                            double yoffset) {
  Window *instance = static_cast<Window *>(glfwGetWindowUserPointer(window));
  if (!instance)
    return;

  MouseScrolledEvent event((float)xoffset, (float)yoffset);
  if (instance->data_.EventCallback)
    instance->data_.EventCallback(event);
}

void Window::WindowResizeCallback(GLFWwindow *window, int width, int height) {
  GET_WINDOW_INSTANCE
  thisWindow->data_.Width = width;
  thisWindow->data_.Height = height;
  WindowResizeEvent event((unsigned int)width, (unsigned int)height);
  if (thisWindow->data_.EventCallback)
    thisWindow->data_.EventCallback(event);
}

void Window::WindowCloseCallback(GLFWwindow *window) {
  GET_WINDOW_INSTANCE;
  WindowCloseEvent event;
  thisWindow->data_.EventCallback(event);
}

void Window::WindowIconifyCallback(GLFWwindow *window, int /*iconified*/) {
  GET_WINDOW_INSTANCE;
  WindowIconifyEvent event;
  thisWindow->data_.EventCallback(event);
}

void Window::WindowFrameBufferSizeCallback(GLFWwindow *window, int width,
                                           int height) {
  GET_WINDOW_INSTANCE;
  thisWindow->data_.FramebufferWidth = width;
  thisWindow->data_.FramebufferHeight = height;
  WindowFrameBufferSizeEvent event(static_cast<unsigned int>(width),
                                   static_cast<unsigned int>(height));
  thisWindow->data_.EventCallback(event);
}

#undef GET_WINDOW_INSTANCE
