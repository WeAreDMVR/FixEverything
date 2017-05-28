#include "Server.h"

#include "GameAction.h"

#include "asio.hpp"
#include "cereal/archives/portable_binary.hpp"
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
using std::make_pair;
using std::make_shared;
using std::move;
using std::shared_ptr;
using std::unordered_set;

static int next_session_id = 1;

void Session::start() {
  PortableBinaryOutputArchive oarchive(*iostream_);
  oarchive(GameAction::connectionEstablishedAction(id_));
  do_read();
}

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
  int session_id = next_session_id++;
  connections_.push_back(make_pair(iostream, session_id));
  acceptor_.async_accept(
      *iostream->rdbuf(), [this, iostream, session_id](error_code ec) {
        if (!ec) {
          make_shared<Session>(iostream, session_id)->start();
        }

        if (connections_.size() < 2) {
          do_accept();
        } else {
          int player1_id = connections_[0].second;
          int player2_id = connections_[1].second;
          for (auto& connection : connections_) {
            PortableBinaryOutputArchive oarchive(*connection.first);
            oarchive(GameAction::gameStartAction(player1_id, player2_id));
          }
          do_game_loop();
        }
      });
}

void Server::do_game_loop() {}

bool ServerScene::init() {
  // 1. super init first
  if (!Scene::init()) {
    return false;
  }
  const auto& visibleSize = Director::getInstance()->getVisibleSize();
  const Vec2& origin = Director::getInstance()->getVisibleOrigin();

  // create and initialize a label
  auto label =
      Label::createWithTTF("Server is running", "fonts/Marker Felt.ttf", 24);

  // position the label on the center of the screen
  label->setPosition(
      Vec2(origin.x + visibleSize.width / 2,
           origin.y + visibleSize.height - label->getContentSize().height));

  // add the label as a child to this layer
  this->addChild(label, 1);

  scheduleUpdate();

  return true;
}

void ServerScene::update(float dt) { io_service_.run_one(); }
