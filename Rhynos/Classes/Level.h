#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "Player.h"
#include "KeyboardPoller.h"
#include "AI.hpp"
#include "cocos2d.h"

#include <chrono>
#include <string>
#include <vector>
#include <cstdlib>



class Level : public cocos2d::Scene {
 public:
  Level();

  static Level* createWithMap(const std::string& tmxFile);
  void loadLayers();
  void loadObjects();
  void update(float dt) override;

  cocos2d::Point positionForTileCoord(const cocos2d::Point& coordinate);
  cocos2d::Point tileCoordForPosition(const cocos2d::Point& position);
    
 private:
  void createFixtures(cocos2d::TMXLayer* layer);
  pSprite* addObject(const std::string& className,
                     const cocos2d::ValueMap& properties);
  double getCurrentTime();
  void handleInput();
    
    


 protected:
  b2World* _world;
  const cocos2d::TMXTiledMap* _map;
  std::vector<pSprite*> _sprites;
  std::unordered_map<std::string, Player*> _players;
  double _lastTime;
  KeyboardPoller* keyPoll;
};

#endif  // _LEVEL_H_
