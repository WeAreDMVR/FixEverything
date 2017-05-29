#ifndef _SERVER_H_
#define _SERVER_H_

#include "asio.hpp"
#include "cocos2d.h"

#include "NetworkingConstants.h"

using asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
 public:
  Session(tcp::socket socket) : socket_(std::move(socket)) {}

  void start() { do_read(); }

 private:
  void do_read();

  void do_write(size_t length);

  tcp::socket socket_;
  char data_[NetworkingConstants::network_buffer_length];
};

class Server {
 public:
  Server(asio::io_service& io_service, short port)
      : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
        socket_(io_service) {
    do_accept();
  }

 private:
  void do_accept();

  tcp::acceptor acceptor_;
  tcp::socket socket_;
};

class ServerScene : public cocos2d::Scene {
 public:
  virtual bool init() override;

  CREATE_FUNC(ServerScene);

  void update(float delta) override;

 private:
  ServerScene();

  asio::io_service io_service_;
  Server server_;
};

#endif  // _SERVER_H_
