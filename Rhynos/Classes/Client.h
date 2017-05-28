#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "asio.hpp"
#include "cocos2d.h"

#include "GameAction.h"

#include <cstdlib>
#include <string>
#include <unordered_set>

using asio::ip::tcp;

class Client {
 public:
  Client() {}

  Client(const std::string &host) { connect(host); }

  bool is_open() const;
  bool connect(const std::string &host);

  void write(
      const std::unordered_set<cocos2d::EventKeyboard::KeyCode> keys_pressed);

  void read(GameAction *game_action);

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
