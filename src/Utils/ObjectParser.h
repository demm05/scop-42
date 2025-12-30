#pragma once

#include "Object.h"

#include <filesystem>
#include <span>
#include <string_view>
#include <vector>

struct Vertex {
  float x, y, z, w;

public:
  Vertex(std::span<const float, 4> v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}
};

struct Scene {
  std::vector<Vertex> vertices;

  struct Mesh {
    std::string name;
    std::vector<uint32_t> indices;
  };

  std::vector<Mesh> meshes;
};

class ObjectParser {
public:
  static std::expected<Object, std::error_code>
  parse(std::filesystem::path const &path);

private:
  ObjectParser(std::string const &filePath, std::string_view file);

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
  std::string filePath_;
  std::string_view file_;
  std::string_view line_;
  std::string_view data_;
  size_t lineNum_ = 0;

  std::vector<Vertex> vertecies_;
};
