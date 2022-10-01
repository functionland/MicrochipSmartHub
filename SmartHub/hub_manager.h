#pragma once

#include <atomic>
#include <memory>
#include <thread>
#include <vector>

#include "http_client.h"
#include "hub_controller.h"

namespace SmartHub {
class HubManager final {
 public:
  HubManager() = default;
  ~HubManager();

  void ResetAllHubs();
  void AddHubController(std::shared_ptr<IHubController> controller);
  void SetNotifyClient(std::shared_ptr<HttpClient> client);
  void Start();

 private:
  static void Worker(HubManager * manager);
  std::vector<std::shared_ptr<IHubController>> hubs_;
  std::shared_ptr<HttpClient> client_;
  std::unique_ptr<std::thread> main_thread_;
  std::atomic<bool> runnig_{false};
};
}  // namespace SmartHub