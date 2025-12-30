#pragma once

#include <expected>
#include <filesystem>
#include <system_error>

struct Object {
public:
  static std::expected<Object, std::error_code>
  parse(std::filesystem::path const &path);
};
