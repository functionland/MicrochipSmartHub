

#include "usb_smart_hub_manager.h"
#include "i2c_smart_hub_manager.h"

#include "log/logger.h"
#include "http_client.h"
static constexpr auto TAG{"Main"};


int main(){
UsbSmartHubManager usb;

std::string LOG_PATH=".";
LOG::Init(LOG_PATH);
LOG::Warn(TAG,"hello mohsen");

HttpClient client;
client.NotifyConnection("hello mohsen");

usb.SetVidPid(0x0424,0x7240);
usb.Initialize();

std::array<uint8_t, 7> port_map={0,0,0,0,
                                 0,0,0};
usb.PortMappingUsb2(port_map);

return 1;
}