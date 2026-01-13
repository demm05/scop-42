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
  void PollEvents();
  void SwapBuffers();
  bool ShouldClose() const;

  void SetEventCallback(const EventCallbackFn &callback);

  void SetVSync(bool enabled);
  bool IsVSync() const;

  void SetTitle(std::string_view title);

  void Minimize();
  void Maximize();
  void Restore();
  void Close();
  bool IsVisible() const;
  void ToggleFullscreen();
  void SetFullScreen(bool enable);
  bool IsFullScreen() const;
  int Width() const;
  int Height() const;
  int FramebufferWidth() const;
  int FramebufferHeight() const;

  // bool IsKeyPressed(int keycode) const;
  // bool IsMouseButtonPressed(int button) const;
  // std::pair<float, float> GetMousePosition() const;

public:
  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;
  ~Window();

private:
  struct WindowData {
    EventCallbackFn EventCallback;
    std::string Title;
    int Width, Height;
    int FramebufferWidth, FramebufferHeight;
    int WindowedPosX, WindowedPosY, WindowedWidth, WindowedHeight;
    bool isFullScreen;
    bool VSync;
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
  static void WindowCloseCallback(GLFWwindow *);
  static void WindowIconifyCallback(GLFWwindow *, int iconified);
  static void WindowFrameBufferSizeCallback(GLFWwindow *, int, int);

private:
  GLFWwindow *windowHandle_ = nullptr;
  WindowData data_;
};
