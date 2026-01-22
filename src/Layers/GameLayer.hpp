#pragma once

#include "Layers/ILayer.hpp"
#include "Renderer/Mesh.hpp"
#include "Renderer/Shader.hpp"
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
  std::unique_ptr<Shader> m_Shader;
  std::unique_ptr<Mesh> m_Mesh;
};
