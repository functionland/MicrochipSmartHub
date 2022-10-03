#pragma once

#include <libusb-1.0/libusb.h>

#include "smart_hub_manager.h"
namespace SmartHub {
class UsbHubController : public IHubController {
 public:
  UsbHubController();
  ~UsbHubController();
  void SetVidPid(uint16_t vid, uint16_t pid);

  bool Initialize() override;

  bool RegisterRead(uint32_t address, uint16_t length,
                    std::vector<uint8_t> &buffer) override;
  bool RegisterWrite(uint32_t address, uint16_t length,
                     const std::vector<uint8_t> &buffer) override;

  bool SetLogicalMapping(uint8_t phy_port,uint8_t logic_from, uint8_t logic_to) override;
  bool SetPortConfiguration(uint8_t phy_port,UsbConfiguration config) override;

  /// @brief  Reset SmartHub from usb PC0
  /// • Only one device can be accessed at a time
  /// • No support is provided when multiple HFCs with same VID and PID are
  /// present
  /// @return true if successfull
  bool Reset() override;

  bool CloseEverything() override;

  std::string Name() override;

 private:
  // libusb variables
  libusb_device_handle *handle_ = NULL;
  // Context Variable used for initializing LibUSB session
  libusb_context *ctx_ = NULL;

  uint16_t vid_ = 0x0424;
  uint16_t pid_ = 0x7240;
  int last_error_code = 0;
};
}  // namespace SmartHub
