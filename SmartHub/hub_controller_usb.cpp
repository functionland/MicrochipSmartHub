#include "hub_controller_usb.h"

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log/logger.h"

#define CMD_MEMORY_READ 0x04
#define CMD_MEMORY_WRITE 0x03
#define REGISTER_READ 1
#define REGISTER_WRITE 2

// port mappings registers
#define PORT_PWR_STAT_UDC20_SLEEP_SUSPEND 0xBF8030E8
#define DYNAMIC_PRT_REMAP_EN 0x40
#define PRT_REMAP_EN 0x08
#define CONFIG_DONE_DYNAMIC 0x80

#define HUB_CFG_3 0xBF803008
#define HUB_PRT_REMAP 0xBF8030FB
#define USB3_PRT_REMAP_EN_REG 0xBF803858
#define USB3_PRT_REMAP_EN 0x01
#define USB3_NUMBER_OF_PORTS 0xBF8090CA

#define DEVICE_RESET 0x001
#define CMD_DEV_RESET 0x29

// port mappings usb3
#define USB3_PRT_REMAP_REG_LOCK 0xBF803864
#define USB3_PRT_REMAP_REG_LOCK_BIT 0x01

#define CTRL_TIMEOUT (5 * 1000) /* milliseconds */

static constexpr auto TAG{"UsbHubController"};

namespace SmartHub {

UsbHubController::UsbHubController() : IHubController() {}

bool UsbHubController::Initialize() {
  int error = libusb_init(&ctx_);
  if (error) {
    LOG::Fatal(TAG, "Initialize failed {}{%s}", error,
               libusb_error_name(error));
    return false;
  }

  LOG::Message(TAG, "Open Usb with vid {0:#x} and pid {0:#x}", vid_, pid_);

  handle_ = libusb_open_device_with_vid_pid(ctx_, vid_, pid_);
  if (handle_ == NULL) {
    LOG::Fatal(TAG, "Open device failed");
    libusb_exit(ctx_);
    return false;
  }
  LOG::Message(TAG, "Open device succeeded");
  return true;
}

bool UsbHubController::RegisterRead(uint32_t address, uint16_t length,
                                    std::vector<uint8_t> &buffer) {
  if (handle_ == NULL) {
    LOG::Warn(TAG, "RegisterRead Handle is NULL");
    return false;
  }

  uint8_t buf[1024] = "\0";
  if (length > 1024) {
    LOG::Warn(TAG, "Read length is too large {}", length);
    return false;
  }
  for (int i = 0; i < length; i++) {
    buf[i] = buffer[i];
  }
  last_error_code = libusb_control_transfer(
      handle_,
      LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR |
          LIBUSB_RECIPIENT_INTERFACE,
      CMD_MEMORY_READ, (address & 0xFFFF), ((address & 0xFFFF0000) >> 16), buf,
      length, CTRL_TIMEOUT);
  if (last_error_code < 0) {
    LOG::Warn(TAG, "Can not read register address {}", address);
    return false;
  }

  for (uint16_t i = 1; i <= length; i++) {
    buffer.push_back(buf[i]);
  }
  return true;
}
bool UsbHubController::RegisterWrite(uint32_t address, uint16_t length,
                                     const std::vector<uint8_t> &buffer) {
  if (handle_ == NULL) {
    LOG::Warn(TAG, "RegisterWrite Handle is NULL");
    return false;
  }
  uint8_t buf[1024] = "\0";
  for (int i = 0; i < buffer.size(); i++) buf[i] = buffer[i];

  last_error_code = libusb_control_transfer(
      handle_,
      LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_VENDOR |
          LIBUSB_RECIPIENT_INTERFACE,
      CMD_MEMORY_WRITE, (address & 0xFFFF), ((address & 0xFFFF0000) >> 16), buf,
      length, CTRL_TIMEOUT);
  if (last_error_code < 0) {
    LOG::Warn(TAG, "Can not write register address {}", address);
    return false;
  }
  return true;
}

std::string UsbHubController::Name() {
  return MxStr("USB hub pid:{} vid:{}", pid_, vid_);
}
bool UsbHubController::SetLogicalMapping(uint8_t phy_port, uint8_t logic_from,
                                         uint8_t logic_to) {
  return false;
}
bool UsbHubController::SetPortConfiguration(uint8_t phy_port,
                                            UsbConfiguration config) {
  return false;
}
// bool UsbHubController::PortMappingUsb2(std::array<uint8_t, 7> port_map) {
//   if (handle_ == NULL) {
//     LOG::Warn(TAG, "PortMappingUsb2 Handle is NULL");
//     return false;
//   }

//   std::vector<uint8_t> buffer(4);

//   LOG::Message(TAG, "USB2.0 Dynamic Port Mapping");

//   LOG::Message(TAG, "Read 0xBF8030E8 register");
//   if (!RegisterRead(PORT_PWR_STAT_UDC20_SLEEP_SUSPEND, 1, buffer)) {
//     LOG::Warn(TAG, "Port power status last error: {}",
//               libusb_error_name(last_error_code));
//     return false;
//   }

//   LOG::Message(TAG, "Set bit 6 of 0xBF8030E8 register");
//   buffer[0] |= DYNAMIC_PRT_REMAP_EN;
//   if (!RegisterWrite(PORT_PWR_STAT_UDC20_SLEEP_SUSPEND, 1, buffer)) {
//     LOG::Warn(TAG, "Port power status last error: {}",
//               libusb_error_name(last_error_code));
//     return false;
//   }

//   LOG::Message(TAG, "Set bit 3 (PRT_REMAP_EN) of XDATA register HUB_CFG_3");
//   if (!RegisterRead(HUB_CFG_3, 1, buffer)) {
//     LOG::Warn(TAG, "Port power status last error: {}",
//               libusb_error_name(last_error_code));
//     return false;
//   }

//   LOG::Message(TAG, "Set bit 3 of 0xBF803008 register");
//   buffer[0] |= PRT_REMAP_EN;
//   if (!RegisterWrite(HUB_CFG_3, 1, buffer)) {
//     LOG::Warn(TAG, "Port power status last error: {}",
//               libusb_error_name(last_error_code));
//     return false;
//   }

//   LOG::Message(TAG,
//                "Write New USB2 Port mapping values to HUB_PRT_REMAP registers
//                "
//                "(0xBF80_30FB – 0xBF80_30FE)");

//   std::vector<uint8_t> final_port_map(4);
//   // 0xBF8030FB
//   final_port_map[0] = ((port_map[1] << 4) + (port_map[0] & 0x0F));
//   // 0xBF8030FC
//   final_port_map[1] = ((port_map[3] << 4) + (port_map[2] & 0x0F));
//   // 0xBF8030FD
//   final_port_map[2] = ((port_map[5] << 4) + (port_map[4] & 0x0F));
//   // 0xBF8030FE
//   final_port_map[3] = 0x00 + (port_map[6] & 0x0F);

//   if (!RegisterWrite(HUB_PRT_REMAP, 4, final_port_map)) {
//     LOG::Warn(TAG, "Port power status last error: {}",
//               libusb_error_name(last_error_code));
//     return false;
//   }

//   LOG::Message(TAG, "Set bit 7 (CONFIG_DONE_DYNAMIC) of XDATA register");
//   if (!RegisterRead(PORT_PWR_STAT_UDC20_SLEEP_SUSPEND, 1, buffer)) {
//     LOG::Warn(TAG,
//               "Read bit 7 (CONFIG_DONE_DYNAMIC) of XDATA register error: {}",
//               libusb_error_name(last_error_code));
//     return false;
//   }

//   LOG::Message(TAG, "Set bit 7 of 0xBF8030E8 reg");
//   buffer[0] |= CONFIG_DONE_DYNAMIC;
//   if (!RegisterWrite(HUB_CFG_3, 1, buffer)) {
//     LOG::Warn(TAG, "Register Write Failed.Error: {}",
//               libusb_error_name(last_error_code));
//     return false;
//   }
//   return true;
// }

// bool UsbHubController::PortMappingUsb3(std::array<uint8_t, 7> port_map) {
//   if (handle_ == NULL) {
//     LOG::Warn(TAG, "PortMappingUsb3 Handle is NULL");
//     return false;
//   }
//   std::vector<uint8_t> buffer(4);
//   LOG::Message(TAG, "USB3.1 GEN2 Dynamic Port mapping");

//   LOG::Message(
//       TAG,
//       "Unlock/Clear Bit 0 of 0xBF80_3864 register
//       (USB3_PRT_REMAP_REG_LOCK)");

//   LOG::Message(TAG, "USB3 Reset unlock bit (Bit 0 of 0xBF803864 register)");
//   if (!RegisterRead(USB3_PRT_REMAP_REG_LOCK, 1, buffer)) {
//     LOG::Warn(TAG, "Reset unlock bit (Bit 0 of 0xBF803864 register) error:
//     {}",
//               libusb_error_name(last_error_code));
//     return false;
//   }

//   LOG::Message(TAG, "USB3 Set bit 7 of 0xBF8030E8 reg");
//   buffer[0] |= ~USB3_PRT_REMAP_REG_LOCK_BIT;
//   if (!RegisterWrite(USB3_PRT_REMAP_REG_LOCK, 1, buffer)) {
//     LOG::Warn(TAG, "Set bit 7 of 0xBF8030E8 reg error: {}",
//               libusb_error_name(last_error_code));
//     return false;
//   }

//   LOG::Message(
//       TAG,
//       "Set Bit 0 (USB3_PRT_REMAP_EN) of USB3_PRT_REMAP register
//       (0xBF80_3858)");
//   if (!RegisterRead(USB3_PRT_REMAP_EN_REG, 1, buffer)) {
//     LOG::Warn(TAG, "Register Read Error: {}",
//               libusb_error_name(last_error_code));
//     return false;
//   }

//   LOG::Message(TAG, "Set bit 0 (USB3_PRT_REMAP_EN)");
//   buffer[0] |= USB3_PRT_REMAP_EN;
//   if (!RegisterWrite(USB3_PRT_REMAP_EN_REG, 1, buffer)) {
//     LOG::Warn(TAG, "Register Write Error: {}",
//               libusb_error_name(last_error_code));
//     return false;
//   }

//   LOG::Message(TAG,
//                "Write new usb3 port mapping values to USB3_PRT_REMAP
//                registers "
//                "(0xBF80_3860 – 0xBF80_3863)");

//   std::vector<uint8_t> final_port_map(4);
//   final_port_map[0] =
//       ((port_map[1] << 4) + (port_map[0] & 0x0F));  // 0xBF803860
//   final_port_map[1] =
//       ((port_map[3] << 4) + (port_map[2] & 0x0F));  // 0xBF803861
//   final_port_map[2] =
//       ((port_map[5] << 4) + (port_map[4] & 0x0F));  // 0xBF803862
//   final_port_map[3] = 0x00 + (port_map[6] & 0x0F);  // 0xBF803863

//   if (!RegisterWrite(USB3_NUMBER_OF_PORTS, 4, final_port_map)) {
//     LOG::Warn(TAG, "Register Write Error: {}",
//               libusb_error_name(last_error_code));
//     return false;
//   }

//   LOG::Message(
//       TAG, "Lock/Set Bit 0 of 0xBF80_3864 register
//       (USB3_PRT_REMAP_REG_LOCK)");
//   if (!RegisterRead(USB3_PRT_REMAP_REG_LOCK, 1, buffer)) {
//     LOG::Warn(TAG, "Register Read Error: {}",
//               libusb_error_name(last_error_code));
//     return false;
//   }

//   LOG::Message(TAG, "Set the lock bit (Bit 0 of 0xBF803864 register)");
//   buffer[0] |= USB3_PRT_REMAP_REG_LOCK_BIT;
//   if (!RegisterWrite(USB3_PRT_REMAP_REG_LOCK, 1, buffer)) {
//     LOG::Warn(TAG, "Register Write Error: {}",
//               libusb_error_name(last_error_code));
//     return false;
//   }

//   return true;
// }

bool UsbHubController::Reset() {
  if (handle_ == NULL) {
    LOG::Warn(TAG, "RegisterWrite Handle is NULL");
    return false;
  }
  last_error_code = libusb_control_transfer(
      handle_,
      LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_VENDOR |
          LIBUSB_RECIPIENT_INTERFACE,
      CMD_DEV_RESET, DEVICE_RESET, 0, 0, 0, CTRL_TIMEOUT);
  if (last_error_code < 0) {
    LOG::Warn(TAG, "Can not write register address {}",
              libusb_error_name(last_error_code));
    return false;
  }
  return true;
}

void UsbHubController::SetVidPid(uint16_t vid, uint16_t pid) {
  vid_ = vid;
  pid_ = pid;
  CloseEverything();
  Initialize();
}

bool UsbHubController::CloseEverything() {
  if (handle_ != NULL) {
    libusb_close(handle_);
    handle_ = NULL;
  }
  if (ctx_ != NULL) {
    libusb_exit(ctx_);
    ctx_ = NULL;
  }
  return true;
}

UsbHubController::~UsbHubController() { CloseEverything(); }
}  // namespace SmartHub