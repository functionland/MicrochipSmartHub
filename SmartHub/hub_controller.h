#pragma once

#include <stdint.h>

#include <array>
#include <string>
#include <vector>
#include <memory>

#include "port_usb.h"

namespace SmartHub {

enum class CommandType : uint8_t {
  READ,
  WRITE_FOR_WRITE,
  WRITE_FOR_READ,
};

class IHubController {
 public:
  virtual bool Initialize() = 0;

  virtual bool RegisterRead(uint32_t address, uint16_t length,
                            std::vector<uint8_t> &buffer) = 0;
  virtual bool RegisterWrite(uint32_t address, uint16_t length,
                             const std::vector<uint8_t> &buffer) = 0;

  virtual bool SetLogicalMapping(uint8_t phy_port,uint8_t logic_from, uint8_t logic_to) = 0;
  virtual uint8_t GetLogicalMapping(uint8_t phy_port)=0;
  virtual bool SetPortConfiguration(uint8_t phy_port,UsbConfiguration config) = 0;
  virtual UsbConfiguration GetPortConfiguration(uint8_t phy_port)=0;

  virtual bool Reset() = 0;

  virtual bool CloseEverything() = 0;
  virtual std::string Name() = 0;

  void AddUsbPort(std::shared_ptr<Usbport> port){
    ports_.push_back(port);
  }

protected:
  std::vector<std::shared_ptr<Usbport>> ports_;
};
}  // namespace SmartHub
