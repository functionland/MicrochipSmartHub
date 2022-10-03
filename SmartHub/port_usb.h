#pragma once

#include <stdint.h>

namespace SmartHub {
enum UsbType {
  USB_TYPE_2,
  USB_TYPE_3,
};

enum UsbConfiguration {
  USB_DOWNSTREAM,
  USB_UPSTREAM,
  USB_POWER_SAVING,
  USB_POWER_DELIVERY,
};

class Usbport {
 public:
  Usbport(UsbType type, uint8_t physical_num);
  ~Usbport() = default;
  UsbConfiguration configuration() const { return config_; }
  void set_configuration(UsbConfiguration configuration);

  uint8_t physical_num() const { return physical_num_; }
  void set_logical_num(uint8_t num) { logical_num_ = num; }
  uint8_t logical_num() const { return logical_num_; }

 private:
  /// @brief The PHYSICAL port number is the port number with respect to the
  /// physical PHY on the chip. PHYSICAL port numbering is fixed and the
  /// settings are not impacted by LOGICAL port renumbering/remapping
  const uint8_t physical_num_;
  uint8_t logical_num_;
  UsbType type_;
  UsbConfiguration config_;
};
}