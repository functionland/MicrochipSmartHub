#include "logger.h"

#include <signal.h>
#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

std::vector<std::shared_ptr<spdlog::logger>> Logger::logger_{};
void Logger::Init(std::string_view path) {
  static constexpr auto kMaxFileByte{1024*1024*50};
  static constexpr auto kMaxFileCount{1u};

  auto file_log = spdlog::rotating_logger_mt<spdlog::async_factory_nonblock>(
      "file", path.data(), kMaxFileByte, kMaxFileCount);
  file_log->set_pattern("[%x %H:%M:%S.%e] %v");
  file_log->set_level(spdlog::level::trace);

  auto console_log =
      spdlog::stdout_color_mt <spdlog::async_factory_nonblock> ("console");
  console_log->set_pattern("%^[%-8l] %x %H:%M:%S.%e %$ %v");
  console_log->set_level(spdlog::level::trace);

  spdlog::flush_every(std::chrono::seconds(5));
  spdlog::flush_on(spdlog::level::critical);

  logger_.push_back(file_log);
  logger_.push_back(console_log);
}

void Logger::Error(std::string_view tag, std::string_view msg) {
  try {
    for (const auto& log : logger_) {
      log->error(kFormat, tag, msg);
    }
  } catch (...) {
    for (const auto& log : logger_) {
      log->error(kFormat, tag, "EXCEPTION IN FORMAT");
    }
  }
}

void Logger::Fatal(std::string_view tag, std::string_view msg) {
  try {
    for (const auto& log : logger_) {
      log->critical(kFormat, tag, msg);
    }
  } catch (...) {
    for (const auto& log : logger_) {
      log->error(kFormat, tag, "EXCEPTION IN FORMAT");
    }
  }
}
void Logger::Message(std::string_view tag, std::string_view msg) {
  try {
    for (const auto& log : logger_) {
      log->info(kFormat, tag, msg);
    }
  } catch (...) {
    for (const auto& log : logger_) {
      log->error(kFormat, tag, "EXCEPTION IN FORMAT");
    }
  }
}
void Logger::Debug(std::string_view tag, std::string_view msg) {
  try {
    for (const auto& log : logger_) {
      log->debug(kFormat, tag, msg);
    }
  } catch (...) {
    for (const auto& log : logger_) {
      log->error(kFormat, tag, "EXCEPTION IN FORMAT");
    }
  }
}
void Logger::Trace(std::string_view tag, std::string_view msg) {
  try {
    for (const auto& log : logger_) {
      log->trace(kFormat, tag, msg);
    }
  } catch (...) {
    for (const auto& log : logger_) {
      log->error(kFormat, tag, "EXCEPTION IN FORMAT");
    }
  }
}
void Logger::Warn(std::string_view tag, std::string_view msg) {
  try {
    for (const auto& log : logger_) {
      log->warn(kFormat, tag, msg);
    }
  } catch (...) {
    for (const auto& log : logger_) {
      log->error(kFormat, tag, "EXCEPTION IN FORMAT");
    }
  }
}

void Logger::KillSignal(int sig) {
  spdlog::dump_backtrace();
  LOG::Fatal("Kill Signal", fmt::format("Terminate App {}", sig));

  std::terminate();
}
