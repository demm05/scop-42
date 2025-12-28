#include "Logger.h"
#include "MappedFile.h"

#include <print>
#include <vector>

struct Vertex {
  float x, y, z;
};

struct Object {
  std::vector<Vertex> vertecies;
};

class ObjectParser {
public:
  static std::expected<Object, std::error_code> parse(char const *filePath) {
    auto file = MappedFile::open(filePath);
    if (!file)
      return std::unexpected(file.error());
    std::string_view line = file->view().substr(0, file->view().find('\n'));
    std::println("{}", line);
    return Object();
  }

private:
};

int main(int argc, char **argv) {
  Logger::init();
  if (argc < 2) {
    CORE_ERROR("Please prove arguments to which textures to load");
    return -1;
  }
  ObjectParser::parse(argv[1]);
}
