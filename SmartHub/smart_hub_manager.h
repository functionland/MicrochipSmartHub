#ifndef SMART_HUB_MANAGER_H
#define SMART_HUB_MANAGER_H

#include <stdint.h>

#include <array>
#include <string>
#include <vector>

namespace SmartHub{
enum class Stages {
  SPI_INIT,
  CFG_ROM,
  CFG_STRAP,
  SMBUS_CHECK,
  CFG_OTP,
  CFG_SOC,
  NORMAL_MODE,
  UNKNOWN_MODE,
};
enum class CommandType : uint8_t {
  READ,
  WRITE,
};


enum class SpecialSmbusCommands : uint16_t {
  CONFIG_REG_ACCESS ,//0X9937
  USB_ATTACH , //0XAA55
  USB_ATTACH_WITH_SMB_RUNTIME_ACCESS,//0XAA56
  OTP_PROGRAM ,//0X9933
  OTP_READ ,//0X9934
};

class ISmartHubManager {
 public:
  virtual bool Initialize() = 0;

  virtual bool RegisterRead(uint32_t address, uint16_t length,
                            std::vector<uint8_t> &buffer) = 0;
  virtual bool RegisterWrite(uint32_t address, uint16_t length,
                             const std::vector<uint8_t> &buffer) = 0;

  virtual bool PortMappingUsb2(std::array<uint8_t, 7> port_map) = 0;
  virtual bool PortMappingUsb3(std::array<uint8_t, 7> port_map) = 0;

  virtual bool Reset() = 0;

  virtual bool CloseEverything() = 0;

  // virtual SmartHubStatus CurrentStatus()=0;
};
}

#endif  // end of SMART_HUB_MANAGER_H
