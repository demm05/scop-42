#include <expected>
#include <filesystem>
#include <span>
#include <string_view>
#include <vector>

#include "Renderer/Model.hpp"

class ObjectParser {
public:
  static std::expected<Model, std::error_code>
  parse(std::filesystem::path const &path);

private:
  ObjectParser(std::filesystem::path const &filePath, std::string_view file);

  std::expected<Model, std::error_code> parse();

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

  // Temporary storage for parsing
  std::vector<Vertex> vertices_;
  Model model_;
};
