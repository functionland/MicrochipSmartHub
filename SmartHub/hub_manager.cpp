#include "hub_manager.h"

#include "hub_controller_i2c.h"
#include "log/logger.h"

static constexpr auto TAG{"HubManager"};

namespace SmartHub {
HubManager::~HubManager() {
  runnig_ = false;
  LOG::Warn(TAG, "Hub Controller is Shutting down ...");
}
void HubManager::AddNewI2CController(const std::string &path, int address) {
  std::lock_guard<std::mutex> lock{lock_};
  auto controller = std::make_shared<I2CHubController>(path, address);
  if (controller) {
    controller->Initialize();
    hubs_.push_back(controller);
    LOG::Debug(TAG, "Add Hub Controller {}", controller->Name());
  } else {
    LOG::Warn(TAG, "I2C Hub Controller {} {} is NULL", path, address);
  }
}

void HubManager::Worker() {
  LOG::Debug(TAG, "HUB Manager Is Starting ...");

  ResetAllHubs();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  while (runnig_) {
    // run hub with i2c access
    for (auto &hub : hubs_) {
      if (hub) hub->SendSpecialCmd(USB_ATTACH_WITH_SMB_RUNTIME_ACCESS);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    int count = 0;
    while (count++ < 10) {
      for (auto &hub : hubs_) {
        if (hub) {
          if (hub->SetUsbVID(0x12, 0x34) != HUB_ERR) {
            // hub->Revision();
            hub->RetrieveUsbVID();
          }
          std::this_thread::sleep_for(std::chrono::seconds(20));
        }
      }
    }
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
    auto usb = std::make_shared<Usbport>(USB_TYPE_3, i);
  }
  auto usb = std::make_shared<Usbport>(USB_TYPE_2, 5);
}

}  // namespace SmartHub