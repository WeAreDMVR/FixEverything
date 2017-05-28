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
    this->_players["localhost"]->applyInput(
        EventKeyboard::KeyCode::KEY_RIGHT_ARROW);
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
    CCLOG("left");
    this->_players["localhost"]->applyInput(
        EventKeyboard::KeyCode::KEY_LEFT_ARROW);
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_SPACE)) {
    CCLOG("up");
    this->_players["localhost"]->applyInput(EventKeyboard::KeyCode::KEY_SPACE);
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_1)) {
    CCLOG("1");
    this->_players["localhost"]->applyInput(EventKeyboard::KeyCode::KEY_1);
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_2)) {
    CCLOG("2");
    this->_players["localhost"]->applyInput(EventKeyboard::KeyCode::KEY_2);
  }
  if (this->keyPoll->isKeyPressed(EventKeyboard::KeyCode::KEY_3)) {
    CCLOG("3");
    this->_players["localhost"]->applyInput(EventKeyboard::KeyCode::KEY_3);
  }
}

void AILevel::addPlayer(const std::string& className, Player* player) {
  if (className == "Player") {
    this->_players["localhost"] = player;
    this->_localPlayer = player;
  } else if (className == "AI") {
    this->_players["ai"] = player;
  }
}
