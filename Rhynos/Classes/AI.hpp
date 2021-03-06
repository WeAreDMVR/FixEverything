#ifndef AI_hpp
#define AI_hpp

#include <stdio.h>
#include "Player.h"

using namespace cocos2d;

class AI : public Player {
 public:
  cocos2d::Point target;
  float failProb;
  AI(Sprite* player);

  int atTarget();
  int moveOther(Player* play);
  int move();
  void applyJumpSound();
  void analyzeMap(const TMXTiledMap* map);
  Point prev;
};
#endif /* AI_hpp */
