#include "Client.h"

#include <ui/UITextField.h>
#include "asio.hpp"
#include "cocos2d.h"

#include "NetworkingConstants.h"

#include <cstring>
#include <string>

using namespace asio;
using namespace cocos2d;

using std::string;
using cocos2d::ui::TextField;

void Client::write(char* request) {
  asio::write(socket_,
              buffer(request, NetworkingConstants::network_buffer_length));
}

size_t Client::read(char* reply) {
  return asio::read(socket_,
                    buffer(reply, NetworkingConstants::network_buffer_length));
}

bool Client::is_open() const { return socket_.is_open(); }

void Client::connect(const string& host) {
  asio::connect(
      socket_,
      resolver_.resolve({host, std::to_string(NetworkingConstants::PORT)}));
}

bool ClientScene::init() {
  // 1. super init first
  if (!Scene::init()) {
    return false;
  }
  const auto& visibleSize = Director::getInstance()->getVisibleSize();
  const Vec2& origin = Director::getInstance()->getVisibleOrigin();

  // create and initialize a label
  auto host_field =
      TextField::create("Enter the hostname or ip address of the server",
                        "fonts/Marker Felt.ttf", 30);

  // position the label on the center of the screen
  host_field->setPosition(Vec2(
      origin.x + visibleSize.width / 2,
      origin.y + visibleSize.height - host_field->getContentSize().height));

  auto keyListener = EventListenerKeyboard::create();
  keyListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode,
                                  Event* event) {
    switch (keyCode) {
      case EventKeyboard::KeyCode::KEY_ENTER: {
        if (client_.is_open()) {
          char buffer[NetworkingConstants::network_buffer_length];
          strcpy(buffer, host_field->getString().c_str());
          client_.write(buffer);
          client_.read(buffer);
          CCLOG(buffer);
        } else {
          client_.connect(host_field->getString());
        }

        break;
      }
      default:
        break;
    }
  };

  this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener,
                                                                 this);

  // add the label as a child to this layer
  this->addChild(host_field, 1);

  this->scheduleUpdate();

  return true;
}

void ClientScene::update(float delta) {}
