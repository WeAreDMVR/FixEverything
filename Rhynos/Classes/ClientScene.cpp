#include "ClientScene.h"

#include "ClientLevel.h"

#include "cocos2d.h"

using namespace cocos2d;

using ui::TextField;
using std::async;
using std::future_status;
using std::launch;

bool ClientScene::init() {
  // 1. super init first
  if (!Scene::init()) {
    return false;
  }
  const auto& visibleSize = Director::getInstance()->getVisibleSize();
  const Vec2& origin = Director::getInstance()->getVisibleOrigin();

  // create and initialize a label
  host_field_ =
      TextField::create("Enter the hostname or ip address of the server",
                        "fonts/Marker Felt.ttf", 30);

  // position the label on the center of the screen
  host_field_->setPosition(Vec2(
      origin.x + visibleSize.width / 2,
      origin.y + visibleSize.height - host_field_->getContentSize().height));

  auto keyListener = EventListenerKeyboard::create();
  keyListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode,
                                  Event* event) {
    switch (keyCode) {
      case EventKeyboard::KeyCode::KEY_ENTER: {
        if (!client_level_future_.valid()) {
          auto spritecache = cocos2d::SpriteFrameCache::getInstance();
          spritecache->addSpriteFramesWithFile("images/textures.plist");
          auto client_level =
              ClientLevel::createWithMap("images/track-2-noai.tmx");
          client_level_future_ = async(launch::async, ClientLevel::init,
                                       client_level, host_field_->getString());

          this->removeChild(host_field_);
          auto label = Label::createWithTTF("Waiting for another player",
                                            "fonts/Marker Felt.ttf", 24);
          label->setPosition(Vec2(
              origin.x + visibleSize.width / 2,
              origin.y + visibleSize.height - label->getContentSize().height));
          this->addChild(label, 1);
        }
        break;
      }
      default:
        break;
    }
  };

  this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener,
                                                                 this);

  this->scheduleUpdate();

  // add the label as a child to this layer
  this->addChild(host_field_, 1);

  return true;
}

void ClientScene::update(float delta) {
  if (client_level_future_.valid()) {
    future_status status =
        client_level_future_.wait_for(std::chrono::milliseconds(1));
    if (status != future_status::timeout) {
      Level* level1 = client_level_future_.get();
      level1->loadLayers();
      level1->loadObjects();

      audioSource_->playBackgroundMusic("audio/level_theme.mp3", true);
      Director::getInstance()->replaceScene(level1);
    }
  }
}
