#pragma once

#include "IEvent.h"

struct WindowResizeEvent : public IEvent {
  unsigned int Width, Height;
  WindowResizeEvent(unsigned int width, unsigned int height)
      : Width(width), Height(height) {}

  MAKE_EVENT_CLASS_TYPE(WindowResize)
};

struct WindowCloseEvent : public IEvent {
  WindowCloseEvent() = default;
  MAKE_EVENT_CLASS_TYPE(WindowClose)
};
