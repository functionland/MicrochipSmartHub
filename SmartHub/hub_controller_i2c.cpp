#include "hub_controller_i2c.h"

#include "log/logger.h"

// for aceesing smbus port on linux
extern "C" {
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
}
#include <errno.h>
#include <fcntl.h> /* For O_RDWR */
#include <sys/ioctl.h>
#include <unistd.h> /* For open(), creat() */

#include "smart_hub_regs.h"

static constexpr auto TAG{"I2CHubController"};
inline static void debug_buffer(std::vector<uint8_t> &buff)
{
   std::string str;
  for(auto a:buff)
  {
    str+=fmt::format("{:#X},",a);
  }
  LOG::Debug(TAG, "{}",str);
}

namespace SmartHub {
I2CHubController::I2CHubController(const std::string &port_path,
                                   int i2c_address)
    : IHubController(), port_path_{port_path}, i2c_address_{i2c_address} {}

bool I2CHubController::Initialize() {
  if (initialized_) return true;

  file_handle_ = open(port_path_.c_str(), O_RDWR);
  if (file_handle_ < 0) {
    LOG::Warn(TAG, "Can not open i2c path {} with err:", port_path_,
              strerror(errno));
    return false;
  }

  if (ioctl(file_handle_, I2C_SLAVE, i2c_address_) < 0) {
    LOG::Warn(TAG, "Can not specify what device address");
    return false;
  }
  initialized_ = true;
  return true;
}
bool I2CHubController::RegisterRead(uint32_t address, uint16_t length,
                                    std::vector<uint8_t> &data) {
  if (!initialized_) {
    LOG::Warn(TAG, "Not Initilized (RegisterRead) ");
    return false;
  }
  std::lock_guard<std::mutex> lock(lock_);
  std::vector<uint8_t> snd_buff;
  PrepareMessage(CommandType::WRITE_FOR_READ, 2, address, {}, snd_buff);
  if (!WriteSmbus(snd_buff.data(), snd_buff.size())) {
    return false;
  }
  debug_buffer(snd_buff);
  /* Run configuration access command */
  if (!SendSpecialCmd(SpecialCommands::CONFIG_REG_ACCESS)) {
    return false;
  }

  // this command will skip 6 byte header
  std::vector<uint8_t> read = {
      0x00,  // Buffer address MSB: always 0
      0x06,
  };
  if (!WriteSmbus(read.data(), read.size())) {
    return false;
  }
  debug_buffer(read);

  if (!ReadSmbus(data)) {
    return false;
  }
  return true;
}

bool I2CHubController::RegisterWrite(uint32_t address, uint16_t length,
                                     const std::vector<uint8_t> &data) {
  if (!initialized_) {
    LOG::Warn(TAG, "Not Initilized (RegisterWrite) ");
    return false;
  }
  std::lock_guard<std::mutex> lock(lock_);

  std::vector<uint8_t> buff;
  PrepareMessage(CommandType::WRITE_FOR_WRITE, data.size(), address, data,
                 buff);
  if (!WriteSmbus(buff.data(), buff.size())) {
    return false;
  }
  /* Run configuration access command */
  if (!SendSpecialCmd(SpecialCommands::CONFIG_REG_ACCESS)) {
    return false;
  }
  return true;
}
bool I2CHubController::SetLogicalMapping(uint8_t phy_port, uint8_t logic_from,
                                         uint8_t logic_to) {
  return false;
}

int I2CHubController::SetUpstreamPort(uint8_t port) { return HUB_ERR; }
uint8_t I2CHubController::GetLogicalMapping(uint8_t phy_port) { return 0; }

// bool I2CHubController::SetPortConfiguration(uint8_t phy_port,
//                                             UsbConfiguration config) {
//   switch (config) {
//     case UsbConfiguration::USB_DOWNSTREAM:
//       if (!RegisterWrite(HUB_CFG, 1, {phy_port})) {
//         return false;
//       }
//       return true;
//     case UsbConfiguration::USB_UPSTREAM:
//       break;
//     case UsbConfiguration::USB_POWER_DELIVERY:
//       break;
//     case UsbConfiguration::USB_POWER_SAVING:
//       break;
//     default:
//       break;
//   }
//   return false;
// }
// UsbConfiguration GetPortConfiguration(uint8_t phy_port) {
//   return UsbConfiguration::USB_DOWNSTREAM;
// }

bool I2CHubController::CloseEverything() {
  if (close(file_handle_) < 0) {
    LOG::Warn(TAG, "Can not close i2c port");
    return false;
  }
  initialized_ = false;
  return true;
}

std::string I2CHubController::Name() { return MxStr("I2C {}", port_path_); }

bool I2CHubController::WriteSmbus(const unsigned char *buff, int size) {
  if (write(file_handle_, buff, size) < 0) {
    LOG::Warn(TAG, "Can not write to reg with error {} ", strerror(errno));
    return false;
  }
  return true;
}
bool I2CHubController::ReadSmbus(std::vector<uint8_t> &buff) {
  static constexpr auto kMaxSize = 32;
  unsigned char rbuf[kMaxSize];
  int count=read(file_handle_, rbuf, kMaxSize);
  if ( count< 0) {
    LOG::Warn(TAG, "Can not read from reg with error {} ", strerror(errno));
    return false;
  }
  for (int i = 0; i < count; i++) buff.push_back(rbuf[i]);
  debug_buffer(buff);
  return true;
}

void I2CHubController::PrepareMessage(CommandType type,
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
      debug_buffer(buff);
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
      // byte 5 Number of byte read for address
      buff.push_back(total_bytes_loaded);

      // byte 6-9
      buff.push_back((uint8_t)(reg_addr >> 24));
      buff.push_back((uint8_t)(reg_addr >> 16));
      buff.push_back((uint8_t)(reg_addr >> 8));
      buff.push_back((uint8_t)(reg_addr));
      // LOG::Debug(TAG, "Write For Read SMBus ");
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

      // LOG::Debug(TAG, "Read SMBus ");
      break;

    default:
      LOG::Warn(TAG, "Unhaandled Command Type : {}", static_cast<int>(type));
      buff.clear();
      return;
  }
}

bool I2CHubController::SendSpecialCmd(SpecialCommands cmd) {
  if (!initialized_) {
    LOG::Warn(TAG, "Not Initilized (SendSpecialCmd)  {}",cmd);
    return false;
  }
  /* Run configuration access command */
  static constexpr int kSize = 3;
  unsigned char buff[kSize];
  switch (cmd) {
    case SpecialCommands::CONFIG_REG_ACCESS:
      buff[0] = 0x99;
      buff[1] = 0x37;
      buff[2] = 0x00;
      break;
    case SpecialCommands::OTP_PROGRAM:
      buff[0] = 0x99;
      buff[1] = 0x33;
      buff[2] = 0x00;
      break;
    case SpecialCommands::OTP_READ:
      buff[0] = 0x99;
      buff[1] = 0x34;
      buff[2] = 0x00;
      break;
    case SpecialCommands::USB_ATTACH:
      buff[0] = 0xAA;
      buff[1] = 0x55;
      buff[2] = 0x00;
      break;
    case SpecialCommands::USB_ATTACH_WITH_SMB_RUNTIME_ACCESS:
      buff[0] = 0xAA;
      buff[1] = 0x56;
      buff[2] = 0x00;
      break;

    default:
      break;
  }
  if (!WriteSmbus(buff, kSize)) {
    return false;
  }
  return true;
}

uint32_t I2CHubController::Revision() {
  std::vector<uint8_t> buff;
  if (!RegisterRead(DEV_REV, 4, buff)) {
    return HUB_ERR;
  }
  uint32_t revision = (uint32_t)((buff[1] << 0) + (buff[2] << 8) +
                                 (buff[3] << 16) + (buff[4] << 24));
  return revision;
}
uint16_t I2CHubController::RetrieveID() {
  std::vector<uint8_t> buff;
  if (!RegisterRead(DEV_ID, 2, buff)) {
    return HUB_ERR;
  }
  int16_t data = (uint16_t)((buff[1]) + (buff[2] << 8));
  return data;
}
uint32_t I2CHubController::RetrieveConfiguration() {
  std::vector<uint8_t> buff;
  if (!RegisterRead(HUB_CFG, 3, buff)) {
    return HUB_ERR;
  }
  uint32_t data = (uint32_t)((buff[1] << 0) + (buff[2] << 8) + (buff[3] << 16) +
                             (buff[4] << 24));

  return data;
}

int I2CHubController::SetUsbVID(uint8_t vid1, uint8_t vid2) {
  if (!RegisterWrite(VENDOR_ID, 2, {vid1, vid2})) {
    return HUB_ERR;
  }
  return 1;
}

static std::string LinkStateToString(uint8_t state) {
  switch (state) {
    case 0x00:
      return "normal";
    case 0x01:
      return "sleep";
    case 0x02:
      return "suspend";
    case 0x03:
      return "off";
    default:
      break;
  };
  return "invalid";
}

uint16_t I2CHubController::RetrieveUsbVID() {
  std::vector<uint8_t> buff;
  if (!RegisterRead(VENDOR_ID, 2, buff)) {
    return HUB_ERR;
  }
  uint16_t vid = (uint16_t)(buff[2] << 8) + buff[1];
  LOG::Debug(TAG,"vid of hub is: {:#X}",vid);
 
  return vid;
}
int I2CHubController::IsPortActive(uint8_t port) {
  std::vector<uint8_t> buff;
  if (!RegisterRead((port < 4) ? USB2_LINK_STATE0_3 : USB2_LINK_STATE4_7, 2,
                    buff)) {
    return HUB_ERR;
  }
  int portstat = buff[1];

  /* Temporary debug output... */
  LOG::Debug(TAG, "Port %d: %s", (port < 4) ? 0 : 4,
             LinkStateToString(portstat & 0x3));
  LOG::Debug(TAG, "Port %d: %s", (port < 4) ? 1 : 5,
             LinkStateToString((portstat & 0xC) >> 2));
  LOG::Debug(TAG, "Port %d: %s", (port < 4) ? 2 : 6,
             LinkStateToString((portstat & 0x30) >> 4));
  LOG::Debug(TAG, "Port %d: %s", (port < 4) ? 3 : 7,
             LinkStateToString((portstat & 0xC0) >> 6));
  /* End of ugly debug output */

  return portstat;
}
int I2CHubController::IsPortEnabled(uint8_t port) {
  /*
   * Bit 7 = 1; PHYSICAL Port 7 is disabled.
   * Bit 6 = 1; PHYSICAL Port 6 is disabled.
   * Bit 5 = 1; PHYSICAL Port 5 is disabled.
   * Bit 4 = 1; PHYSICAL Port 4 is disabled.
   * Bit 3 = 1; PHYSICAL Port 3 is disabled.
   * Bit 2 = 1; PHYSICAL Port 2 is disabled.
   * Bit 1 = 1; PHYSICAL Port 1 is disabled.
   * Bit 0 = Reserved, always = ‘0’
   */

  std::vector<uint8_t> buff;
  if (!RegisterRead(PORT_DIS_SELF, 2, buff)) {
    return HUB_ERR;
  }
  LOG::Debug(TAG, "Port Disable Self-Powered: 0x%x, return value: %d", buff[1],
             0);

  if (!RegisterRead(PORT_DIS_BUS, 2, buff)) {
    return HUB_ERR;
  }

  LOG::Debug(TAG, "Port Disable Bus-Powered: 0x%x, return value: %d", buff[1],
             0);

  return 0;
}
int I2CHubController::DisablePort(uint8_t port) { return 0; }
int I2CHubController::SetFlexFeatureRegisters(uint16_t value) {
  std::vector<uint8_t> buff;
  buff.push_back(value << 8);
  buff.push_back(value);
  if (!RegisterWrite(FLEX_FEATURE_REG, 1, buff)) {
    return HUB_ERR;
  }
  return 1;
}
uint16_t I2CHubController::GetFlexFeatureRegisters() {
  std::vector<uint8_t> buff;
  if (!RegisterRead(FLEX_FEATURE_REG, 2, buff)) {
    return HUB_ERR;
  }
  uint16_t data = (uint16_t)((buff[1]) + (buff[2] << 8));
  return data;
}
int I2CHubController::SetPrimaryI2CAddressRegisters(uint16_t address) {
  std::vector<uint8_t> buff;
  buff.push_back(address << 8);
  buff.push_back(address);
  if (!RegisterWrite(SMBUS_PRIMAIRY_ADR, 1, buff)) {
    return HUB_ERR;
  }
  return 1;
}
uint8_t I2CHubController::GetPrimaryI2CAddressRegisters() {
  std::vector<uint8_t> buff;
  if (!RegisterRead(SMBUS_PRIMAIRY_ADR, 1, buff)) {
    return HUB_ERR;
  }
  return buff[1];
}
int I2CHubController::SetSecondryI2CAddressRegisters(uint16_t address) {
  std::vector<uint8_t> buff;
  buff.push_back(address << 8);
  buff.push_back(address);
  if (!RegisterWrite(SMBUS_SECOND_ADR, 1, buff)) {
    return HUB_ERR;
  }
  return 1;
}
uint8_t I2CHubController::GetSecondryI2CAddressRegisters() {
  std::vector<uint8_t> buff;
  if (!RegisterRead(SMBUS_SECOND_ADR, 1, buff)) {
    return HUB_ERR;
  }
  return buff[1];
}

}  // namespace SmartHub