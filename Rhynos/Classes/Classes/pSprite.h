#include "../cocos2d/cocos/cocos2d.h"
#include "World.h"

class pSprite : public Node {
	friend Sprite;
public:
	pSprite();
	pSprite(Sprite* sprite);

	inline Sprite* getSprite();
	inline pSprite::setPosition(Point p);
	inline pSprite::getPositionX();
	inline pSprite::getPositionY();
	inline pSprite::setProperties(Value properties);
	inline pSprite::getProperties();
	void pSprite::addBodyToWorld(b2World* world, b2BodyType bodyType);
    void pSprite::createRectangularFixture(TMXLayer* layer, const Size tileSize, int x, int y);

private:
	typedef Node super;
	typedef Vec2 Point;

protected:
	b2Body*  _body;
	Sprite*  _sprite;
	Value    _properties;
	int      _xposition;
	int      _yposition;
}

enum class FilterCategory {
	SolidTrack1 	= 0x01,
	NonSolidTrack1 	= 0x02,
	SolidTrack2 	= 0x03,
	NonSolidTrack2	= 0x04,
	SolidTrack3 	= 0x05,
	NonSolidTrack3 	= 0x06
}