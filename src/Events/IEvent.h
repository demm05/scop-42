#pragma once

#include <string>

enum class EventType {
  None = 0,
  WindowClose,
  WindowResize,
  KeyPressed,
  KeyReleased,
  KeyRepeat,
  KeyTyped,
  MouseButtonPressed,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled
};

class IEvent {
public:
  virtual ~IEvent() = default;

  bool Handled = false;

  virtual EventType GetType() const = 0;
  virtual const char *GetName() const = 0;
  virtual std::string ToString() const { return GetName(); }
};

#define MAKE_EVENT_CLASS_TYPE(type)                                            \
  static constexpr EventType GetStaticType() { return EventType::type; }       \
  virtual EventType GetType() const override { return GetStaticType(); }       \
  virtual const char *GetName() const override { return #type; }

class EventDispatcher {
public:
  EventDispatcher(IEvent &event) : m_Event(event) {}

  template <typename T, typename F> bool Dispatch(const F &func) {
    if (m_Event.GetType() == T::GetStaticType() && !m_Event.Handled) {
      m_Event.Handled |= func(static_cast<T &>(m_Event));
      return true;
    }
    return false;
  }

private:
  IEvent &m_Event;
};
