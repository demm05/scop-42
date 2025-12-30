#pragma once

#include "IEvent.hpp"

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

  template <typename F>
  bool DispatchCategory(EventCategory::Category category, const F &func) {
    if (m_Event.IsInCategory(category) && !m_Event.Handled) {
      m_Event.Handled |= func(m_Event);
      return true;
    }
    return false;
  }

private:
  IEvent &m_Event;
};
