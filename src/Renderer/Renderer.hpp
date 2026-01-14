#pragma once

#include "Math/Math.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

/**
 * @class Renderer
 * @brief Static system responsible for executing draw commands.
 * * This class abstracts the OpenGL draw calls. It handles setup like
 * clearing the screen and submitting meshes for rendering.
 */
class Renderer {
public:
  /** @brief Clears the color and depth buffers. */
  static void Clear();

  /**
   * @brief Draws a single mesh using a specific shader and transform.
   * @param mesh The geometry to draw.
   * @param shader The shader to use.
   * @param transform The world position/rotation/scale matrix.
   */
  static void Submit(const Mesh &mesh, const Shader &shader,
                     const Mat4 &transform);
};
