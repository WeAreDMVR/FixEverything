#include "Server.h"

#include "asio.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/vector.hpp"
#include "cocos2d.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

using namespace asio;
using namespace cereal;
using namespace cocos2d;

using std::error_code;
using std::make_shared;
using std::move;
using std::vector;
using std::shared_ptr;

void Session::do_read() {
  vector<EventKeyboard::KeyCode> keys_pressed;
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

  do_read();
}

void Session::do_write(size_t length) {}

void Server::do_accept() {
  shared_ptr<tcp::iostream> iostream = make_shared<tcp::iostream>();
  connections_.push_back(iostream);
  acceptor_.async_accept(*iostream->rdbuf(), [this, iostream](error_code ec) {
    if (!ec) {
      make_shared<Session>(iostream)->start();
    }

    do_accept();
  });
}

ServerScene::ServerScene()
    : io_service_(), server_(io_service_, NetworkingConstants::PORT) {}

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

  this->scheduleUpdate();

  return true;
}

void ServerScene::update(float delta) { io_service_.run_one(); }
