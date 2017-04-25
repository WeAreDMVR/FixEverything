#include <typeinfo>
#include "Level.h"
#include "Rhyno.h"
#include "World.h"

#include <Box2D/Box2D.h>

#define COCOS2D_DEBUG 1

using cocos2d::Color4B;
using cocos2d::Director;
using cocos2d::Event;
using cocos2d::EventKeyboard;
using cocos2d::EventListenerKeyboard;
using cocos2d::Label;
using cocos2d::Menu;
using cocos2d::MenuItemImage;
using cocos2d::Sprite;
using cocos2d::Vec2;
using cocos2d::TMXTiledMap;
using cocos2d::TMXLayer;
using cocos2d::TMXObjectGroup;

Level* Rhyno::createScene() {
  Level* level = Level::createWithMap("images/spring_map.tmx");
  Rhyno* rhynoGame = Rhyno::create();

  level->addChild(rhynoGame);

  return level;
}

// on "init" you need to initialize your instance
bool Rhyno::init() {
  //////////////////////////////
  // 1. super init first
  if (!KeyboardPoller::init()) {
    return false;
  }

  const auto& visibleSize = Director::getInstance()->getVisibleSize();
  const Vec2& origin = Director::getInstance()->getVisibleOrigin();

  /////////////////////////////
  // 2. add a menu item with "X" image, which is clicked to quit the program
  //    you may modify it.

  // add a "close" icon to exit the progress. it's an autorelease object
  const auto& closeItem =
      MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
                            CC_CALLBACK_1(Rhyno::menuCloseCallback, this));

  closeItem->setPosition(
      Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
           origin.y + closeItem->getContentSize().height / 2));

  // create menu, it's an autorelease object
  Menu* menu = Menu::create(closeItem, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  /////////////////////////////
  // 3. add your codes below...

  // add a label shows "Rhyno"
  // create and initialize a label

  auto label = Label::createWithTTF("Rhyno", "fonts/Marker Felt.ttf", 24);

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
  // 4. New Code

  this->_currentTime = 0.0;
  this->_lastTime = 0.0;
  this->scheduleUpdate();

/*
  _sprite = Sprite::create("images/blob_rimuru.png");
  _sprite->setPosition(this->getContentSize().width / 2,
                       this->getContentSize().height / 2);
  _sprite->setAnchorPoint(Vec2(0, 0));
  _sprite->setPosition(Vec2(5, 60));
  */

  TMXTiledMap* tileMap = TMXTiledMap::create("images/spring_map.tmx");
  TMXObjectGroup* objectGroup = tileMap->objectGroupNamed("blob_layer");
  if(objectGroup == NULL){
    CCLOG("TileMap has no objects layer");
    return false;
  }
  auto& objects = objectGroup->getObjects(); //Currently just one object
  _sprite = Sprite::create("images/blob_rimuru.png");
  for (auto& obj : objects)
  {
    cocos2d::ValueMap& dict = obj.asValueMap();

    float x = dict["x"].asFloat()+50;
    float y = dict["y"].asFloat()+100;
    _sprite->setPosition(cocos2d::Point(x,y));
}
  //TMXLayer* background = tileMap->getLayer("background");
  //TMXLayer* ground = tileMap->getLayer("ground");
  this->addChild(tileMap, 0);
  this->addChild(_sprite, 0);
  
  return true;
}

void Rhyno::update(float delta) {
  _currentTime += delta;

  if (_currentTime >= _lastTime + TimeStep || _lastTime == 0.0) {
    _lastTime = _currentTime;
    // World::step(this->_world);
  }

  const Vec2& loc = _sprite->getPosition();
  if (isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW) ||
      isKeyPressed(EventKeyboard::KeyCode::KEY_A)) {
    _sprite->setPosition(loc.x - 100.0 * delta, loc.y);
  } else if (isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW) ||
             isKeyPressed(EventKeyboard::KeyCode::KEY_D)) {
    _sprite->setPosition(loc.x + 100.0 * delta, loc.y);
  } else if (isKeyPressed(EventKeyboard::KeyCode::KEY_UP_ARROW) ||
             isKeyPressed(EventKeyboard::KeyCode::KEY_W)) {
    //_sprite->setPosition(loc.x, loc.y + 100.0 * delta);
  } else if (isKeyPressed(EventKeyboard::KeyCode::KEY_DOWN_ARROW) ||
             isKeyPressed(EventKeyboard::KeyCode::KEY_S)) {
    //_sprite->setPosition(loc.x, loc.y - 100.0 * delta);
  }
}

void Rhyno::menuCloseCallback(Ref* pSender) {
  // Close the cocos2d-x game scene and quit the application
  Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  exit(0);
#endif
}
