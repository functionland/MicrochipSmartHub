#include "hub_controller.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>
#include <cstdio>
#include <thread>

#include "log/logger.h"

static constexpr auto TAG{"HubController"};

static const int RESET_N=23;

namespace SmartHub {
bool IHubController::Reset() {
  /// export gpio RESET_N pin
  FILE *fd = fopen("/sys/class/gpio/export", "w");
  if (fd == NULL) {
    LOG::Warn(TAG, "Unable to open /sys/class/gpio/export {}",strerror(errno));
    return false;
  }

  fprintf(fd, "%d\n", RESET_N);
  fclose(fd);

  /// Set the pin to be an output
  char fName[128];
  sprintf(fName, "/sys/class/gpio/gpio%d/direction", RESET_N);
  if ((fd = fopen(fName, "w")) == NULL)
    if (fd == NULL) {
      LOG::Warn(TAG, "Unable to open {}", fName);
      return false;
    }
  fprintf(fd, "out\n");
  fclose(fd);

  sprintf(fName, "/sys/class/gpio/gpio%d/value", RESET_N);
  if ((fd = fopen(fName, "w")) == NULL)
    if (fd == NULL) {
      LOG::Warn(TAG, "Unable to open {}", fName);
      return false;
    }
  fprintf(fd, "1\n");
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  fprintf(fd, "0\n");
  fclose(fd);

  return true;
}
}  // namespace SmartHub