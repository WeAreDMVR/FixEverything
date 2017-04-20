#include "../cocos2d/cocos/cocos2d.h"
#include "World.h"
#include "pSprite.h"

class Level: public Scene {
public:
	Level();
	static Level *createWithMap(const std::string& tmxFile);
	void loadLayers();
	void loadObjects();
	
	Point positionForTileCoord(Point coordinate);
	Point tileCoordForPosition(Point position);
private:
	typedef Scene super;
	typedef std::vector<pSprite> pSpriteVector;
	
	void createFixtures(TMXLayer* layer);
	pSprite* addObject(const std::string className, ValueMap& properties);

protected:
	b2World* 	  _world;
	TMXTiledMap*  _map;
	pSpriteVector _sprites;
}