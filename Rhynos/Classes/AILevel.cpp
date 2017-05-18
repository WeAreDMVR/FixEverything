#include "AILevel.h"

#include <Box2D/Box2D.h>
#include "cocos2d.h"

#include <string>

using namespace cocos2d;

using std::string;

AILevel* AILevel::createWithMap(const string& tmxFile) {
  return new AILevel(tmxFile);
}

void AILevel::handleInput() {
  // Arrows
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
    CCLOG("right");
    this->_players["localhost"]->applyMoveRight();
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
    CCLOG("left");
    this->_players["localhost"]->applyMoveLeft();
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_SPACE)) {
    CCLOG("up");
    this->_players["localhost"]->applyJump();
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_1)) {
    CCLOG("1");
    this->_players["localhost"]->setLayer(1);
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_2)) {
    CCLOG("2");
    this->_players["localhost"]->setLayer(2);
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_3)) {
    CCLOG("3");
    this->_players["localhost"]->setLayer(3);
  }
}
