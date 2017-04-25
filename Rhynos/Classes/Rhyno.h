#ifndef _RHYNO_H_
#define _RHYNO_H_

#include "KeyboardPoller.h"

#include <Box2D/Box2D.h>

#include "cocos2d.h"

class Rhyno : public KeyboardPoller {
 public:
  static cocos2d::Scene* createScene();

  virtual bool init() override;

  // a selector callback
  void menuCloseCallback(cocos2d::Ref* pSender);

  // implement the "static create()" method manually
  CREATE_FUNC(Rhyno);

  void update(float) override;

 private:
  // For fixed step physics
  float _currentTime;
  float _lastTime;
  cocos2d::Sprite* _sprite;
};

#endif  // _RHYNO_H_
