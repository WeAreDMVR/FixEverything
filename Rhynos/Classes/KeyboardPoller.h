#ifndef _KEYBOARD_POLLER_H_
#define _KEYBOARD_POLLER_H_

#include "cocos2d.h"

#include <map>

class KeyboardPoller : public cocos2d::Layer {
 public:
  static cocos2d::Scene* createScene();
  virtual bool init() override;

  bool isKeyPressed(const cocos2d::EventKeyboard::KeyCode& code);
  double keyPressedDuration(const cocos2d::EventKeyboard::KeyCode& code);

  CREATE_FUNC(KeyboardPoller);

 private:
  static std::map<cocos2d::EventKeyboard::KeyCode,
                  std::chrono::high_resolution_clock::time_point>
      keys;
};

#endif  // _KEYBOARD_POLLER_H_
