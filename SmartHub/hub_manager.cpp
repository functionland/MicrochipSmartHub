#include "hub_manager.h"

#include "log/logger.h"

static constexpr auto TAG{"HubManager"};

namespace SmartHub {
HubManager::~HubManager() {
  runnig_ = false;
  LOG::Warn(TAG, "Hub Controller is Shutting down ...");
}
void HubManager::AddHubController(std::shared_ptr<IHubController> controller) {
  if (controller) {
    hubs_.push_back(controller);
    LOG::Debug(TAG, "Hub Controller", controller->Name());
  } else {
    LOG::Warn(TAG, "Null Hub Controller");
  }
}

void HubManager::Worker() {
  LOG::Debug(TAG, "HUB Manager Is Starting ...");

  ResetAllHubs(); 
  while (runnig_) {
    
    
    /// SetConfiguration for each hub

    /// Check port status for start configuration

    /// if need to restart

    /// 

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
  runnig_ = false;
}
void HubManager::Start() {
  main_thread_ = std::make_unique<std::thread>(&HubManager::Worker,this);
  runnig_ = true;
  main_thread_->join();
}

void HubManager::SetNotificationClient(std::shared_ptr<HttpClient> client) {
  client_ = client;
}

void HubManager::ResetAllHubs() {
  for (auto &hub : hubs_) {
    if (hub) hub->Reset();
  }
}
}  // namespace SmartHub