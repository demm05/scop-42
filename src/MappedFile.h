#pragma once

#include <expected>
#include <span>
#include <system_error>

struct MappedFile {
public:
  static std::expected<MappedFile, std::error_code> open(char const *filePath);
  std::string_view view() const;
  size_t length() const;

public:
  template <typename T> std::span<const T> as_span() const noexcept {
    const size_t count = length_ / sizeof(T);
    return std::span<const T>(reinterpret_cast<const T *>(data_.data()), count);
  }

public:
  MappedFile(const MappedFile &) = delete;
  MappedFile &operator=(const MappedFile &) = delete;

  MappedFile(MappedFile &&other) noexcept;
  MappedFile &operator=(MappedFile &&other) noexcept;
  ~MappedFile();

private:
  MappedFile(void *address, size_t length);
  void cleanup() noexcept;

private:
  size_t length_;
  std::string_view data_;
};
