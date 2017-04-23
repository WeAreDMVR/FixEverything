#include "Player.h"

class Level: public cocos2d::Scene {
public:
	Level();
	static Level *createWithMap(const std::string& tmxFile);
	void loadLayers();
	void loadObjects();
	
    cocos2d::Point positionForTileCoord(cocos2d::Point coordinate);
    cocos2d::Point tileCoordForPosition(cocos2d::Point position);
private:
	typedef Scene super;
	typedef std::vector<pSprite> pSpriteVector;
	
    void createFixtures(cocos2d::TMXLayer* layer);
    pSprite* addObject(const std::string className, cocos2d::ValueMap& properties);

protected:
	b2World*               _world;
    cocos2d::TMXTiledMap*  _map;
	pSpriteVector          _sprites;
};
