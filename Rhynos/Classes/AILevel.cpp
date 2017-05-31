#include "AILevel.h"

#include <Box2D/Box2D.h>
#include "cocos2d.h"

#include <string>

using namespace cocos2d;

using std::string;

AILevel* AILevel::createWithMap(const string& tmxFile) {
  return new AILevel(tmxFile);
}

void AILevel::addPlayer(const std::string& className, Player* player) {
  if (className == "Player") {
    this->_players["localhost"] = player;
    this->_localPlayer = player;
  } else if (className == "AI") {
    this->_players["ai"] = player;
  }
}
