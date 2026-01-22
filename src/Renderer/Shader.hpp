#pragma once

#include "Math/Math.hpp"
#include <expected>
#include <filesystem>
#include <string>

/**
 * @class Shader
 * @brief Manages OpenGL shader program lifecycle and uniforms.
 */
class Shader {
public:
  /** @brief Activates the shader program for rendering. */
  void Bind() const;

  /** @brief Deactivates the current shader program. */
  void Unbind() const;

  /**
   * @brief Sets a 4x4 matrix uniform in the shader.
   * @param name Uniform variable name in GLSL.
   * @param matrix Matrix data to upload.
   */
  void SetUniformMat4(const std::string &name, const Mat4 &matrix);

public:
  using Result = std::expected<Shader, std::string>;
  /**
   * @brief Loads and compiles a shader from files on disk.
   * @return Shader instance or error message.
   */
  static Result FromFile(const std::filesystem::path &vertexPath,
                         const std::filesystem::path &fragmentPath);

  /**
   * @brief Compiles a shader from raw GLSL source strings.
   * @return Shader instance or error message.
   */
  static Result FromSource(std::string_view vertexSrc,
                           std::string_view fragmentSrc);

public:
  ~Shader();

  Shader(Shader &&other) noexcept;
  Shader &operator=(Shader &&other) noexcept;

  // Shaders represent unique GPU resources and cannot be copied.
  Shader(const Shader &) = delete;
  Shader &operator=(const Shader &) = delete;

private:
  explicit Shader(uint32_t id) : m_RendererID(id) {}
  uint32_t m_RendererID;
};
