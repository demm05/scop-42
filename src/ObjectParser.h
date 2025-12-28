#pragma once

#include "Object.h"

#include <string_view>

class ObjectParser {
public:
  static std::expected<Object, std::error_code>
  parse(std::filesystem::path const &path);

private:
  ObjectParser(std::string_view file);

  std::expected<Object, std::error_code> parse();

  void dispatchHandler();

private:
  void handleVertex();
  void handleFace();
  void handleObjectName();
  void handleGroupName();
  void handleSmoothingGroup();
  void handleMatirialLibrary();
  void handleMaterialName();

private:
  std::string_view file_;
  std::string_view line_;
  std::string_view data_;
  size_t lineNum_ = 0;
};
