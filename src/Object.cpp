#include "Object.h"
#include "Utils/ObjectParser.h"

std::expected<Object, std::error_code>
Object::parse(std::filesystem::path const &path) {
  return ObjectParser::parse(path);
}
