#pragma once

#include <stdint.h>

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "port_usb.h"

namespace SmartHub {

enum CommandType {
  READ,
  WRITE_FOR_WRITE,
  WRITE_FOR_READ,
};
enum SpecialCommands {
  CONFIG_REG_ACCESS,                   // 0X9937  Run memory command
  USB_ATTACH,                          // 0XAA55  Go to runtime without SMBus
  USB_ATTACH_WITH_SMB_RUNTIME_ACCESS,  // 0XAA56  Go to runtime with SMBus
  OTP_PROGRAM,  // 0X9933  Permanently program configuration commands to the OTP
  OTP_READ,     // 0X9934  Read the values of the OTP register
};

class IHubController {
 public:
  virtual bool Initialize() = 0;

  virtual bool RegisterRead(uint32_t address, uint16_t length,
                            std::vector<uint8_t> &buffer) = 0;
  virtual bool RegisterWrite(uint32_t address, uint16_t length,
                             const std::vector<uint8_t> &buffer) = 0;
  virtual bool SendSpecialCmd(SpecialCommands cmd) = 0;

  virtual bool SetLogicalMapping(uint8_t phy_port, uint8_t logic_from,
                                 uint8_t logic_to) = 0;
  virtual uint8_t GetLogicalMapping(uint8_t phy_port) = 0;
  // virtual bool SetPortConfiguration(uint8_t phy_port,UsbConfiguration config)
  // = 0; virtual UsbConfiguration GetPortConfiguration(uint8_t phy_port)=0;

  virtual bool Reset();

  virtual bool CloseEverything() = 0;
  virtual std::string Name() = 0;
  virtual uint32_t Revision() = 0;
  virtual uint16_t RetrieveID() = 0;
  virtual uint32_t RetrieveConfiguration() = 0;
  virtual int SetUpstreamPort(uint8_t port) = 0;
  virtual int SetUsbVID(uint8_t usb_vid1, uint8_t usb_vid2) = 0;
  virtual uint16_t RetrieveUsbVID() = 0;
  virtual int IsPortActive(uint8_t port) = 0;
  virtual int IsPortEnabled(uint8_t port) = 0;
  virtual int DisablePort(uint8_t port) = 0;
  virtual int SetFlexFeatureRegisters(uint16_t value) = 0;
  virtual uint16_t GetFlexFeatureRegisters() = 0;

  void AddUsbPort(std::shared_ptr<Usbport> port) { ports_.push_back(port); }

 protected:
  std::vector<std::shared_ptr<Usbport>> ports_;
};
}  // namespace SmartHub
