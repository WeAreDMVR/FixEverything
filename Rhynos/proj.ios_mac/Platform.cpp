#include "Platform.h"

#include "cocos2d.h"

using namespace cocos2d;

Platform::Platform(Sprite* sprite) : pSprite(sprite) {}

inline void Platform::setProperties(ValueMap* properties) {
  super::setProperties(properties);
  this->_xAcc = properties->at("XAcceleration").asFloat();
  this->_yAcc = properties->at("YAcceleration").asFloat();
  this->_leftBound = properties->at("LeftBound").asInt();
  this->_rightBound = properties->at("RightBound").asInt();
  this->_upBound = properties->at("UpBound").asInt();
  this->_downBound = properties->at("DownBoudn").asInt();
}

Point Platform::getDefaultPosition() {
  int x = this->getProperties()->at("x").asInt();
  int y = this->getProperties()->at("y").asInt();
  return Point(x, y);
}

Point Platform::getCurrentPosition() {
  int x = this->getPositionX();
  int y = this->getPositionY();
  return Point(x, y);
}

void Platform::applyMoveLeft() {
  const b2Vec2& worldCenter = this->_body->GetWorldCenter();
  b2Vec2 force = b2Vec2(-this->_xAcc, 0);

  if (this->getPositionX() > this->_leftBound)
    this->_body->ApplyForce(force, worldCenter, false);
}

void Platform::applyMoveRight() {
  const b2Vec2& worldCenter = this->_body->GetWorldCenter();
  b2Vec2 force = b2Vec2(this->_xAcc, 0);

  if (this->getPositionX() < this->_rightBound)
    this->_body->ApplyForce(force, worldCenter, false);
}

void Platform::applyMoveUp() {
  const b2Vec2& worldCenter = this->_body->GetWorldCenter();
  b2Vec2 force = b2Vec2(0, this->_yAcc);

  if (this->getPositionY() < this->_upBound)
    this->_body->ApplyForce(force, worldCenter, false);
}

void Platform::applyMoveDown() {
  const b2Vec2& worldCenter = this->_body->GetWorldCenter();
  b2Vec2 force = b2Vec2(0, -this->_yAcc);

  if (this->getPositionX() > this->_downBound)
    this->_body->ApplyForce(force, worldCenter, false);
}
