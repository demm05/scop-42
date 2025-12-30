#include "Core/Application.hpp"
#include "Core/Window.hpp"

int main(void) {
  auto win_prop = Window::Config{
      .Title = "Scop", .Width = 1920, .Height = 1080, .VSync = true};

  auto &app = Application::Get();
  if (app.Init(win_prop)) {
    app.Run();
  };
  app.Shutdown();
}
