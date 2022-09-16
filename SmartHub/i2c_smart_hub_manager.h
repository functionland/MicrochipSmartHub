
#ifndef USB_SMART_HUB_MANAGER_H
#define USB_SMART_HUB_MANAGER_H

#include "smart_hub_manager.h"

class I2CSmartHubManager : public SmartHubManager {
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

 protected:
  void PreapareMessage(SmartHubCommandType type, uint8_t total_bytes_loaded,
                       uint32_t reg_addr, std::vector<uint8_t> &buff);

  bool WriteSmbus(uint8_t reg, std::vector<uint8_t> &buff);
  bool ReadSmbus(uint8_t reg, std::vector<uint8_t> &buff);

 private:
  std::string port_path_;
  const int i2c_address_;
  int file_handle_{0};
};
#endif  // USB_SMART_HUB_MANAGER_H