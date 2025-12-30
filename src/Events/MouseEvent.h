#pragma once

#include "IEvent.h"
#include "MouseCode.h"

struct MouseMovedEvent : public IEvent {
  float MouseX, MouseY;
  MouseMovedEvent(float x, float y) : MouseX(x), MouseY(y) {}

  MAKE_EVENT_CLASS_TYPE(MouseMoved);
};

struct MouseButtonEvent : public IEvent {
  MouseCode Button;
  MouseButtonEvent(MouseCode button) : Button(button) {}
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
};
