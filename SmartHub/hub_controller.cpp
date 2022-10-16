#include "hub_controller.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <thread>

#include <chrono>

#include "log/logger.h"

static constexpr auto TAG{"HubController"};

static const std::string RESET_N("23");
static const std::string GPIO_EXPORT("/sys/class/gpio/export");
static const std::string GPIO_DIRECTION =
    "/sys/class/gpio/gpio" + RESET_N + "/direction";
static const std::string GPIO_VALUE =
    "/sys/class/gpio/gpio" + RESET_N + "/value";

namespace SmartHub {
bool IHubController::Reset() {
  /// export gpio RESET_N pin
  int fd = open(GPIO_EXPORT.c_str(), O_WRONLY);
  if (fd == -1) {
    LOG::Warn(TAG, "Unable to open {}", GPIO_EXPORT);
    return false;
  }

  if (write(fd, RESET_N.c_str(), 2) != 2) {
    LOG::Warn(TAG, "Error writing to {}", GPIO_EXPORT);
    return false;
  }

  close(fd);

  /// Set the pin to be an output
  fd = open(GPIO_DIRECTION.c_str(), O_WRONLY);
  if (fd == -1) {
    LOG::Warn(TAG, "Unable to open {}", GPIO_DIRECTION);
    return false;
  }

  if (write(fd, "out", 3) != 3) {
    LOG::Warn(TAG, "Error writing to {}", GPIO_DIRECTION);
    return false;
  }
  close(fd);

  fd = open(GPIO_VALUE.c_str(), O_WRONLY);
  if (fd == -1) {
    LOG::Warn(TAG, "Unable to open {}", GPIO_VALUE);
    return false;
  }

  if (write(fd, "1", 1) != 1) {
    LOG::Warn(TAG, "Error writing to {}", GPIO_VALUE);
    return false;
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  if (write(fd, "0", 1) != 1) {
    LOG::Warn(TAG, "Error writing to {}", GPIO_VALUE);
    return false;
  }

  close(fd);

  return true;
}
}  // namespace SmartHub