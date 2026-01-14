#include "Renderer/Model.hpp"
#include "Core/Logger.hpp"
#include "Utils/ObjectParser.hpp"

Model::Model(std::filesystem::path const &path)
    : m_Directory(path.parent_path()) {
  auto object = ObjectParser::parse(path);
  if (!object) {
    CORE_ERROR("Failed to load model: {}", object.error().message());
    return;
  }
  *this = std::move(object.value());
}
