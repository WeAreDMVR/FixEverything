#ifndef _SERVER_H_
#define _SERVER_H_

#include "asio.hpp"
#include "cocos2d.h"

#include "Level.h"
#include "NetworkingConstants.h"

#include <memory>
#include <vector>

using asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
 public:
  Session(std::shared_ptr<tcp::iostream> iostream, Level* level)
      : iostream_(iostream), level_(level) {}

  void start() { do_read(); }

 private:
  void do_read();

  void do_write();

  std::shared_ptr<tcp::iostream> iostream_;
  Level* level_;
};

class Server {
 public:
  Server(asio::io_service& io_service, short port, Level* level)
      : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)), level_(level) {
    do_accept();
  }

 private:
  void do_accept();

  Level* level_;
  tcp::acceptor acceptor_;
  std::vector<std::shared_ptr<tcp::iostream>> connections_;
};

#endif  // _SERVER_H_
