#pragma once

#include <atomic>
#include <memory>
#include <thread>
#include <vector>

#include "http_client.h"
#include "hub_controller.h"

namespace SmartHub {
enum Stage {
  STG_SPI_INIT,
  STG_CFG_ROM,
  STG_CFG_STRAP,
  STG_SMBUS_CHECK,
  STG_CFG_OTP,
  STG_CFG_SOC,
  STG_NORMAL_MODE,
  STG_UNKNOWN_MODE,
};
class HubManager final {
 public:
  HubManager() = default;
  ~HubManager();

  void ResetAllHubs();
  void AddHubController(std::shared_ptr<IHubController> controller);
  void SetNotificationClient(std::shared_ptr<HttpClient> client);

  void Start();

 private:
  void Worker();
  std::vector<std::shared_ptr<IHubController>> hubs_;
  std::shared_ptr<HttpClient> client_;
  std::unique_ptr<std::thread> main_thread_;
  std::atomic<bool> runnig_{false};
};
}  // namespace SmartHub