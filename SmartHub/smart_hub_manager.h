

#ifndef SMART_HUB_MANAGER_h
#define SMART_HUB_MANAGER_h

#include <libusb-1.0/libusb.h>
#include <vector>
class SmartHubManager
{
public:
    SmartHubManager();
    ~SmartHubManager();
    void SetVidPid(uint16_t vid,uint16_t pid);

    bool Initialize();

    bool RegisterRead(uint32_t address,uint16_t length,std::vector<uint8_t> &buffer);
    bool RegisterWrite(uint32_t address,uint16_t length,const std::vector<uint8_t> &buffer);

    bool PortMappingUsb2(std::array<uint8_t,7> port_map);
    bool PortMappingUsb3(std::array<uint8_t,7> port_map);

    bool CloseEverything();

private:
    // libusb variables
    libusb_device_handle *handle_ = NULL;
    // Context Variable used for initializing LibUSB session
    libusb_context *ctx_ = NULL;

    uint16_t vid_=0x0424;
    uint16_t pid_=0x7240;
    int last_error_code=0;
};

#endif // end SMART_HUB_MANAGER_h