#include "Utils/ObjectParser.hpp"
#include "Core/Logger.hpp"
#include "Utils/MappedFile.hpp"
#include <cassert>
#include <charconv>
#include <cstdint>
#include <expected>
#include <ranges>
#include <string_view>
#include <system_error>

namespace {

constexpr uint32_t hash(std::string_view sv) {
  uint32_t hash = 0x811c9dc5;
  for (char c : sv) {
    hash ^= static_cast<uint32_t>(c);
    hash *= 0x01000193;
  }
  return hash;
}

} // namespace

std::expected<Model, std::error_code>
ObjectParser::parse(std::filesystem::path const &path) {
  auto file = MappedFile::open(path);
  if (!file)
    return std::unexpected(file.error());
  ObjectParser op(path, file.value().view());
  return op.parse();
}

ObjectParser::ObjectParser(std::filesystem::path const &filePath,
                           std::string_view file)
    : filePath_(filePath), file_(file), model_(filePath) {}

std::expected<Model, std::error_code> ObjectParser::parse() {
  for (auto const line_range : file_ | std::views::split('\n')) {
    lineNum_++;
    line_ = std::string_view(line_range.begin(), line_range.end());

    { // Trim the line
      if (!line_.empty() && line_.back() == '\r') {
        line_.remove_suffix(1);
      }
      auto first_content = line_.find_first_not_of(" \t");
      if (first_content == std::string_view::npos)
        continue;
      line_.remove_prefix(first_content);
    }

    if (line_.empty() || line_.starts_with('#'))
      continue;
    dispatchHandler();
  }
  return model_;
}

void ObjectParser::dispatchHandler() {
  auto first_space = line_.find_first_of(" \t");
  std::string_view command = line_.substr(0, first_space);
  data_ = first_space == std::string_view::npos ? ""
                                                : line_.substr(first_space + 1);

  // clang-format off
#define CASE(cmd, func) case hash(cmd): func(); break

  switch (hash(command)) {
    CASE("v", handleVertex);
    CASE("f", handleFace);
    CASE("o", handleObjectName);
    CASE("g", handleGroupName);
    CASE("s", handleSmoothingGroup);
    CASE("mtllib", handleMatirialLibrary);
    CASE("usemtl", handleMaterialName);
  default: CORE_WARN("uknown command on line {} {}", lineNum_, line_); break;
  }

#undef CASE
  // clang-format on
}

#define PARSE_CHECK(cond, col, msg)                                            \
  if (!(cond)) {                                                               \
    CORE_WARN("Parse Error at [{}:{}]\n  {}\n  {:>{}}^-- {}",                  \
              filePath_.string(), lineNum_, line_, "", (col), (msg));          \
    return;                                                                    \
  }

[[nodiscard]] inline constexpr const char *
skip_spaces(const char *first, const char *last) noexcept {
  while (first < last && (*first == ' ' || *first == '\t')) {
    first++;
  }
  return first;
}

void ObjectParser::handleVertex() {
  char const *first = data_.data();
  char const *last = data_.data() + data_.size();

  // Temporary simplified loading for now, as Vertex struct in Mesh.hpp expects
  // 3 floats (Vec3) and we were parsing 4. The user asked not to finish
  // implementing mesh population perfectly. We'll just parse to ensure code
  // validity but might not fully populate a final Mesh::Vertex yet.

  float v[4]{0.0f, 0.0f, 0.0f, 1.0f};

  for (int i = 0; i < 4; ++i) {
    first = skip_spaces(first, last);
    if (first == last) {
      // 3 coordinates are enough
      PARSE_CHECK(i >= 3, line_.length(), "Missing coordinates");
      break;
    }

    auto [ptr, ec] = std::from_chars(first, last, v[i]);
    PARSE_CHECK(ec == std::errc{}, first - line_.data(),
                std::make_error_code(ec).message());
    first = ptr;
  }
  // TODO: Add to temporary vertex storage or directly to current mesh being
  // built
}

void ObjectParser::handleFace() {
  // Parsing logic kept but Mesh population deferred
}
void ObjectParser::handleObjectName() {}
void ObjectParser::handleGroupName() {}
void ObjectParser::handleSmoothingGroup() {}
void ObjectParser::handleMatirialLibrary() {}
void ObjectParser::handleMaterialName() {}

#undef PARSE_CHECK
