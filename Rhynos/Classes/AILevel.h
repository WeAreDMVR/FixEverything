#ifndef _AI_LEVEL_H_
#define _AI_LEVEL_H_

#include "Level.h"
#include "Player.h"

#include "cocos2d.h"

#include <string>

class AILevel : public Level {
 public:
  using Level::Level;

  static AILevel* createWithMap(const std::string& tmxFile);

 protected:
  void addPlayer(const std::string& className, Player* player) override;
  bool didWin(float x, float y) override;
};

#endif  // _AI_LEVEL_H_
