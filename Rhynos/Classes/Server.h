#ifndef _SERVER_H_
#define _SERVER_H_

#include "asio.hpp"
#include "cocos2d.h"

#include "NetworkingConstants.h"

#include <memory>
#include <utility>
#include <vector>

using asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
 public:
  Session(std::shared_ptr<tcp::iostream> iostream, const int id)
      : iostream_(iostream), id_(id) {}

  void start();

 private:
  void do_read();

  void do_write();

  std::shared_ptr<tcp::iostream> iostream_;
  const int id_;
};

class Server {
 public:
  Server(asio::io_service& io_service, short port)
      : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
    do_accept();
  }

 private:
  void do_accept();
  void do_game_loop();

  tcp::acceptor acceptor_;
  std::vector<std::pair<std::shared_ptr<tcp::iostream>, int>> connections_;
};

class ServerScene : public cocos2d::Scene {
 public:
  virtual bool init() override;

  void update(float dt) override;

  CREATE_FUNC(ServerScene);

 private:
  ServerScene()
      : io_service_(), server_(io_service_, NetworkingConstants::PORT) {}

  asio::io_service io_service_;
  Server server_;
};

#endif  // _SERVER_H_
