#include <typeinfo>
#include "Level.h"
#include "Rhyno.h"
#include "World.h"

#include <Box2D/Box2D.h>

#define COCOS2D_DEBUG 1

using cocos2d::Point;
using cocos2d::Color4B;
using cocos2d::Director;
using cocos2d::Event;
using cocos2d::EventKeyboard;
using cocos2d::EventListenerKeyboard;
using cocos2d::Label;
using cocos2d::Menu;
using cocos2d::MenuItemImage;
using cocos2d::Scene;
using cocos2d::Sprite;
using cocos2d::Vec2;
using cocos2d::TMXTiledMap;
using cocos2d::TMXLayer;
using cocos2d::TMXObjectGroup;

Scene* Rhyno::createScene() {
    return Rhyno::create();
}

// on "init" you need to initialize your instance
bool Rhyno::init() {
  //////////////////////////////
  // 1. super init first
  if (!Scene::init()) {
    return false;
  }
  const auto& visibleSize = Director::getInstance()->getVisibleSize();
  const Vec2& origin = Director::getInstance()->getVisibleOrigin();

  // Set up the keyboard listener
  auto keyListener = cocos2d::EventListenerKeyboard::create();
  keyListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event){
    switch(keyCode){
      case EventKeyboard::KeyCode::KEY_ENTER:
        // Check to switch to the level
        auto spritecache = cocos2d::SpriteFrameCache::getInstance();
        spritecache->addSpriteFramesWithFile("images/textures.plist");
        Level* level1 = Level::createWithMap("images/long.tmx");
        level1->loadLayers();
        level1->loadObjects();

        Director::getInstance()->pushScene(level1);
      }
  };

  this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener,this);

  /////////////////////////////
  // 2. Some old stuff

  // add a label shows "Rhyno"
  // create and initialize a label

  auto label = Label::createWithTTF("Press Enter to Play", "fonts/Marker Felt.ttf", 24);

  // position the label on the center of the screen
  label->setPosition(
      Vec2(origin.x + visibleSize.width / 2,
           origin.y + visibleSize.height - label->getContentSize().height));

  // add the label as a child to this layer
  this->addChild(label, 1);

  // add "Rhyno" splash screen"
  // auto sprite = Sprite::create("HelloWorld.png");

  // position the sprite on the center of the screen
  // sprite->setPosition(Vec2(visibleSize.width/2 + origin.x,
  // visibleSize.height/2 + origin.y));

  // add the sprite as a child to this layer
  // this->addChild(sprite, 0);

  //////////////////////////////
  // 3. New Code
  this->scheduleUpdate();

  return true;
}

void Rhyno::update(float delta) {

}

void Rhyno::menuCloseCallback(Ref* pSender) {
  // Close the cocos2d-x game scene and quit the application
  Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  exit(0);
#endif
}
