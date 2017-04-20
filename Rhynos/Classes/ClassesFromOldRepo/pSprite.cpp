#include "pSprite.h"

pSprite::pSprite() {
	_sprite = Sprite::create();
}

pSprite::pSprite(Sprite* sprite) {
	_sprite = sprite;
}

inline pSprite::getSprite() {
	return this->_sprite;
}

inline pSprite::setPosition(Point p) {
	_xposition = p.x;
	_yposition = p.y;
}

inline pSprite::getPositionX() {
	return this->_xposition;
}

inline pSprite::getPositionY() {
	return this->_yposition;
}

inline pSprite::setProperties(Value properties) {
	_properties = properties;
	// TODO: set Sprite properties
}

inline pSprite::getProperties() {
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
	fixtureDef.filter.categoryBits = this->getProperties->valueForKey("CategoryBits");
	fixtureDef.filter.maskBits = 0xffff;
	
	this->_body->CreateFixture(&fixtureDef);
}

void pSprite::addBodyToWorld(b2World* world) {
	b2BodyDef bodyDef;
	// NOTE: tile sprite must have a BodyType property
	bodyDef.type = this->getProperties->valueForKey("BodyType");
	bodyDef.position.Set(
		this->getPositionX() / PixelsPerMeter,
		this->getPositionY() / PixelsPerMeter);
	bodyDef.userDate = this;
	bodyDef.fixedRotation = true;

	this->_body = world->CreateBody(&bodyDef); 
}