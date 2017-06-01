#ifndef _CLIENT_SCENE_H_
#define _CLIENT_SCENE_H_

#include <SimpleAudioEngine.h>
#include <ui/UITextField.h>
#include "cocos2d.h"

#include "ClientLevel.h"

#include <future>

class ClientScene : public cocos2d::Scene {
 public:
  virtual bool init() override;

  static ClientScene* create(CocosDenshion::SimpleAudioEngine* audioSource) {
    ClientScene* pRet = new (std::nothrow) ClientScene(audioSource);
    if (pRet && pRet->init()) {
      pRet->autorelease();
      return pRet;
    } else {
      delete pRet;
      pRet = nullptr;
      return nullptr;
    }
  }

 private:
  ClientScene(CocosDenshion::SimpleAudioEngine* audioSource)
      : audioSource_(audioSource) {}

  std::future<ClientLevel*> client_level_future_;
  cocos2d::ui::TextField* host_field_;
  CocosDenshion::SimpleAudioEngine* audioSource_;
};

#endif  // _CLIENT_SCENE_H_
