#pragma once

#include "Layers/ILayer.hpp"
#include <expected>
#include <string>
#include <string_view>

class GameLayer : public ILayer {
public:
  GameLayer() = default;
  virtual ~GameLayer() = default;

  virtual void onAttach() override;
  virtual void onDetach() override;
  virtual void onUpdate() override;
  virtual void onEvent(IEvent &event) override;

private:
  // Helper functions for cleaner code
  std::expected<unsigned int, std::string> createShader(std::string_view source,
                                                        unsigned int type);
  std::expected<unsigned int, std::string>
  createProgram(unsigned int vertexShader, unsigned int fragmentShader);

private:
  unsigned int m_ShaderProgram = 0;
  unsigned int m_VAO = 0;
  unsigned int m_VBO = 0;
};
