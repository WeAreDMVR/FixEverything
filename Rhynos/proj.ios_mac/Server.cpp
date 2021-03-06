#include "Server.h"

#include "asio.hpp"
#include "cocos2d.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

using namespace asio;
using namespace cocos2d;

using std::error_code;
using std::make_shared;
using std::move;

void Session::do_read() {
  auto self(shared_from_this());
  socket_.async_read_some(
      buffer(data_, NetworkingConstants::network_buffer_length),
      [this, self](error_code ec, size_t length) {
        if (!ec) {
          CCLOG(data_);
          do_read();
        }
      });
}

void Session::do_write(size_t length) {
  auto self(shared_from_this());
  async_write(socket_,
              buffer(data_, NetworkingConstants::network_buffer_length),
              [this, self](error_code ec, size_t /*length*/) {
                if (!ec) {
                  do_read();
                }
              });
}

void Server::do_accept() {
  acceptor_.async_accept(socket_, [this](error_code ec) {
    if (!ec) {
      make_shared<Session>(move(socket_))->start();
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
