
#include <chrono>
#include <thread>

#include "http_client.h"
#include "hub_manager.h"
#include "log/logger.h"
#include "utility.h"
#include "hub_controller_i2c.h"

static constexpr auto TAG{"Main"};
using namespace SmartHub;
int main() {
  // UsbHubController usb;

  std::string LOG_PATH = ".";
  LOG::Init(LOG_PATH);
  LOG::Warn(TAG, "hello mohsen");

  //   HttpClient client;
  //   client.NotifyConnection("hello mohsen");

  //   usb.SetVidPid(0x0424, 0x7240);
  //   usb.Initialize();

  //   std::array<uint8_t, 7> port_map = {0, 0, 0, 0, 0, 0, 0};
  //   usb.PortMappingUsb2(port_map);

  HubManager manager;
  const std::string i2c_hub1_path="/dev/i2c-1";
  auto i2c_hub1=std::make_shared<I2CHubController>(i2c_hub1_path,0x2D);
  manager.AddHubController(i2c_hub1);
  manager.Start();    

  return 1;
}