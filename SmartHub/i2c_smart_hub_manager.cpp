#include "i2c_smart_hub_manager.h"

#include "log/logger.h"
// for aceesing smbus port on linux
extern "C" {
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
}
#include <fcntl.h> /* For O_RDWR */
#include <sys/ioctl.h>
#include <unistd.h> /* For open(), creat() */

#include "smart_hub_regs_cmd.h"

static constexpr auto TAG{"I2cSmartHubManager"};

namespace SmartHub {
I2CSmartHubManager::I2CSmartHubManager(std::string &port_path, int i2c_address)
    : ISmartHubManager(), port_path_{port_path}, i2c_address_{i2c_address} {}

bool I2CSmartHubManager::Initialize() {
  file_handle_ = open(port_path_.c_str(), O_RDWR);
  if (file_handle_ < 0) {
    LOG::Warn(TAG, "Can not open i2c path {}", port_path_);
    return false;
  }

  if (ioctl(file_handle_, I2C_SLAVE, i2c_address_) < 0) {
    LOG::Warn(TAG, "Can not specify what device address");
    return false;
  }
  initialized_ = true;
  return true;
}
bool I2CSmartHubManager::RegisterRead(uint32_t address, uint16_t length,
                                      std::vector<uint8_t> &data) {
  if (!initialized_) {
    LOG::Warn(TAG, "Not Initilized (RegisterRead) ");
    return false;
  }
  std::vector<uint8_t> snd_buff;
  PrepareMessage(CommandType::WRITE_FOR_READ, 0, address, {}, snd_buff);
  if (!WriteSmbus(snd_buff)) {
    return false;
  }
  /* Run configuration access command */
  if (!SendSpecialCmd(SpecialSmbusCommands::CONFIG_REG_ACCESS)) {
    return false;
  }
  std::vector<uint8_t> read = {
      0x00,  // Buffer address MSB: always 0
      0x06,  // Buffer address LSB: 6 to skip header
  };
  if (!WriteSmbus(snd_buff)) {
    return false;
  }
  snd_buff.clear();
  if (!ReadSmbus(snd_buff)) {
    return false;
  }
  if (snd_buff.size() != length) {
    LOG::Warn(TAG, "Invalid return length %d != %d (length) ", snd_buff.size(),
              length);
  }
  data = snd_buff;
  return true;
}
bool I2CSmartHubManager::RegisterWrite(uint32_t address, uint16_t length,
                                       const std::vector<uint8_t> &data) {
  if (!initialized_) {
    LOG::Warn(TAG, "Not Initilized (RegisterWrite) ");
    return false;
  }
  std::vector<uint8_t> buff;
  PrepareMessage(CommandType::WRITE_FOR_WRITE, buff.size(), address, data,
                 buff);
  if (!WriteSmbus(buff)) {
    return false;
  }
  /* Run configuration access command */
  PrepareSpecialMessage(SpecialSmbusCommands::CONFIG_REG_ACCESS, buff);
  if (!WriteSmbus(buff)) {
    return false;
  }
  return true;
}
bool I2CSmartHubManager::PortMappingUsb2(std::array<uint8_t, 7> port_map) {
  return false;
}
bool I2CSmartHubManager::PortMappingUsb3(std::array<uint8_t, 7> port_map) {
  return false;
}
bool I2CSmartHubManager::Reset() { return false; }
bool I2CSmartHubManager::CloseEverything() {
  if (close(file_handle_) < 0) {
    LOG::Warn(TAG, "Can not close i2c port");
    return false;
  }
  return true;
}
bool I2CSmartHubManager::WriteSmbus(std::vector<uint8_t> &buff) {
  __u8 wbuf[256];
  for (int i = 1; i < buff.size(); i++) wbuf[i] = buff[i];
  wbuf[buff.size() - 1] = '\0';
  const auto res =
      i2c_smbus_write_block_data(file_handle_, 0x5A, buff.size(), wbuf);
  if (res < 0) {
    LOG::Warn(TAG, "Can not write to reg:{} ", 0x5A);
    return false;
  }
  return true;
}
bool I2CSmartHubManager::ReadSmbus(std::vector<uint8_t> &buff) {
  __u8 rbuf[256];
  /* Using SMBus commands */
  const auto res = i2c_smbus_read_block_data(file_handle_, 0x5B, rbuf);
  if (res < 0) {
    LOG::Warn(TAG, "Can not read from reg:{}", 0x5B);
    return false;
  }

  for (int i = 0; i < res; i++) buff.push_back(rbuf[i]);
  return true;
}

void I2CSmartHubManager::PrepareMessage(CommandType type,
                                        uint8_t total_bytes_loaded,
                                        uint32_t reg_addr,
                                        const std::vector<uint8_t> &data,
                                        std::vector<uint8_t> &buff) {
  buff.clear();
  switch (type) {
    case CommandType::WRITE_FOR_WRITE:
      buff.push_back(0x00);                    // byte 1
      buff.push_back(0x00);                    // byte 2
      buff.push_back(total_bytes_loaded + 6);  // byte 3
      buff.push_back(0x00);  // byte 4 transaction type 0x00 for write
      // byte 5 Input the total number of consecutive bytes to write
      // (i.e: total number of consecutive registers to modify)
      buff.push_back(total_bytes_loaded);  // byte 5 Register Data Length

      // byte 6-9
      buff.push_back((uint8_t)(reg_addr >> 24));
      buff.push_back((uint8_t)(reg_addr >> 16));
      buff.push_back((uint8_t)(reg_addr >> 8));
      buff.push_back((uint8_t)(reg_addr));
      for (auto &a : data) {
        buff.push_back(a);
      }
      LOG::Debug(TAG, "Write For Write SMBus ");
      // for byte 10-136
      // Up to 128 Bytes of Data payload. Must match the value present in Byte 5
      // and the value present in Byte 3 minus six (6).
      break;
    case CommandType::WRITE_FOR_READ:
      buff.push_back(0x00);  // byte 1
      buff.push_back(0x00);  // byte 2
      buff.push_back(0x06);  // byte 3
      buff.push_back(0x01);  // byte 4 transaction type 0x00 for write
      // byte 5 Input the total number of consecutive bytes to write
      // (i.e: total number of consecutive registers to modify)
      buff.push_back(
          total_bytes_loaded);  // byte 5 Number of byte read for address

      // byte 6-9
      buff.push_back((uint8_t)(reg_addr >> 24));
      buff.push_back((uint8_t)(reg_addr >> 16));
      buff.push_back((uint8_t)(reg_addr >> 8));
      buff.push_back((uint8_t)(reg_addr));
      LOG::Debug(TAG, "Write For Read SMBus ");
      break;
    case CommandType::READ:
      buff.push_back(0x00);  // byte 1
      buff.push_back(0x06);  // byte 2
      buff.push_back(0x6A);  // byte 3
      buff.push_back(0x01);  // byte 4 transaction type 0x01 for read
      // byte 5 This is a “don’t care.” Hub will always return 128 bytes
      // of data per read instruction.
      buff.push_back(0x80);  // byte 5 128=0x80
      // byte 6-9
      buff.push_back((uint8_t)(reg_addr >> 24));
      buff.push_back((uint8_t)(reg_addr >> 16));
      buff.push_back((uint8_t)(reg_addr >> 8));
      buff.push_back((uint8_t)(reg_addr));

      LOG::Debug(TAG, "Read SMBus ");
      break;

    default:
      LOG::Warn(TAG, "Unhaandled Command Type : {}", static_cast<int>(type));
      return;
  }
}
void I2CSmartHubManager::PrepareSpecialMessage(SpecialSmbusCommands type,
                                               std::vector<uint8_t> &buff) {
  buff.clear();

  switch (type) {
    case SpecialSmbusCommands::CONFIG_REG_ACCESS:
      buff.push_back(0x99);
      buff.push_back(0x37);
      buff.push_back(0x00);
      break;
    case SpecialSmbusCommands::OTP_PROGRAM:
      buff.push_back(0x99);
      buff.push_back(0x33);
      buff.push_back(0x00);
      break;
    case SpecialSmbusCommands::OTP_READ:
      buff.push_back(0x99);
      buff.push_back(0x34);
      buff.push_back(0x00);
      break;
    case SpecialSmbusCommands::USB_ATTACH:
      buff.push_back(0xAA);
      buff.push_back(0x55);
      buff.push_back(0x00);
      break;
    case SpecialSmbusCommands::USB_ATTACH_WITH_SMB_RUNTIME_ACCESS:
      buff.push_back(0xAA);
      buff.push_back(0x56);
      buff.push_back(0x00);
      break;

    default:
      break;
  }
}

bool I2CSmartHubManager::SendSpecialCmd(SpecialSmbusCommands cmd) {
  /* Run configuration access command */
  std::vector<uint8_t> buff;
  PrepareSpecialMessage(cmd, buff);
  if (!WriteSmbus(buff)) {
    return false;
  }
  return true;
}

uint32_t I2CSmartHubManager::RetrieveRevision() {
  std::vector<uint8_t> buff;
  if (!RegisterRead(USB7252C_DEV_REV, 4, buff)) {
    return -1;
  }
  uint32_t revision = (uint32_t)((buff[1] << 0) + (buff[2] << 8) +
                                 (buff[3] << 16) + (buff[4] << 24));
  return revision;
}
uint16_t I2CSmartHubManager::RetrieveID() {
  std::vector<uint8_t> buff;
  if (!RegisterRead(USB7252C_DEV_ID, 2, buff)) {
    return -1;
  }
  int16_t data = (uint16_t)((buff[1]) + (buff[2] << 8));
  return data;
}
uint32_t I2CSmartHubManager::RetrieveConfiguration() {
  std::vector<uint8_t> buff;
  if (!RegisterRead(USB7252C_HUB_CFG, 3, buff)) {
    return -1;
  }
  uint32_t data = (uint32_t)((buff[1] << 0) + (buff[2] << 8) + (buff[3] << 16) +
                             (buff[4] << 24));

  return data;
}
int I2CSmartHubManager::SetUpstreamPort(uint8_t port) {
  if(!RegisterWrite(USB7252C_HUB_CFG, 1, {port}))
  {
    return -1;
  }
  return 1;
}
int I2CSmartHubManager::SetUsbVID() {}
uint16_t I2CSmartHubManager::RetrieveUsbVID() {}
int I2CSmartHubManager::IsPortActive(uint8_t port) {}
int I2CSmartHubManager::IsPortEnabled(uint8_t port) {}
int I2CSmartHubManager::DisablePort(uint8_t port) {}
int I2CSmartHubManager::SetFlexFeatureRegisters(uint8_t *value) {}
uint16_t I2CSmartHubManager::GetFlexFeatureRegisters() {}
int I2CSmartHubManager::SetPrimaryI2CAddressRegisters(uint8_t *address) {}
uint8_t I2CSmartHubManager::GetPrimaryI2CAddressRegisters() {}
int I2CSmartHubManager::SetSecondryI2CAddressRegisters(uint8_t *address) {}
uint8_t I2CSmartHubManager::GetSecondryI2CAddressRegisters() {}
int I2CSmartHubManager::SetGenericRegister(uint32_t reg, uint8_t *value,
                                           uint8_t count) {}
int I2CSmartHubManager::GetGenericRegister(uint32_t reg, uint8_t *value,
                                           uint8_t count) {}
}  // namespace SmartHub