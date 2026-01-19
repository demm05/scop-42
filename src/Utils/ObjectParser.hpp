#include "Renderer/Mesh.hpp"
#include "Renderer/Model.hpp"
#include <filesystem>
#include <string_view>
#include <unordered_map>
#include <vector>

class ObjectParser {
public:
  static Model::ModelResult parse(std::filesystem::path const &path);

private:
  ObjectParser(std::filesystem::path const &filePath, std::string_view file);

  Model::ModelResult parse();

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
  std::filesystem::path filePath_;
  std::string_view file_;
  std::string_view line_;
  std::string_view data_;
  size_t lineNum_ = 0;

  Model model_;
  std::unordered_map<uint32_t, Vertex> allVertices_;
  std::vector<uint32_t> meshVertices_;
  std::vector<uint32_t> meshIndices_;
};
