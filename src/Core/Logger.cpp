#include "Logger.hpp"
#include "spdlog/common.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

std::shared_ptr<spdlog::logger> Logger::coreLogger_;

void Logger::init() {
  spdlog::set_pattern("%^[%T] %n: %v%$");
  coreLogger_ = spdlog::stdout_color_mt("core");
  coreLogger_->set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger> Logger::getCoreLogger() { return coreLogger_; }
