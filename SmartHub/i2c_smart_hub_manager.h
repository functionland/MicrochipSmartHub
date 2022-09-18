
#ifndef I2C_SMART_HUB_MANAGER_H
#define I2C_SMART_HUB_MANAGER_H

#include "smart_hub_manager.h"
#include <atomic>

namespace SmartHub {

enum class SpecialSmbusCommands : uint16_t {
  CONFIG_REG_ACCESS,                   // 0X9937  Run memory command
  USB_ATTACH,                          // 0XAA55  Go to runtime without SMBus
  USB_ATTACH_WITH_SMB_RUNTIME_ACCESS,  // 0XAA56  Go to runtime with SMBus
  OTP_PROGRAM,  // 0X9933  Permanently program configuration commands to the OTP
  OTP_READ,     // 0X9934  Read the values of the OTP register
};

class I2CSmartHubManager : public ISmartHubManager {
 public:
  I2CSmartHubManager(std::string &port_path, int i2c_address);
  ~I2CSmartHubManager() = default;

  bool Initialize() override;

  /// @brief Run the Read Configuration Register command sequence
  /// @param address is Register to be read
  /// @param length  Number of bytes that will be read
  /// @param data vector which will hold the return value
  /// @return true if successful
  bool RegisterRead(uint32_t address, uint16_t length,
                    std::vector<uint8_t> &data) override;

  /// @brief Run the Write Configuration Register command sequence
  /// @param address Register to be written
  /// @param length  Number of bytes that will be written
  /// @param data vector which holds the data to be written
  /// @return true if successfull
  bool RegisterWrite(uint32_t address, uint16_t length,
                     const std::vector<uint8_t> &data) override;

  bool PortMappingUsb2(std::array<uint8_t, 7> port_map) override;
  bool PortMappingUsb3(std::array<uint8_t, 7> port_map) override;

  bool Reset() override;

  bool CloseEverything() override;

  bool SendSpecialCmd(SpecialSmbusCommands cmd);
  uint32_t RetrieveRevision();
  uint16_t RetrieveID();
  uint32_t RetrieveConfiguration();
  int SetUpstreamPort(uint8_t port);
  int SetUsbVID(uint8_t usb_vid1, uint8_t usb_vid2);
  uint16_t RetrieveUsbVID();
  int IsPortActive(uint8_t port);
  int IsPortEnabled(uint8_t port);
  int DisablePort(uint8_t port);
  int SetFlexFeatureRegisters(uint16_t value);
  uint16_t GetFlexFeatureRegisters();
  int SetPrimaryI2CAddressRegisters(uint16_t address);
  uint8_t GetPrimaryI2CAddressRegisters();
  int SetSecondryI2CAddressRegisters(uint16_t address);
  uint8_t GetSecondryI2CAddressRegisters();

 protected:
  void PrepareMessage(CommandType type, uint8_t total_bytes_loaded,
                      uint32_t reg_addr, const std::vector<uint8_t> &data,
                      std::vector<uint8_t> &buff);

  void PrepareSpecialMessage(SpecialSmbusCommands type,
                             std::vector<uint8_t> &buff);
  bool WriteSmbus(std::vector<uint8_t> &buff);
  bool ReadSmbus(std::vector<uint8_t> &buff);

 private:
  std::atomic<bool> initialized_ {false};
  std::string port_path_;
  const int i2c_address_ = 0x2D;
  /// @brief  handle to i2c file open
  int file_handle_{0};
};
}  // namespace SmartHub
#endif  // I2C_SMART_HUB_MANAGER_H