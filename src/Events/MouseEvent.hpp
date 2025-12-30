#pragma once

#include "IEvent.hpp"
#include "MouseCode.hpp"

struct MouseMovedEvent : public IEvent {
  float MouseX, MouseY;
  MouseMovedEvent(float x, float y) : MouseX(x), MouseY(y) {}

  MAKE_EVENT_CLASS_TYPE(MouseMoved);
  MAKE_EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input);
};

struct MouseButtonEvent : public IEvent {
  MouseCode Button;
  MouseButtonEvent(MouseCode button) : Button(button) {}
  MAKE_EVENT_CLASS_CATEGORY(EventCategory::MouseButton | EventCategory::Mouse |
                            EventCategory::Input);
};

struct MouseButtonPressedEvent : public MouseButtonEvent {
  MouseButtonPressedEvent(MouseCode button) : MouseButtonEvent(button) {}
  MAKE_EVENT_CLASS_TYPE(MouseButtonPressed)
};

struct MouseButtonReleasedEvent : public MouseButtonEvent {
  MouseButtonReleasedEvent(MouseCode button) : MouseButtonEvent(button) {}
  MAKE_EVENT_CLASS_TYPE(MouseButtonReleased)
};

struct MouseScrolledEvent : public IEvent {
  float XOffset, YOffset;
  MouseScrolledEvent(float xOffset, float yOffset)
      : XOffset(xOffset), YOffset(yOffset) {}

  MAKE_EVENT_CLASS_TYPE(MouseScrolled)
  MAKE_EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input);
};
