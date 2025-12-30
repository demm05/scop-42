#pragma once

#include "Layers/ILayer.hpp"
#include "Window.hpp"
#include <memory>

class IEvent;

class Application {
public:
  static Application &Get();

  bool Init(const Window::Config &config);
  void Run();
  void Stop();
  void Shutdown();
  void OnEvent(IEvent &e);

public:
  Application(const Application &) = delete;
  Application &operator=(const Application &) = delete;

private:
  Application() = default;
  static void errorCallback(int error, const char *description);

private:
  std::unique_ptr<Window> window_;
  std::unique_ptr<ILayer> gameLayer_;
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
