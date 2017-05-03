#include "pSprite.h"

#include "cocos2d.h"

using cocos2d::Point;
using cocos2d::Size;
using cocos2d::Sprite;
using cocos2d::TMXLayer;

pSprite::pSprite() { _sprite = Sprite::create(); }

pSprite::pSprite(Sprite* sprite) { _sprite = sprite; }

pSprite::pSprite(const pSprite& object) {
    _sprite = object._sprite;
}

inline Sprite* pSprite::getSprite() { return this->_sprite; }

inline void pSprite::setSprite(Sprite* sprite) { this->_sprite = sprite; }

void pSprite::createRectangularFixture(TMXLayer* layer, const Size tileSize,
                                       int x, int y) {
  // get position and size
  auto position = layer->getPositionAt(Point(x, y));
  // auto tileSize = this->_map->getTileSize();

  // create shape
  b2PolygonShape shape;
  shape.SetAsBox((tileSize.width / PixelsPerMeter),
                 (tileSize.height / PixelsPerMeter));

  // create fixture
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &shape;
  fixtureDef.density = Density;
  fixtureDef.friction = Friction;
  fixtureDef.restitution = Restitution;
  // NOTE: tile sprite must have a CategoryBits property
  // fixtureDef.filter.categoryBits =
  // this->getProperties()->valueForKey("CategoryBits");
  fixtureDef.filter.categoryBits =
      this->getProperties()->at("CategoryBits").asByte();
  fixtureDef.filter.maskBits = 0xffff;

  this->_body->CreateFixture(&fixtureDef);
}

void pSprite::createRectangularFixture() {
  // get position and size
  int width = this->getProperties()->at("width").asInt();
  int height = this->getProperties()->at("height").asInt();
  // create shape
  b2PolygonShape shape;
  shape.SetAsBox(width / PixelsPerMeter, height / PixelsPerMeter);
  // create fixture
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &shape;
  fixtureDef.density = Density;
  fixtureDef.friction = Friction;
  fixtureDef.restitution = Restitution;
  unsigned char bits = this->getProperties()->at("CategoryBits").asByte();
  fixtureDef.filter.categoryBits = bits;
  fixtureDef.filter.maskBits = bits;

  this->_body->CreateFixture(&fixtureDef);
}

void pSprite::addBodyToWorld(b2World* world) {
  b2BodyDef bodyDef;
  // NOTE: tile sprite must have a BodyType property
  // bodyDef.type = this->getProperties()->valueForKey("BodyType");
  bodyDef.type =
      (b2BodyType)this->getProperties()->at("BodyType").asUnsignedInt();
  bodyDef.position.Set(this->getPositionX() / PixelsPerMeter,
                       this->getPositionY() / PixelsPerMeter);
  bodyDef.userData = this;
  bodyDef.fixedRotation = true;

  this->_body = world->CreateBody(&bodyDef);
}

void pSprite::addBodyToWorldAtPosition(b2World* world, Point p) {
  b2BodyDef bodyDef;
  // NOTE: tile sprite must have a BodyType property
  // bodyDef.type = this->getProperties()->valueForKey("BodyType");
  bodyDef.type =
      (b2BodyType)this->getProperties()->at("BodyType").asUnsignedInt();
  bodyDef.position.Set(p.x / PixelsPerMeter, p.y / PixelsPerMeter);
  bodyDef.userData = this;
  bodyDef.fixedRotation = true;

  this->_body = world->CreateBody(&bodyDef);
}

void pSprite::removeBodyFromWorld(b2World* world) {
  // destory body
  world->DestroyBody(this->_body);
  this->_body = nullptr;
}

void pSprite::updateSprite() {
  float x = PixelsPerMeter * this->getBodyPositionX();
  float y = PixelsPerMeter * this->getBodyPositionY();
  this->setPosition(Point(x, y));
}

void pSprite::setLayer(int layerNum, bool solid) {
  this->_layernum = layerNum;
  b2Filter filter;
  int layerBits;
  switch (layerNum) {
    case 1: 
      layerBits = Layer1Bits;
      break;
    case 2:
      layerBits = Layer2Bits;
      break;
    case 3:
      layerBits = Layer3Bits;
  }
  int solidBits;
  if (solid) {
    solidBits = SolidBits;
  } else {
    solidBits = NotSolidBits;
  }
  // iterate over all fixtures
  for (b2Fixture* fixture = this->_body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
    filiter = fixture->GetFilterData();
    // set category bits
    filter.categoryBits = layerBits | solidBits;
    // set mask bits
    filter.maskBits = layerBits | solidBits;
    fixture->SetFilterData(filter);
  }
}

int pSprite::getLayerNum() {
    return this->_layernum;
  }
