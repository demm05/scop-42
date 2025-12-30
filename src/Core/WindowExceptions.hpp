#pragma once

#include <format>
#include <stdexcept>
#include <string>

namespace Exceptions {

class WindowException : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class GlfwInitializationFailed : public WindowException {
public:
  GlfwInitializationFailed()
      : WindowException("GLFW failed to initialize (or was not initialized by "
                        "Application).") {}
};

class WindowCreationFailed : public WindowException {
public:
  WindowCreationFailed(const std::string &title, int width, int height)
      : WindowException(
            std::format("Failed to create GLFW window: '{}' ({}x{})", title,
                        width, height)) {}
};

} // namespace Exceptions
