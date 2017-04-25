#include "Rhyno.h"

#include "World.h"

using cocos2d::Color4B;
using cocos2d::Director;
using cocos2d::Event;
using cocos2d::EventKeyboard;
using cocos2d::EventListenerKeyboard;
using cocos2d::Label;
using cocos2d::Layer;
using cocos2d::LayerColor;
using cocos2d::Menu;
using cocos2d::MenuItemImage;
using cocos2d::Scene;
using cocos2d::Sprite;
using cocos2d::Vec2;

Scene* Rhyno::createScene() {
  Scene* scene = Scene::create();
  Rhyno* rhynoGame = Rhyno::create();

  scene->addChild(rhynoGame);

  return scene;
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

  LayerColor* background = LayerColor::create(Color4B(0xCF, 0x53, 0x00, 0xFF));
  this->addChild(background, 0);

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

  _sprite = Sprite::create("images/blob_rimuru.png");
  _sprite->setPosition(this->getContentSize().width / 2,
                       this->getContentSize().height / 2);

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
    _sprite->setPosition(loc.x - 1, loc.y);
  } else if (isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW) ||
             isKeyPressed(EventKeyboard::KeyCode::KEY_D)) {
    _sprite->setPosition(loc.x + 1, loc.y);
  } else if (isKeyPressed(EventKeyboard::KeyCode::KEY_UP_ARROW) ||
             isKeyPressed(EventKeyboard::KeyCode::KEY_W)) {
    _sprite->setPosition(loc.x, loc.y + 1);
  } else if (isKeyPressed(EventKeyboard::KeyCode::KEY_DOWN_ARROW) ||
             isKeyPressed(EventKeyboard::KeyCode::KEY_S)) {
    _sprite->setPosition(loc.x, loc.y - 1);
  }
}

void Rhyno::menuCloseCallback(Ref* pSender) {
  // Close the cocos2d-x game scene and quit the application
  Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  exit(0);
#endif
}