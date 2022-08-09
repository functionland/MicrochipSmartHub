#include "smart_hub_manager.h"

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log/logger.h"


static constexpr auto TAG{"SmartHubManager"};

SmartHubManager::SmartHubManager(){

}

bool SmartHubManager::Initilize(){
    int error=libusb_init(&ctx_);
    if(error){
        LOG::Fatal(TAG,"Initilize failed {}{%s}", error, libusb_error_name(iError));
        return false;
    }

    LOG::Message(TAG,"Open Usb with vid {} and pid {}",vid_, pid_);

    handle_=libusb_open_device_with_vid_pid(vid_, pid_);
    if(handle_==NULL)
    {
        LOG::Fatal(TAG,"Open device failed");
        libusb_exit(ctx);
        return false;
    }
    LOG::Message("Open device succeeded");
}

void SmartHubManager::setVidPid(int vid,int pid)
{
    vid_=vid;
    pid_=pid;
}
