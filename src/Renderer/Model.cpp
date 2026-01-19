#include "Renderer/Model.hpp"
#include "Utils/ObjectParser.hpp"

Model::ModelResult Model::Load(std::filesystem::path const &path) {
  return ObjectParser::parse(path);
}
