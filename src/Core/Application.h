#pragma once

#include "Window.h"
#include <memory>

class IEvent;

class Application {
public:
  static Application &Get();

  bool Init(const Window::Config &config);
  void Run();
  void Shutdown();
  void OnEvent(IEvent &e);

public:
  Application(const Application &) = delete;
  Application &operator=(const Application &) = delete;

private:
  Application() = default;

private:
  std::unique_ptr<Window> window_;
  bool running_ = true;
};

/*
class Layer {
public:
  virtual ~Layer() = default;
  virtual void OnAttach() {}
  virtual void OnUpdate(float dt) {}
  virtual void OnRender() = 0;
};

// Example Usage
class GameLayer : public Layer {
  void OnRender() override {
    // Draw your 3D Triangle here
  }
};
*/
