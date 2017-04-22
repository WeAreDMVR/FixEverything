#include "../cocos2d/cocos/cocos2d.h"
#include "World.h"

class pSprite : public Node {
	friend Sprite;
public:
	pSprite();
	pSprite(Sprite* sprite);

	inline Sprite* getSprite();
	inline void setSprite(Sprite* sprite);
	inline void setPosition(Point p);
	inline int getPositionX();
	inline int getPositionY();
	inline void setProperties(ValueMap& properties);
	inline ValueMap& getProperties();
	void addBodyToWorld(b2World* world);
	void pSprite::addBodyToWorldAtPosition(b2World * world, Point p);
    void createRectangularFixture(TMXLayer* layer, const Size tileSize, int x, int y);
    void createRectangularFixture();
    // NOTE: Does NOT remove sprite from Level
    // Level must do that
    void removeBodyFromWorld(b2World *world);

private:
	typedef Node super;
	typedef Vec2 Point;

protected:
	b2Body*   _body;
	Sprite*   _sprite;
	ValueMap& _properties;
	int       _xposition;
	int       _yposition;
}

enum class FilterCategory {
	SolidTrack1 	= 0x01,
	NonSolidTrack1 	= 0x02,
	SolidTrack2 	= 0x03,
	NonSolidTrack2	= 0x04,
	SolidTrack3 	= 0x05,
	NonSolidTrack3 	= 0x06
}