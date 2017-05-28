#ifndef _SERVER_H_
#define _SERVER_H_

#include "asio.hpp"
#include "cocos2d.h"

#include "GameAction.h"
#include "NetworkingConstants.h"

#include <memory>
#include <mutex>
#include <utility>
#include <vector>

using asio::ip::tcp;

class Server;

class Session : public std::enable_shared_from_this<Session> {
 public:
  Session(std::shared_ptr<tcp::iostream> iostream, Server* server, const int id)
      : iostream_(iostream), server_(server), id_(id) {}

  void start();

 private:
  void do_read();

  std::shared_ptr<tcp::iostream> iostream_;
  Server* server_;
  const int id_;
};

class Server {
  friend class Session;

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
  std::vector<GameAction> game_actions_;
  std::mutex mut_;
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
