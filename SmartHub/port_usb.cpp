#include "port_usb.h"
namespace SmartHub {
Usbport::Usbport(UsbType type, uint8_t physical_num)
    : physical_num_(physical_num) {}

void Usbport::set_configuration(UsbConfiguration configuration) {
  config_ = configuration;
};
}  // namespace SmartHub