

#include "http_client.h"
#include "i2c_smart_hub_manager.h"
#include "log/logger.h"
#include "usb_smart_hub_manager.h"
static constexpr auto TAG{"Main"};
using namespace SmartHub;
int main() {
  //UsbSmartHubManager usb;

  std::string LOG_PATH = ".";
  LOG::Init(LOG_PATH);
  LOG::Warn(TAG, "hello mohsen");

  //   HttpClient client;
  //   client.NotifyConnection("hello mohsen");

  //   usb.SetVidPid(0x0424, 0x7240);
  //   usb.Initialize();

  //   std::array<uint8_t, 7> port_map = {0, 0, 0, 0, 0, 0, 0};
  //   usb.PortMappingUsb2(port_map);

  std::string port_path = "/dev/i2c-9";
  I2CSmartHubManager i2c_sm(port_path, 0x2D);
  i2c_sm.Initialize();
  auto id=i2c_sm.RetrieveID();
  std::vector<uint8_t> buff;
  std::vector<uint8_t> data={0xBF,0xDF};

  return 1;
}