#include "MappedFile.h"
#include <fcntl.h>
#include <filesystem>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

std::expected<MappedFile, std::error_code>
MappedFile::open(std::filesystem::path const &path) {
  int fd = ::open(path.c_str(), O_RDONLY);
  if (fd == -1) {
    return std::unexpected(std::error_code(errno, std::generic_category()));
  }

  struct stat sb;
  if (::fstat(fd, &sb) == -1) {
    auto err = std::error_code(errno, std::generic_category());
    close(fd);
    return std::unexpected(err);
  }

  size_t length = static_cast<size_t>(sb.st_size);
  if (length == 0) {
    ::close(fd);
    return MappedFile(nullptr, 0);
  }

  void *addr = ::mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);
  ::close(fd);
  if (addr == MAP_FAILED) {
    return std::unexpected(std::error_code(errno, std::generic_category()));
  }

  return MappedFile(addr, length);
}

MappedFile::~MappedFile() { cleanup(); }

MappedFile::MappedFile(MappedFile &&other) noexcept
    : length_(other.length_), data_(other.data_) {
  other.data_ = {};
}

MappedFile &MappedFile::operator=(MappedFile &&other) noexcept {
  if (this != &other) {
    cleanup();
    length_ = other.length_;
    data_ = other.data_;
    other.data_ = {};
  }
  return *this;
}

std::string_view MappedFile::view() const { return data_; }
size_t MappedFile::length() const { return length_; }

MappedFile::MappedFile(void *address, size_t length)
    : length_(length), data_(static_cast<const char *>(address), length) {}

void MappedFile::cleanup() noexcept {
  if (data_.data() != nullptr) {
    ::munmap(const_cast<char *>(data_.data()), length_);
  }
  length_ = 0;
  data_ = {};
}
