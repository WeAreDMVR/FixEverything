#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "pSprite.h"

#include "cocos2d.h"

#include <chrono>
#include <string>
#include <vector>

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

 protected:
  b2World* _world;
  const cocos2d::TMXTiledMap* _map;
  std::vector<pSprite*> _sprites;
  double _lastTime;
};

#endif  // _LEVEL_H_
