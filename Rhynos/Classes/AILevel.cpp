#include "AILevel.h"

#include <Box2D/Box2D.h>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

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

bool AILevel::didWin(float x, float y) {
  // Currently only doing for AI and player

  bool playerWin = this->_players["localhost"]->checkWin(Point(2700, 500));
  bool AIWin = this->_players["ai"]->checkWin(Point(2700, 500));

  const char* msg;
  if (playerWin && !AIWin) {
    msg = "Player 1 wins! \n Press Enter to go to the main menu.";
    this->over = true;
  } else if (AIWin && !playerWin) {
    msg = "The AI wins! \n Press Enter to go to the main menu.";
    this->over = true;
  } else {
    return false;
  }

  if (this->over) {
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("audio/fanfare.wav");
    auto label = Label::createWithTTF(msg, "fonts/Marker Felt.ttf", 48);
    // position the label on the center of the screen
    label->setPosition(Vec2(x, y));

    // Keeep the message in the middle of the screen
    label->setName("winning");
    this->removeChildByName("winning", true);
    this->addChild(label, 1, "winning");
  }
  return true;
}
