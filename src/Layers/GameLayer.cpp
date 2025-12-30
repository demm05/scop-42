#include "Layers/GameLayer.hpp"
#include "Core/Application.hpp"
#include "Events/EventDispatcher.hpp"
#include "Events/KeyEvent.hpp"
#include "Platform/OpenGL/GLContext.hpp"
#include <print>

void GameLayer::onAttach() {}

void GameLayer::onDetach() {}

void GameLayer::onUpdate() {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void GameLayer::onEvent(IEvent &event) {
  EventDispatcher dispatcher(event);
  std::println("{}", event.GetName());

  dispatcher.Dispatch<KeyPressEvent>([](KeyPressEvent &e) {
    if (e.Code == KeyCode::Escape) {
      Application::Get().Stop();
      return true;
    }
    return false;
  });
}
