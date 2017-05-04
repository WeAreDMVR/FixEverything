#ifndef _PSPRITE_H_
#define _PSPRITE_H_

#include "World.h"

#include "cocos2d.h"

class pSprite : public cocos2d::Node {
  friend cocos2d::Sprite;

 public:
  pSprite();
  pSprite(cocos2d::Sprite* sprite);
  pSprite(const pSprite& obj);

  inline cocos2d::Sprite* getSprite();
  inline void setSprite(cocos2d::Sprite* sprite);
  inline void setPosition(cocos2d::Point p) {
    CCLOG("(%f, %f)", p.x, p.y);
    this->_sprite->setPosition(p);
    _xposition = p.x;
    _yposition = p.y;
  }
  inline int getPositionX() { return this->_xposition; }
  inline float getBodyPositionX() { return this->_body->GetPosition().x; }
  inline int getPositionY() { return this->_yposition; }
  inline float getBodyPositionY() { return this->_body->GetPosition().y; }
  inline void setProperties(const cocos2d::ValueMap* properties) {
    _properties = properties;
    _damage = properties->at("Damage").asInt();
    _destructable = properties->at("Destructable").asBool();
    _floating = properties->at("Floating").asBool();
  }
  inline const cocos2d::ValueMap* getProperties() { return _properties; }
  inline int getDamage() { return this->_damage; }
  inline bool isFloating() { return this->_floating; }
  inline bool isDestructable() { return this->_destructable; }

  void addBodyToWorld(b2World* world);
  void addBodyToWorldAtPosition(b2World* world, cocos2d::Point p);
  // this one is for tiles
  void createRectangularFixture(cocos2d::TMXLayer* layer,
                                const cocos2d::Size tileSize, int x, int y);
  // this one is for sprites
  void createRectangularFixture();
  // NOTE: Does NOT remove sprite from Level
  // Level must do that
  void removeBodyFromWorld(b2World* world);

  void updateSprite();

 private:
  typedef Node super;

 protected:
  b2Body* _body;
  cocos2d::Sprite* _sprite;
  const cocos2d::ValueMap* _properties;
  int _xposition;
  int _yposition;

  // damage object deals to other objects on contact
  int _damage;
  // whether or not the object can be destoryed (die)
  bool _destructable;
  // whether or not the player is floating
  bool _floating;
};

enum class FilterCategory {
  SolidTrack1 = 0x01,
  NonSolidTrack1 = 0x02,
  SolidTrack2 = 0x03,
  NonSolidTrack2 = 0x04,
  SolidTrack3 = 0x05,
  NonSolidTrack3 = 0x06
};

#endif  // _PSPRITE_H_
