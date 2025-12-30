#pragma once

#include "IEvent.hpp"
#include "KeyCodes.hpp"

struct KeyEvent : public IEvent {
  KeyCode Code;
  int Mods;

  KeyEvent(KeyCode keycode, int mods) : Code(keycode), Mods(mods) {}
  MAKE_EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input);
};

struct KeyPressEvent : public KeyEvent {
  KeyPressEvent(KeyCode keycode, int mods) : KeyEvent(keycode, mods) {}
  MAKE_EVENT_CLASS_TYPE(KeyPressed);
};

struct KeyReleaseEvent : public KeyEvent {
  KeyReleaseEvent(KeyCode keycode, int mods) : KeyEvent(keycode, mods) {}
  MAKE_EVENT_CLASS_TYPE(KeyReleased);
};

struct KeyRepeatEvent : public KeyEvent {
  KeyRepeatEvent(KeyCode keycode, int mods) : KeyEvent(keycode, mods) {}
  MAKE_EVENT_CLASS_TYPE(KeyRepeat);
};
