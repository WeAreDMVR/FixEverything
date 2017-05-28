#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "AI.hpp"
#include "Client.h"
#include "KeyboardPoller.h"
#include "Player.h"

#include "cocos2d.h"

#include <chrono>
#include <cstdlib>
#include <string>
#include <vector>

class Level : public cocos2d::Scene {
 public:
  void loadLayers();
  void loadObjects();
  virtual void update(float dt) override;

  cocos2d::Point positionForTileCoord(const cocos2d::Point& coordinate);
  cocos2d::Point tileCoordForPosition(const cocos2d::Point& position);

 private:
  void createFixtures(cocos2d::TMXLayer* layer);
  pSprite* addObject(const std::string& className,
                     const cocos2d::ValueMap& properties);
  double getCurrentTime();

 protected:
  Level(const std::string& tmxFile);

  virtual void handleInput() = 0;
  virtual void addPlayer(const std::string& className, Player* player) = 0;

  Player* _localPlayer;
  b2World* _world;
  const cocos2d::TMXTiledMap* _map;
  std::vector<pSprite*> _sprites;
  std::unordered_map<std::string, Player*> _players;
  double _lastTime;
  KeyboardPoller* keyPoll;
};

#endif  // _LEVEL_H_
