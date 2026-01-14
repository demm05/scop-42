#pragma once

#include <string>

/**
 * @class Texture
 * @brief Represents a 2D image uploaded to the GPU.
 * * Handles loading image files (PNG/JPG) using stb_image and setting up
 * OpenGL texture parameters like wrapping and filtering.
 */
class Texture {
public:
  /**
   * @brief Loads and generates an OpenGL texture.
   * @param path The filesystem path to the image.
   */
  Texture(const std::string &path);
  ~Texture();

  /**
   * @brief Binds the texture to a specific GPU slot.
   * @param slot The texture unit (0 to 15 usually).
   */
  void Bind(uint32_t slot = 0) const;

private:
  uint32_t m_RendererID;
  int m_Width, m_Height, m_BPP;
};
