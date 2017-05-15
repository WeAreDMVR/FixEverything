#include "Client.h"

#include <cocos2d.h>
#include <ui/UITextField.h>
#include <asio.hpp>

#include "Level.h"
#include "NetworkingConstants.h"

#include <cstring>
#include <string>

using namespace asio;
using namespace cocos2d;

using cocos2d::ui::TextField;
using std::string;
using std::to_string;

bool Client::is_open() const { return iostream_.rdbuf()->is_open(); }

bool Client::connect(const string& host) {
  iostream_.connect(host, to_string(NetworkingConstants::PORT));
  return static_cast<bool>(iostream_);
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
        auto spritecache = cocos2d::SpriteFrameCache::getInstance();
        spritecache->addSpriteFramesWithFile("images/textures.plist");
        Level* level1 = Level::createNetworkedWithMap("images/track-2.tmx",
                                                      host_field->getString());
        level1->loadLayers();
        level1->loadObjects();

        Director::getInstance()->pushScene(level1);
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
