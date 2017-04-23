#include "cocos2d.h"
#include "World.h"

class pSprite : public cocos2d::Node {
    friend cocos2d::Sprite;
public:
	pSprite();
    pSprite(cocos2d::Sprite* sprite);
    pSprite(const pSprite& obj);

    inline cocos2d::Sprite* getSprite();
    inline void setSprite(cocos2d::Sprite* sprite);
    inline void setPosition(cocos2d::Point p);
	inline int getPositionX();
	inline int getPositionY();
    inline void setProperties(cocos2d::ValueMap* properties);
    inline cocos2d::ValueMap* getProperties();
	void addBodyToWorld(b2World* world);
    void addBodyToWorldAtPosition(b2World * world, cocos2d::Point p);
    void createRectangularFixture(cocos2d::TMXLayer* layer, const cocos2d::Size tileSize, int x, int y);
    void createRectangularFixture();
    // NOTE: Does NOT remove sprite from Level
    // Level must do that
    void removeBodyFromWorld(b2World *world);

private:
	typedef Node super;

protected:
	b2Body*            _body;
    cocos2d::Sprite*   _sprite;
    cocos2d::ValueMap* _properties;
	int                _xposition;
	int                _yposition;
};

enum class FilterCategory {
	SolidTrack1 	= 0x01,
	NonSolidTrack1 	= 0x02,
	SolidTrack2 	= 0x03,
	NonSolidTrack2	= 0x04,
	SolidTrack3 	= 0x05,
	NonSolidTrack3 	= 0x06
};
