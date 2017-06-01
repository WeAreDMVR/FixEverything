#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "asio.hpp"
#include "cereal/archives/json.hpp"
#include "cocos2d.h"

#include "Connection.h"
#include "GameAction.h"
#include "Level.h"

#include <cstdlib>
#include <string>

using asio::ip::tcp;

class Client {
 public:
  Client() : conn_(new Connection(io_service_)) {}

  Client(connection_ptr conn, const std::string& host) : Client() {
    connect(host);
  }

  bool is_open() const;
  asio::error_code connect(const std::string& host);
  bool available();

  template <typename T>
  void write(const T& t) {
    conn_->write(t);
  }

  template <typename T>
  void read(T& t) {
    conn_->read(t);
  }

 private:
  asio::io_service io_service_;
  connection_ptr conn_;
};

#endif  // _CLIENT_H_
