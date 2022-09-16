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

static constexpr auto TAG{"I2cSmartHubManager"};

I2CSmartHubManager::I2CSmartHubManager(std::string &port_path, int i2c_address)
    : port_path_{port_path}, i2c_address_{i2c_address} {}

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
  return true;
}

bool I2CSmartHubManager::RegisterRead(uint32_t address, uint16_t length,
                                      std::vector<uint8_t> &buffer) {
  return false;
}
bool I2CSmartHubManager::RegisterWrite(uint32_t address, uint16_t length,
                                       const std::vector<uint8_t> &buffer) {
  return false;
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

bool I2CSmartHubManager::WriteSmbus(uint8_t reg, std::vector<uint8_t> &buff) {
  __u8 wbuf[256];
  for (int i = 0; i < buff.size(); i++) wbuf[i] = buff[i];
  wbuf[buff.size()] = '\0';
  const auto res =
      i2c_smbus_write_block_data(file_handle_, reg, buff.size(), wbuf);
  if (res < 0) {
    LOG::Warn(TAG, "Can not write to reg:{} ", reg);
    return false;
  }
  return true;
}
bool I2CSmartHubManager::ReadSmbus(uint8_t reg, std::vector<uint8_t> &buff) {
  __u8 rbuf[256];
  /* Using SMBus commands */
  const auto res = i2c_smbus_read_block_data(file_handle_, reg, rbuf);
  if (res < 0) {
    LOG::Warn(TAG, "Can not read from reg:{}", reg);
    return false;
  }

  for (int i = 0; i < res; i++) buff.push_back(rbuf[i]);
  return true;
}

void I2CSmartHubManager::PreapareMessage(SmartHubCommandType type,
                                         uint8_t total_bytes_loaded,
                                         uint32_t reg_addr,
                                         std::vector<uint8_t> &buff) {
  buff.clear();
  switch (type) {
    case SmartHubCommandType::WRITE:
      buff.push_back(0x5A);
      buff.push_back(0x00);                    // byte 1
      buff.push_back(0x00);                    // byte 2
      buff.push_back(total_bytes_loaded + 6);  // byte 3
      buff.push_back(0x00);                    // byte 4 transaction type
      // byte 5 Input the total number of consecutive bytes to write
      // (i.e: total number of consecutive registers to modify)
      buff.push_back(total_bytes_loaded);  // byte 5 Register Data Length
      break;
    case SmartHubCommandType::READ:
      buff.push_back(0x5B);  // byte 0
      buff.push_back(0x00);  // byte 1
      buff.push_back(0x06);  // byte 2
      buff.push_back(0x6A);  // byte 3
      buff.push_back(0x01);  // byte 4 transaction type
      // byte 5 This is a “don’t care.” Hub will always return 128 bytes
      // of data per read instruction.
      break;

    default:
      LOG::Warn(TAG, "Unhaandled Command Type : {}", type);
      return;
  }
}
