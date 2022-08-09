

#ifndef SMART_HUB_MANAGER_h
#define SMART_HUB_MANAGER_h

#include <libusb-1.0/libusb.h>
#include <vector>
class SmartHubManager
{
public:
    SmartHubManager();
    void setVidPid(uint16_t vid,uint16_t pid);

    bool Initilize();

    bool RegisterRead(uint32_t address,uint16_t length,std::vector<uint8_t> &buffer);
    bool RegisterWrite(uint32_t address,uint16_t length,const std::vector<uint8_t> &buffer);

     

private:
    // libusb variables
    libusb_device_handle *handle_ = NULL;
    // Context Variable used for initializing LibUSB session
    libusb_context *ctx_ = NULL;

    uint16_t vid_;
    uint16_t pid_;
    int last_error_code=0;
};

#endif // end SMART_HUB_MANAGER_h