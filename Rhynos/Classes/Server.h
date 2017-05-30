#ifndef _SERVER_H_
#define _SERVER_H_

#include "asio.hpp"
#include "cocos2d.h"

#include "Connection.h"
#include "GameAction.h"
#include "NetworkingConstants.h"

#include <memory>
#include <mutex>
#include <utility>
#include <vector>

using asio::ip::tcp;

struct Server;

void session(connection_ptr conn, Server* server);

struct Server {
  Server(asio::io_service& io_service, short port)
      : acceptor(io_service, tcp::endpoint(tcp::v4(), port)) {}

  void start();
  void do_accept();
  void do_game_loop();

  tcp::acceptor acceptor;
  std::vector<connection_ptr> connections;
  std::vector<GameAction> game_actions;
  std::mutex mut;
  bool ready;
};

class ServerScene : public cocos2d::Scene {
 public:
  virtual bool init() override;

  CREATE_FUNC(ServerScene);

 private:
  ServerScene()
      : io_service_(), server_(io_service_, NetworkingConstants::PORT) {}

  asio::io_service io_service_;
  Server server_;
};

#endif  // _SERVER_H_
