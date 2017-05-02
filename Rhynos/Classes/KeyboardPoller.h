#ifndef _KEYBOARD_POLLER_H_
#define _KEYBOARD_POLLER_H_

#include "cocos2d.h"

#include <map>

class KeyboardPoller : public cocos2d::Layer {
 public:
  virtual bool init() override;

  bool isKeyPressed(const cocos2d::EventKeyboard::KeyCode& code);
  double keyPressedDuration(const cocos2d::EventKeyboard::KeyCode& code);

  CREATE_FUNC(KeyboardPoller);

 private:
  static std::map<cocos2d::EventKeyboard::KeyCode,
                  bool> keys;
};

#endif  // _KEYBOARD_POLLER_H_
