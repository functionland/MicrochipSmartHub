#pragma once

#include <stdint.h>

namespace SmartHub {
enum UsbType {
  USB_TYPE_2,
  USB_TYPE_3,
};
enum UsbBatteryChargingMode{
    BAT_CHARGING_DISABLED = 0,
    BAT_CHARGING_500mA,
    BAT_CHARGING_1_5A,
    BAT_CHARGING_2A,
};

class Usbport {
 public:
  Usbport(UsbType type, uint8_t physical_num);
  ~Usbport() = default;

  uint8_t physical_num() const { return physical_num_; }
  
  void set_logical_num(uint8_t num) { logical_num_ = num; }
  uint8_t logical_num() const { return logical_num_; }

  bool enabled()const {return enabled_; }
  void set_enabled(bool enabled) { enabled_ = enabled; }

  bool removable()const {return removable_; }
  void set_removable(bool removable) { removable_ = removable; }

  UsbBatteryChargingMode battery_charging_mode() const { return battery_charging_mode_; }
  void set_battery_charging_mode(UsbBatteryChargingMode charging){battery_charging_mode_= charging;}

  bool is_downstream() const { return is_downstream_; }
  void set_is_downstream(bool is_downstream){is_downstream_=is_downstream;}

  bool power_saving_mode() const { return power_saving_mode_; }
  void set_power_saving_mode(bool enabled){power_saving_mode_=enabled_;}

  bool flex_connect_enabled()const {return flex_connect_enabled_; }
  void set_flex_connect_enabled(bool enabled){flex_connect_enabled_=enabled;}

 private:
  /// @brief The PHYSICAL port number is the port number with respect to the
  /// physical PHY on the chip. PHYSICAL port numbering is fixed and the
  /// settings are not impacted by LOGICAL port renumbering/remapping
  const uint8_t physical_num_;
  uint8_t logical_num_;
  UsbType type_;
  bool enabled_{true};
  bool removable_{true};
  UsbBatteryChargingMode battery_charging_mode_{BAT_CHARGING_DISABLED};
  bool is_downstream_{true};
  bool power_saving_mode_{false};
  bool flex_connect_enabled_{true};
};
}