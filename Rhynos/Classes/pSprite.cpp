#include "pSprite.h"

using namespace cocos2d;

pSprite::pSprite() {
    _sprite = Sprite::create();
}

pSprite::pSprite(Sprite* sprite) {
	_sprite = sprite;
}

// Copy constructor for push_back
pSprite::pSprite(const pSprite& obj) {
    // TODO
}


inline Sprite* pSprite::getSprite() {
	return this->_sprite;
}

inline void pSprite::setSprite(Sprite* sprite) {
	this->_sprite = sprite;
}

void pSprite::createRectangularFixture(TMXLayer* layer, const Size tileSize, int x, int y) {
	// get position and size
	auto position = layer->getPositionAt(Point(x,y));
	// auto tileSize = this->_map->getTileSize();

	// create shape
	b2PolygonShape shape;
	shape.SetAsBox(
		(tileSize.width / PixelsPerMeter),
		(tileSize.height / PixelsPerMeter));

	// create fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = Density;
	fixtureDef.friction = Friction;
	fixtureDef.restitution = Restitution;
	// NOTE: tile sprite must have a CategoryBits property
	//fixtureDef.filter.categoryBits = this->getProperties()->valueForKey("CategoryBits");
	fixtureDef.filter.categoryBits = this->getProperties()->at("CategoryBits").asByte();
	fixtureDef.filter.maskBits = 0xffff;
	
	this->_body->CreateFixture(&fixtureDef);
}

void pSprite::createRectangularFixture() {
	// get position and size
	int width = this->getProperties()->at("width").asInt();
	int height = this->getProperties()->at("height").asInt();
	// create shape
	b2PolygonShape shape;
	shape.SetAsBox(
		width / PixelsPerMeter,
		height / PixelsPerMeter);
	// create fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = Density;
	fixtureDef.friction = Friction;
	fixtureDef.restitution = Restitution;
	fixtureDef.filter.categoryBits = this->getProperties()->at("CategoryBits").asByte();
	fixtureDef.filter.maskBits = 0xffff;

	this->_body->CreateFixture(&fixtureDef);
}

void pSprite::addBodyToWorld(b2World* world) {
	b2BodyDef bodyDef;
	// NOTE: tile sprite must have a BodyType property
	//bodyDef.type = this->getProperties()->valueForKey("BodyType");
	bodyDef.type = (b2BodyType) this->getProperties()->at("BodyType").asUnsignedInt();
	bodyDef.position.Set(
		this->getPositionX() / PixelsPerMeter,
		this->getPositionY() / PixelsPerMeter);
	bodyDef.userData = this;
	bodyDef.fixedRotation = true;

	this->_body = world->CreateBody(&bodyDef); 
}

void pSprite::addBodyToWorldAtPosition(b2World * world, Point p) {
	b2BodyDef bodyDef;
	// NOTE: tile sprite must have a BodyType property
	//bodyDef.type = this->getProperties()->valueForKey("BodyType");
	bodyDef.type = (b2BodyType) this->getProperties()->at("BodyType").asUnsignedInt();
	bodyDef.position.Set(
		p.x / PixelsPerMeter,
		p.y / PixelsPerMeter);
	bodyDef.userData = this;
	bodyDef.fixedRotation = true;

	this->_body = world->CreateBody(&bodyDef); 
}

void pSprite::removeBodyFromWorld(b2World *world) {
	// destory body
	world->DestroyBody(this->_body);
	this->_body = nullptr;
}
