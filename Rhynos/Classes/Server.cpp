#include "Server.h"

#include "ServerLevel.h"

#include "asio.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/unordered_set.hpp"
#include "cocos2d.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <unordered_set>
#include <utility>

using namespace asio;
using namespace cereal;
using namespace cocos2d;

using std::error_code;
using std::make_shared;
using std::move;
using std::shared_ptr;
using std::unordered_set;

void Session::do_read() {
  while (true) {
    unordered_set<EventKeyboard::KeyCode> keys_pressed;
    PortableBinaryInputArchive iarchive(*iostream_);
    iarchive(keys_pressed);
    for (const EventKeyboard::KeyCode code : keys_pressed) {
      switch (code) {
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
          CCLOG("right");
          break;
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
          CCLOG("left");
          break;
        case EventKeyboard::KeyCode::KEY_SPACE:
          CCLOG("up");
          break;
        case EventKeyboard::KeyCode::KEY_1:
          CCLOG("1");
          break;
        case EventKeyboard::KeyCode::KEY_2:
          CCLOG("2");
          break;
        case EventKeyboard::KeyCode::KEY_3:
          CCLOG("3");
          break;
        default:
          break;
      }
    }

    do_write();
  }
}

void Session::do_write() {}

void Server::do_accept() {
  shared_ptr<tcp::iostream> iostream = make_shared<tcp::iostream>();
  connections_.push_back(iostream);
  acceptor_.async_accept(*iostream->rdbuf(), [this, iostream](error_code ec) {
    if (!ec) {
      make_shared<Session>(iostream, level_)->start();
    }

    do_accept();
  });
}
