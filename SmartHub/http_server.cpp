#include "http_server.h"

#include <httplib.h>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <streambuf>

#include "global.h"
#include "log/logger.h"

static constexpr auto TAG{"HttpServer"};

#define SERVER_CERT_FILE "./cert.pem"
#define SERVER_PRIVATE_KEY_FILE "./key.pem"

using namespace httplib;
static std::string dump_headers(const Headers &headers);
static std::string log(const Request &req, const Response &res);

namespace SmartHub {

void HttpServer::SetSmartHubConfig(const Request &req, Response &resp) {
  auto configs = nlohmann::json::parse(req.body);
  LOG::Warn(TAG, "{}", configs.dump());
}
void HttpServer::GetSmartHubStatus(const Request &req, Response &resp) {
  const char *fmt = "<p>Status: <span style='color:red;'>%d</span></p>";
  char buf[BUFSIZ];
  snprintf(buf, sizeof(buf), fmt, resp.status);
  resp.set_content(buf, "text/html");
}
void HttpServer::GetSmartHubLog(const httplib::Request &req,
                                httplib::Response &resp) {
  const std::string log_path = APP_LOG_PATH;
  std::ifstream file(log_path);
  file.seekg(0, std::ios::end);
  size_t size = file.tellg();
  std::string buffer(size, ' ');
  file.seekg(0);
  file.read(&buffer[0], size);

  resp.set_content(buffer, "text/plain");
}

bool HttpServer::Init(int port) {
  port_ = port;
  server_ = std::make_unique<Server>();
  if (!server_ || !server_->is_valid()) {
    LOG::Warn(TAG, "Can Not Create HttpServer");
    return false;
  }
  server_->Post("/config", [this](const Request &req, Response &res) {
    this->SetSmartHubConfig(req, res);
  });
  server_->Get("/status", [this](const Request &req, Response &res) {
    this->GetSmartHubStatus(req, res);
  });
  server_->Get("/log", [this](const Request &req, Response &res) {
    this->GetSmartHubLog(req, res);
  });

  server_->set_error_handler([](const Request & /*req*/, Response &res) {
    const char *fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
    char buf[BUFSIZ];
    snprintf(buf, sizeof(buf), fmt, res.status);
    res.set_content(buf, "text/html");
  });

  // server_->set_logger([](const Request &req, const Response &res) {
  //   LOG::Debug(TAG, "{}", log(req, res));
  // });

  LOG::Warn(TAG, "Initialized");

  return true;
}

bool HttpServer::Start() {
  int started = false;
  ;
  std::thread([this, &started] {
    started = server_->listen("localhost", port_);
  }).detach();

  if (started) {
    LOG::Debug(TAG, "HttpServer Started");
  } else {
    LOG::Warn(TAG, "Can not start HttpServer");
  }
  return started;
}

void HttpServer::SetHubManager(HubManager *manager) { hub_manager_ = manager; }
}  // namespace SmartHub

static std::string dump_headers(const Headers &headers) {
  std::string s;
  char buf[BUFSIZ];

  for (auto it = headers.begin(); it != headers.end(); ++it) {
    const auto &x = *it;
    snprintf(buf, sizeof(buf), "%s: %s\n", x.first.c_str(), x.second.c_str());
    s += buf;
  }

  return s;
}

static std::string log(const Request &req, const Response &res) {
  std::string s;
  char buf[BUFSIZ];

  s += "================================\n";

  snprintf(buf, sizeof(buf), "%s %s %s", req.method.c_str(),
           req.version.c_str(), req.path.c_str());
  s += buf;

  std::string query;
  for (auto it = req.params.begin(); it != req.params.end(); ++it) {
    const auto &x = *it;
    snprintf(buf, sizeof(buf), "%c%s=%s",
             (it == req.params.begin()) ? '?' : '&', x.first.c_str(),
             x.second.c_str());
    query += buf;
  }
  snprintf(buf, sizeof(buf), "%s\n", query.c_str());
  s += buf;

  s += dump_headers(req.headers);

  s += "--------------------------------\n";

  snprintf(buf, sizeof(buf), "%d %s\n", res.status, res.version.c_str());
  s += buf;
  s += dump_headers(res.headers);
  s += "\n";

  if (!res.body.empty()) {
    s += res.body;
  }

  s += "\n";

  return s;
}