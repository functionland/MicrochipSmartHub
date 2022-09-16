

#ifndef USB_SMART_HUB_MANAGER_H
#define USB_SMART_HUB_MANAGER_H

#include "smart_hub_manager.h"

#include <libusb-1.0/libusb.h>

class UsbSmartHubManager:public SmartHubManager
{
public:
    UsbSmartHubManager();
    ~UsbSmartHubManager();
    void SetVidPid(uint16_t vid,uint16_t pid);

    bool Initialize() override;

    bool RegisterRead(uint32_t address,uint16_t length,std::vector<uint8_t> &buffer)override;
    bool RegisterWrite(uint32_t address,uint16_t length,const std::vector<uint8_t> &buffer)override;

    bool PortMappingUsb2(std::array<uint8_t,7> port_map)override;
    bool PortMappingUsb3(std::array<uint8_t,7> port_map)override;

    /// @brief  Reset SmartHub from usb PC0
    /// • Only one device can be accessed at a time
    /// • No support is provided when multiple HFCs with same VID and PID are present
    /// @return true if successfull
    bool Reset()override;

    bool CloseEverything()override;

private:
    // libusb variables
    libusb_device_handle *handle_ = NULL;
    // Context Variable used for initializing LibUSB session
    libusb_context *ctx_ = NULL;

    uint16_t vid_=0x0424;
    uint16_t pid_=0x7240;
    int last_error_code=0;
};

#endif // end USB_SMART_HUB_MANAGER_H