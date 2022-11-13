
#include <chrono>
#include <thread>

#include "http_server.h"
#include "hub_manager.h"
#include "log/logger.h"
#include "utility.h"

static constexpr auto TAG{"Main"};
using namespace SmartHub;
int main() {
  // UsbHubController usb;

  std::string LOG_PATH = ".";
  LOG::Init(LOG_PATH);
  LOG::Warn(TAG, "SmartHub Started...");

  HttpServer http_server;
  http_server.Init(8095);
  http_server.Start();


  HubManager hub_manager;

  //create first hub controller
  const std::string path="/dev/i2c-1";
  constexpr auto address=0x2D;
  hub_manager.AddNewI2CController(path,address);

  
  hub_manager.Start();    

  return 1;
}