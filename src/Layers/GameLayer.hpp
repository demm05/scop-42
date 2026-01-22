#pragma once

#include "Layers/ILayer.hpp"
#include "Renderer/Mesh.hpp"
#include <memory>

class GameLayer : public ILayer {
public:
  GameLayer() = default;
  virtual ~GameLayer() = default;

  virtual void onAttach() override;
  virtual void onDetach() override;
  virtual void onUpdate() override;
  virtual void onEvent(IEvent &event) override;

private:
  unsigned int m_ShaderProgram = 0;
  std::unique_ptr<Mesh> m_Mesh;
};
