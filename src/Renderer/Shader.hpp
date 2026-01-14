#pragma once

#include "Math/Math.hpp"
#include <string>

/**
 * @class Shader
 * @brief Manages OpenGL shader programs (Vertex and Fragment).
 * * This class handles the compilation of GLSL source code, linking the
 * program, and provides a simple interface to set uniform variables.
 */
class Shader {
public:
  /**
   * @brief Creates and compiles a shader program from file paths.
   * @param vertexPath Path to the vertex shader source.
   * @param fragmentPath Path to the fragment shader source.
   */
  Shader(const std::string &vertexPath, const std::string &fragmentPath);
  ~Shader();

  void Bind() const;
  void Unbind() const;

  /**
   * @brief Uploads a 4x4 matrix to the GPU (e.g., Model or View matrices).
   * @param name The name of the uniform in the GLSL code.
   * @param matrix The glm matrix data.
   */
  void SetUniformMat4(const std::string &name, const Mat4 &matrix);

private:
  uint32_t m_RendererID;
};
