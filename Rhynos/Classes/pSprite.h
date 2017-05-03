#ifndef _PSPRITE_H_
#define _PSPRITE_H_

#include "World.h"

#include "cocos2d.h"

constexpr int Layer1Bits =   0x000001;
constexpr int Layer2Bits =   0x000010;
constexpr int Layer3Bits =   0x000100;
constexpr int SolidBits =    0x001000;
constexpr int NotSolidBits = 0x000000;
constexpr int Player1Bits =  0x010000;
constexpr int Player2Bits =  0x100000;

class pSprite : public cocos2d::Node {
  friend cocos2d::Sprite;

 public:
  pSprite();
  pSprite(cocos2d::Sprite* sprite);
  pSprite(const pSprite& obj);

  inline cocos2d::Sprite* getSprite();
  inline void setSprite(cocos2d::Sprite* sprite);
  inline void setPosition(cocos2d::Point p) {
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
    // TODO: set Sprite properties
  }
  inline const cocos2d::ValueMap* getProperties() { return _properties; }
  void addBodyToWorld(b2World* world);
  void addBodyToWorldAtPosition(b2World* world, cocos2d::Point p);
  void createRectangularFixture(cocos2d::TMXLayer* layer,
                                const cocos2d::Size tileSize, int x, int y);
  void createRectangularFixture();
  // NOTE: Does NOT remove sprite from Level
  // Level must do that
  void removeBodyFromWorld(b2World* world);

  void updateSprite();
  void setLayer(int layerNum, bool solid);
  int getLayerNum();

 private:
  typedef Node super;

 protected:
  b2Body* _body;
  cocos2d::Sprite* _sprite;
  const cocos2d::ValueMap* _properties;
  int _xposition;
  int _yposition;
  int _layernum;
};

#endif  // _PSPRITE_H_
