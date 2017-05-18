#include "ServerLevel.h"

#include "Level.h"

#include "cocos2d.h"

#include <string>

using namespace cocos2d;

using std::string;

ServerLevel* ServerLevel::createWithMap(const string& tmxFile) {
  auto ret = new ServerLevel(tmxFile);
  ret->init();
  ret->autorelease();
  return ret;
}

bool ServerLevel::init() {
  // 1. super init first
  if (!Level::init()) {
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

  return true;
}

void ServerLevel::update(float dt) { io_service_.run_one(); }

void ServerLevel::handleInput() {}
