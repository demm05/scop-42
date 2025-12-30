#pragma once

#include "ILayer.hpp"

class GameLayer : public ILayer {
public:
  void onAttach() override;
  void onDetach() override;
  void onUpdate() override;
  void onEvent(IEvent &) override;
};
