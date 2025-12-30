#pragma once

#include <functional>
#include <string>

class IEvent;
struct GLFWwindow;

class Window {
public:
  struct Config {
    std::string Title = "Scop";
    int Width = 1280;
    int Height = 720;
    bool VSync = true;
    bool CustomTitlebar = false;
    bool Fullscreen = false;
    int OpenGLVersionMajor = 4;
    int OpenGLVersionMinor = 6;
    bool UseCoreProfile = true;
  };
  using EventCallbackFn = std::function<void(IEvent &)>;

public:
  Window(const Config &config);

  void OnUpdate();
  bool ShouldClose() const;

  void SetEventCallback(const EventCallbackFn &callback);

  void SetVSync(bool enabled);
  bool IsVSync() const;

  void SetTitle(std::string_view title);

  void Minimize();
  void Maximize();
  void Restore();
  void Close();

  // bool IsKeyPressed(int keycode) const;
  // bool IsMouseButtonPressed(int button) const;
  // std::pair<float, float> GetMousePosition() const;

public:
  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;
  ~Window();

private:
  struct WindowData {
    std::string Title;
    int Width, Height;
    bool VSync;
    EventCallbackFn EventCallback;
  };

private:
  void init(const Config &config);
  void shutDown();

  static void KeyCallback(GLFWwindow *, int key, int scancode, int action,
                          int mode);
  static void MouseMoveCallback(GLFWwindow *, double xpos, double ypos);
  static void MouseButtonCallback(GLFWwindow *, int button, int action,
                                  int mods);
  static void ScrollCallback(GLFWwindow *, double xoffset, double yoffset);
  static void WindowResizeCallback(GLFWwindow *, int width, int height);

private:
  GLFWwindow *windowHandle_ = nullptr;
  WindowData data_;
};
