#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <httplib.h>
#include <memory.h>

namespace SmartHub {
class HttpClient {
 public:
  HttpClient();
  bool NotifyConnection(std::string_view str);

 private:
  std::shared_ptr<httplib::Client> client_;
};
}  // namespace SmartHub
