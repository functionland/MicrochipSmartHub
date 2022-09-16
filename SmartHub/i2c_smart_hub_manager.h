
#ifndef I2C_SMART_HUB_MANAGER_H
#define I2C_SMART_HUB_MANAGER_H

#include "smart_hub_manager.h"
namespace SmartHub {
enum class SpecialSmbusCommands : uint16_t {
  CONFIG_REG_ACCESS,                   // 0X9937
  USB_ATTACH,                          // 0XAA55
  USB_ATTACH_WITH_SMB_RUNTIME_ACCESS,  // 0XAA56
  OTP_PROGRAM,                         // 0X9933
  OTP_READ,                            // 0X9934
};
class I2CSmartHubManager : public ISmartHubManager {
 public:
  I2CSmartHubManager(std::string &port_path, int i2c_address);
  ~I2CSmartHubManager() = default;

  bool Initialize() override;

  bool RegisterRead(uint32_t address, uint16_t length,
                    std::vector<uint8_t> &buffer) override;
  bool RegisterWrite(uint32_t address, uint16_t length,
                     const std::vector<uint8_t> &buffer) override;

  bool PortMappingUsb2(std::array<uint8_t, 7> port_map) override;
  bool PortMappingUsb3(std::array<uint8_t, 7> port_map) override;

  bool Reset() override;

  bool CloseEverything() override;

  void PreapareMessage(CommandType type, uint8_t total_bytes_loaded,
                       uint32_t reg_addr, std::vector<uint8_t> &buff);

  void PreapareSpecialMessage(SpecialSmbusCommands type,
                              std::vector<uint8_t> &buff);

 protected:
  bool WriteSmbus(uint8_t reg, std::vector<uint8_t> &buff);
  bool ReadSmbus(uint8_t reg, std::vector<uint8_t> &buff);

 private:
  std::string port_path_;
  const int i2c_address_;
  int file_handle_{0};
};
}  // namespace SmartHub
#endif  // I2C_SMART_HUB_MANAGER_H