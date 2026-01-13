#pragma once

#include <string>

enum class EventType {
  None = 0,
  WindowClose,
  WindowResize,
  WindowIconify,
  WindowFrameBufferSize,
  KeyPressed,
  KeyReleased,
  KeyRepeat,
  KeyTyped,
  MouseButtonPressed,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled
};

namespace EventCategory {
enum Category {
  None = 0,
  Window = 1 << 0,     /**< Events related to the OS window */
  Input = 1 << 1,      /**< Any hardware input  */
  Keyboard = 1 << 2,   /**< Specific to keyboard button presses */
  Mouse = 1 << 3,      /**< Specific to mouse movement and scrolling */
  MouseButton = 1 << 4 /**< Specific to mouse button clicks */
};
}

class IEvent {
public:
  virtual ~IEvent() = default;

  bool Handled = false;

  virtual EventType GetType() const = 0;
  virtual int GetCategoryFlags() const = 0;
  virtual const char *GetName() const = 0;
  virtual std::string ToString() const { return GetName(); }
  bool IsInCategory(EventCategory::Category category) {
    return GetCategoryFlags() & category;
  }
};

#define MAKE_EVENT_CLASS_TYPE(type)                                            \
  static constexpr EventType GetStaticType() { return EventType::type; }       \
  virtual EventType GetType() const override { return GetStaticType(); }       \
  virtual const char *GetName() const override { return #type; }

#define MAKE_EVENT_CLASS_CATEGORY(category)                                    \
  virtual int GetCategoryFlags() const override { return category; }
