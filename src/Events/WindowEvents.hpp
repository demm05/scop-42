#pragma once

#include "IEvent.hpp"

struct WindowResizeEvent : public IEvent {
  unsigned int Width, Height;
  WindowResizeEvent(unsigned int width, unsigned int height)
      : Width(width), Height(height) {}

  MAKE_EVENT_CLASS_TYPE(WindowResize)
  MAKE_EVENT_CLASS_CATEGORY(EventCategory::Window);
};

struct WindowFrameBufferSizeEvent : public IEvent {
  unsigned int Width, Height;
  WindowFrameBufferSizeEvent(unsigned int width, unsigned int height)
      : Width(width), Height(height) {}

  MAKE_EVENT_CLASS_TYPE(WindowFrameBufferSize)
  MAKE_EVENT_CLASS_CATEGORY(EventCategory::Window);
};

struct WindowCloseEvent : public IEvent {
  WindowCloseEvent() = default;
  MAKE_EVENT_CLASS_TYPE(WindowClose)
  MAKE_EVENT_CLASS_CATEGORY(EventCategory::Window);
};

struct WindowIconifyEvent : public IEvent {
  WindowIconifyEvent() = default;
  MAKE_EVENT_CLASS_TYPE(WindowIconify)
  MAKE_EVENT_CLASS_CATEGORY(EventCategory::Window);
};
