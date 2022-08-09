#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>

#define SERVER_CERT_FILE "./cert.pem"
#define SERVER_PRIVATE_KEY_FILE "./key.pem"

class HttpServer
{

public:
    static void init();
};

void HttpServer::init()
{
    // HTTPS
    httplib::SSLServer svr(SERVER_CERT_FILE, SERVER_PRIVATE_KEY_FILE);

   svr.Post("/notify", [&](const auto& req, auto& res) {
  auto size = req.files.size();
  auto ret = req.has_file("name1");
  const auto& file = req.get_file_value("name1");
  // file.filename;
  // file.content_type;
  // file.content;
});

    bool listen=svr.listen("0.0.0.0", 8083);
    if(!listen)
    {
        printf("can notify listen\n");
    }
}
#endif // HTTP_SERVER_H