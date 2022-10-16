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
    LOG::Debug(TAG, "Add Hub Controller {}", controller->Name());
  } else {
    LOG::Warn(TAG, "Null Hub Controller");
  }
}

void HubManager::Worker() {
  LOG::Debug(TAG, "HUB Manager Is Starting ...");

  ResetAllHubs();
  while (runnig_) {
    /// Force Configuration Mode

    /// Check port status for start configuration

    /// if need to restart

    ///

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  runnig_ = false;
}
void HubManager::Start() {
  main_thread_ = std::make_unique<std::thread>(&HubManager::Worker, this);
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

void HubManager::InitilizePorts() {
  for (int i = 0; i < 4; i++) {
    auto usb = std::make_shared<Usbport>(USB_TYPE_3_DN, i);
  }
  auto usb = std::make_shared<Usbport>(USB_TYPE_2_DN, 5);
  
}

}  // namespace SmartHub