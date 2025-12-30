#pragma once

#include "Events/IEvent.hpp"

class ILayer {
public:
  virtual ~ILayer() = default;

  virtual void onAttach() {};
  virtual void onDetach() {};
  virtual void onUpdate() {};
  virtual void onEvent(IEvent &) {};
};
