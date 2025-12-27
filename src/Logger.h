#pragma once

#include "spdlog/logger.h"
#include <memory>

class Logger {
public:
  static void init();

  static std::shared_ptr<spdlog::logger> getCoreLogger();

private:
  static std::shared_ptr<spdlog::logger> coreLogger_;
};

#define CORE_TRACE(...) ::Logger::getCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...) ::Logger::getCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...) ::Logger::getCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...) ::Logger::getCoreLogger()->error(__VA_ARGS__)
#define CORE_FATAL(...) ::Logger::getCoreLogger()->error(__VA_ARGS__)
