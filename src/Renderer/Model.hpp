#pragma once

#include "Mesh.hpp"
#include <filesystem>
#include <string>
#include <vector>

/**
 * @class Model
 * @brief A collection of Meshes representing a full 3D object.
 * * This is the object returned by your ObjectParser. It manages multiple
 * meshes and their associated textures/materials.
 */
class Model {
public:
  /**
   * @brief Loads a model file (like .obj) via the ObjectParser.
   * @param path Path to the .obj file.
   */
  Model(std::filesystem::path const &path);

  /** @brief Returns all meshes belonging to this model. */
  const std::vector<Mesh> &GetMeshes() const { return m_Meshes; }

private:
  friend class ObjectParser;

  std::vector<Mesh> m_Meshes;
  std::filesystem::path m_Directory;
};
