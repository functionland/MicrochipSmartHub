

#include "smart_hub_manager.h"
#include "log/logger.h"
int main(){
SmartHubManager usb;

std::string LOG_PATH=".";
LOG::Init(LOG_PATH);
LOG::Warn("Main","hello mohsen");

return 1;
}