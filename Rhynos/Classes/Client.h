#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "asio.hpp"
#include "cocos2d.h"

#include <cstdlib>
#include <string>

using asio::ip::tcp;

class Client {
 public:
  Client() : io_service_(), socket_(io_service_), resolver_(io_service_) {}

  Client(const std::string &host) : Client() { connect(host); }

  void write(char *request);
  size_t read(char *reply);

  bool is_open() const;
  void connect(const std::string &host);

 private:
  asio::io_service io_service_;
  tcp::socket socket_;
  tcp::resolver resolver_;
};

class ClientScene : public cocos2d::Scene {
 public:
  virtual bool init() override;

  CREATE_FUNC(ClientScene);

  void update(float delta);

 private:
  ClientScene() {}

  Client client_;
};

#endif  // _CLIENT_H_
