#ifdef _PLATFORM_H_
#define _PLATFORM_H_

#include "pSprite.h"

// Typical Platforms should have 'Kinematic' BodyType!!!
// A boulder can be implemented as a Platform with 'Dynamic' BodyType!!!

class Platform : public pSprite {
 public:
  Platform(cocos2d::Sprite* sprite);
  inline void setProperties(cocos2d::ValueMap* properties);

  cocos2d::Point getDefaultPosition();
  cocos2d::Point getCurrentPosition();
 
  void applyMoveRight();
  void applyMoveLeft();
  void applyMoveUp();
  void applyMoveDown();

 private:
  typedef pSprite super;

 protected:
  // how fast the platform moves horizontally
  float _xAcc;
  // how fast the platform moves vertically 
  float _yAcc;
  // the leftmost pixel position the platform can inhabit 
  int _leftBound;
  // the rightmost pixel position the platform can inhabit
  int _rightBound;
  // the upmost pixel position the platform can inhabit
  int _upBound;
  // the downmost pixel position the platform can inhabit
  int _downBound;
}
#endif // _PLATFORM_H_