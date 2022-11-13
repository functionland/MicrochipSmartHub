#pragma once

#include <httplib.h>
#include <memory.h>

#include "hub_manager.h"

namespace SmartHub {
class HttpServer {
 public:
  bool Init(int port);
  bool Start();
  void SetHubManager(HubManager *manager);

 private:
  int port_=8083;
  HubManager *hub_manager_{nullptr};
  std::unique_ptr<httplib::Server> server_;
};
}  // namespace SmartHub