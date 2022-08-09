

#include "smart_hub_manager.h"
#include "log/logger.h"
#include "http_client.h"
static constexpr auto TAG{"Main"};


int main(){
SmartHubManager usb;

std::string LOG_PATH=".";
LOG::Init(LOG_PATH);
LOG::Warn(TAG,"hello mohsen");

HttpClient client;
client.NotifyConnection("hello mohsen");

return 1;
}