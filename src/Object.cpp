#include "Object.hpp"
#include "Utils/ObjectParser.hpp"

std::expected<Object, std::error_code>
Object::parse(std::filesystem::path const &path) {
  return ObjectParser::parse(path);
}
