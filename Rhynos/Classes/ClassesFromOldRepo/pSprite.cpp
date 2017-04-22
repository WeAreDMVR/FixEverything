#include "pSprite.h"

pSprite::pSprite() {
	_sprite = Sprite::create();
}

pSprite::pSprite(Sprite* sprite) {
	_sprite = sprite;
}

inline Sprite* pSprite::getSprite() {
	return this->_sprite;
}

inline void pSprite::setSprite(Sprite* sprite) {
	this->_sprite = sprite;
}

inline void pSprite::setPosition(Point p) {
	_xposition = p.x;
	_yposition = p.y;
}

inline int pSprite::getPositionX() {
	return this->_xposition;
}

inline int pSprite::getPositionY() {
	return this->_yposition;
}

inline void pSprite::setProperties(ValueMap& properties) {
	_properties = properties;
	// TODO: set Sprite properties
}

inline ValueMap& pSprite::getProperties() {
	return _properties;
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
	fixtureDef.filter.categoryBits = this->getProperties()->at("CategoryBits");
	fixtureDef.filter.maskBits = 0xffff;
	
	this->_body->CreateFixture(&fixtureDef);
}

void pSprite::createRectangularFixture() {
	// get position and size
	int width = this->getProperties().at("width");
	int height = this->getProperties().at("height");
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
	fixtureDef.filter.categoryBits = this->getProperties->at("CategoryBits");
	fixtureDef.filter.maskBits = 0xffff;

	this->_body->CreateFixture(&fixtureDef);
}

void pSprite::addBodyToWorld(b2World* world) {
	b2BodyDef bodyDef;
	// NOTE: tile sprite must have a BodyType property
	//bodyDef.type = this->getProperties()->valueForKey("BodyType");
	bodyDef.type = this->getProperties()->at("BodyType");
	bodyDef.position.Set(
		this->getPositionX() / PixelsPerMeter,
		this->getPositionY() / PixelsPerMeter);
	bodyDef.userDate = this;
	bodyDef.fixedRotation = true;

	this->_body = world->CreateBody(&bodyDef); 
}

void pSprite::addBodyToWorldAtPosition(b2World * world, Point p) {
	b2BodyDef bodyDef;
	// NOTE: tile sprite must have a BodyType property
	//bodyDef.type = this->getProperties()->valueForKey("BodyType");
	bodyDef.type = this->getProperties()->at("BodyType");
	bodyDef.position.Set(
		p.x / PixelsPerMeter,
		p.y / PixelsPerMeter);
	bodyDef.userDate = this;
	bodyDef.fixedRotation = true;

	this->_body = world->CreateBody(&bodyDef); 
}

void pSprite::removeBodyFromWorld(b2World *world) {
	// destory body
	world->DestroyBody(this->_body);
	this->_body = nullptr;
}