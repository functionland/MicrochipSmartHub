
#include <chrono>
#include <thread>

#include "http_client.h"
#include "i2c_smart_hub_manager.h"
#include "log/logger.h"
#include "utility.h"
static constexpr auto TAG{"Main"};
using namespace SmartHub;
int main() {
  // UsbSmartHubManager usb;

  std::string LOG_PATH = ".";
  LOG::Init(LOG_PATH);
  LOG::Warn(TAG, "hello mohsen");

  //   HttpClient client;
  //   client.NotifyConnection("hello mohsen");

  //   usb.SetVidPid(0x0424, 0x7240);
  //   usb.Initialize();

  //   std::array<uint8_t, 7> port_map = {0, 0, 0, 0, 0, 0, 0};
  //   usb.PortMappingUsb2(port_map);

  std::string port_path = "/dev/i2c-8";
  I2CSmartHubManager i2c_sm(port_path, 0x2D);
  std::thread([&]() {
    while (!i2c_sm.Initialize()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      continue;
    }
    /// Reset USB hub with gpios

    /// Set delay for Starting SmartHub
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    /// enable Smbus during runtime
    while (!i2c_sm.SendSpecialCmd(
        SpecialSmbusCommands::USB_ATTACH_WITH_SMB_RUNTIME_ACCESS)) {
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      continue;
    }
    while (auto id = i2c_sm.RetrieveID()) {
      if (id != -1) {
        LOG::Trace(TAG, "USB ID: {}", id);
        LOG::Trace(TAG, "USB VID: {}", i2c_sm.RetrieveUsbVID());

        int rev = i2c_sm.RetrieveRevision();
        LOG::Trace(TAG, "USB Revision: {} ", rev);
      } else {
        LOG::Trace(TAG, "USB ID: {}", id);
        LOG::Trace(TAG, "USB VID: {}", i2c_sm.RetrieveUsbVID());
        continue;
      }
    }

    /// 2- Read USB Configs

    /// 3- Set USB Configs
    
  }).join();

  return 1;
}