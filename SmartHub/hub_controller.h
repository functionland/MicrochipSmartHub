#pragma once

#include <stdint.h>

#include <array>
#include <string>
#include <vector>

namespace SmartHub {
enum Stage {
  STG_SPI_INIT,
  STG_CFG_ROM,
  STG_CFG_STRAP,
  STG_SMBUS_CHECK,
  STG_CFG_OTP,
  STG_CFG_SOC,
  STG_NORMAL_MODE,
  STG_UNKNOWN_MODE,
};
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

  virtual bool PortMappingUsb2(std::array<uint8_t, 7> port_map) = 0;
  virtual bool PortMappingUsb3(std::array<uint8_t, 7> port_map) = 0;

  virtual bool Reset() = 0;

  virtual bool CloseEverything() = 0;
  virtual std::string Name() = 0;

};
}  // namespace SmartHub
