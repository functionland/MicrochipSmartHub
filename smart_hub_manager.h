

#ifndef SMART_HUB_MANAGER_h
#define SMART_HUB_MANAGER_h

#include <libusb-1.0/libusb.h>

class SmartHubManager
{
public:
    SmartHubManager();
    void setVidPid(int vid,int pid);

    bool Initilize();
     

private:
    // libusb variables
    libusb_device_handle *handle_ = NULL;
    // Context Variable used for initializing LibUSB session
    libusb_context *ctx_ = NULL;

    int vid_;
    int pid_;
};

#endif // end SMART_HUB_MANAGER_h