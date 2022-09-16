

#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <httplib.h>
#include <memory.h>

class HttpClient {
 public:
  HttpClient();
  bool NotifyConnection(std::string_view str);

 private:
  std::shared_ptr<httplib::Client> client_;
};

#endif  // HTTP_CLIENT_H