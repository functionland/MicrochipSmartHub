#ifndef DIAGNOSTICS_LOGGER_H
#define DIAGNOSTICS_LOGGER_H

#include <string>
#include <string_view>
#include "spdlog/spdlog.h"

/**
 * @class Logger
 * @brief Global Class for Logging into file and console and others,
 *
 *  @details I suggest to use second type of Logging with three args format,
 *  because that prevent application crash due to improper use of
 *  fmt::format the second one report exception if invalid format used
 */
class Logger {
 public:
  /// This function will initilize loggers types(console,file,...),
  ///  output format and flush policy for loggers, we use spdlog
  ///  library for logging
  static void Init(std::string_view path);
  [[noreturn]] static void KillSignal(int sig);
  static void Error(std::string_view tag, std::string_view msg);

  template <typename... Args>
  static void Error(std::string_view tag, std::string_view format,
                    Args&&... args) {
    try {
      for (const auto& log : logger_) {
        log->error(kFormat, tag, fmt::format(format, args...));
      }
    } catch (...) {
      for (const auto& log : logger_) {
        log->error(kFormat, tag, "EXCEPTION IN FORMAT");
      }
    }
  }
  static void Fatal(std::string_view tag, std::string_view msg);
  template <typename... Args>
  static void Fatal(std::string_view tag, std::string_view format,
                    Args&&... args) {
    try {
      for (const auto& log : logger_) {
        log->critical(kFormat, tag, fmt::format(format, args...));
      }
    } catch (...) {
      for (const auto& log : logger_) {
        log->critical(kFormat, tag, "EXCEPTION IN FORMAT");
      }
    }
  }
  static void Message(std::string_view tag, std::string_view msg);
  template <typename... Args>
  static void Message(std::string_view tag, std::string_view format,
                      Args&&... args) {
    try {
      for (const auto& log : logger_) {
        log->info(kFormat, tag, fmt::format(format, args...));
      }
    } catch (...) {
      for (const auto& log : logger_) {
        log->error(kFormat, tag, fmt::format("EXCEPTION IN FORMAT {}", format));
      }
    }
  }
  static void Debug(std::string_view tag, std::string_view msg);
  template <typename... Args>
  static void Debug(std::string_view tag, std::string_view format,
                    Args&&... args) {
    try {
      for (const auto& log : logger_) {
        log->debug(kFormat, tag, fmt::format(format, args...));
      }
    } catch (...) {
      for (const auto& log : logger_) {
        log->error(kFormat, tag, "EXCEPTION IN FORMAT");
      }
    }
  }
  static void Trace(std::string_view tag, std::string_view msg);
  template <typename... Args>
  static void Trace(std::string_view tag, std::string_view format,
                    Args&&... args) {
    try {
      for (const auto& log : logger_) {
        log->trace(kFormat, tag, fmt::format(format, args...));
      }
    } catch (...) {
      for (const auto& log : logger_) {
        log->error(kFormat, tag, "EXCEPTION IN FORMAT");
      }
    }
  }
  static void Warn(std::string_view tag, std::string_view msg);
  template <typename... Args>
  static void Warn(std::string_view tag, std::string_view format,
                   Args&&... args) {
    try {
      for (const auto& log : logger_) {
        log->warn(kFormat, tag, fmt::format(format, args...));
      }
    } catch (...) {
      for (const auto& log : logger_) {
        log->error(kFormat, tag, "EXCEPTION IN FORMAT");
      }
    }
  }

 private:
  /// group of loggers, you can add other loggers by Logger::Init
  static std::vector<std::shared_ptr<spdlog::logger>> logger_;
  static constexpr auto kFormat{"{:<25} {}"};
};


using LOG = Logger;
template <typename T, typename... P>
std::string MxStr(T&& format, P&&... params) {
  try {
    return fmt::format(std::forward<T>(format), std::forward<P>(params)...);
  } catch (...) {
    LOG::Warn("FMT EXP", "{}", format);
    return "";
  }
}




#endif  // DIAGNOSTICS_LOGGER_H
