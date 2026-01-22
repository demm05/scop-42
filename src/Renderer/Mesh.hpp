#pragma once

#include "Math/Math.hpp"
#include <vector>

/** @struct Vertex
 * @brief Basic layout of a single point in 3D space.
 */
struct Vertex {
  Vec4 Position;  /**< x, y, z coordinates */
  Vec3 Normal;    /**< Surface direction for lighting */
  Vec2 TexCoords; /**< u, v coordinates for mapping textures */
};

/**
 * @class Mesh
 * @brief Manages individual GPU buffers (VAO/VBO/EBO) for a piece of geometry.
 * * A Mesh is a sub-part of a Model. It contains the raw vertex data and
 * knows how to draw itself once its Material is bound.
 */
class Mesh {
public:
  /**
   * @brief Constructs a mesh and uploads data to GPU buffers.
   * @param vertices List of Vertex structs.
   * @param indices List of indices for triangle construction.
   */
  Mesh(const std::vector<Vertex> &vertices,
       const std::vector<uint32_t> &indices);

  /** @brief Triggers the glDrawElements call for this specific geometry. */
  void Draw() const;

  ~Mesh();

private:
  uint32_t m_VAO, m_VBO, m_EBO;
  uint32_t m_IndexCount;
};
