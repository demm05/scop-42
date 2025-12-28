#include "Logger.h"
#include "Object.h"

#include <print>

struct Vertex {
  float x, y, z;
};

int main(int argc, char **argv) {
  Logger::init();
  if (argc < 2) {
    CORE_ERROR("Please prove arguments to which textures to load");
    return -1;
  }
  Object::parse(argv[1]);
}
