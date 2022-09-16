
#include "http_client.h"

#include "log/logger.h"

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 8083
#define CA_CERT_FILE "./ca-bundle.crt"

static constexpr auto TAG{"HttpClient"};

HttpClient::HttpClient() {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  client_ = std::make_shared<httplib::Client>(SERVER_ADDRESS, SERVER_PORT);
  client_->set_ca_cert_path(CA_CERT_FILE);
  client_->enable_server_certificate_verification(true);
#else
  client_ = new HttpClient(SERVER_ADDRESS, SERVER_PORT);
#endif
}

bool HttpClient::NotifyConnection(std::string_view str) {
  if (client_ == nullptr) {
    LOG::Warn(TAG, "NotifyConnection: client is null");
    return false;
  }

  std::string json = MxStr("{{ \"port\": \"{}\"}}", str);
  if (auto res = client_->Post("/notify", json, "application/json")) {
    if (res->status == 200)
      LOG::Message(TAG, " Post notification Successfully sent");
    else {
      LOG::Warn(TAG, " Post notification Error {}", res->status);
    }
  } else {
    LOG::Warn(TAG, "Post notification Failure for {}",
              httplib::to_string(res.error()));
    return false;
  }

  return true;
}