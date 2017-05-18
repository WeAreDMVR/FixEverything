#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "asio.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/vector.hpp"
#include "cocos2d.h"

#include <cstdlib>
#include <string>

using asio::ip::tcp;

class Client {
 public:
  Client() {}

  Client(const std::string &host) { connect(host); }

  bool is_open() const;
  bool connect(const std::string &host);

  template <typename T>
  void write(const T &request) {
    cereal::PortableBinaryOutputArchive oarchive(iostream_);
    oarchive(request);
  }

  template <typename T>
  void read(T *reply) {
    cereal::PortableBinaryInputArchive iarchive(iostream_);
    iarchive(*reply);
  }

 private:
  tcp::iostream iostream_;
};

class ClientScene : public cocos2d::Scene {
 public:
  virtual bool init() override;

  CREATE_FUNC(ClientScene);

 private:
  ClientScene() {}
};

#endif  // _CLIENT_H_
