#ifndef _RHYNO_H_
#define _RHYNO_H_

#include "KeyboardPoller.h"
#include "Level.h"
#include "Player.h"
#include "SimpleAudioEngine.h"

#include <Box2D/Box2D.h>

#include "cocos2d.h"

class Rhyno : public cocos2d::Scene {
 public:
  static Scene* createScene();

  virtual bool init() override;

  // a selector callback
  void menuCloseCallback(cocos2d::Ref* pSender);

  // implement the "static create()" method manually
  CREATE_FUNC(Rhyno);

  void update(float) override;
};

#endif  // _RHYNO_H_
