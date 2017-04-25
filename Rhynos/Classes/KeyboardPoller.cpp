#include "KeyboardPoller.h"

#include <chrono>
#include <map>

using cocos2d::Director;
using cocos2d::Event;
using cocos2d::EventKeyboard;
using cocos2d::EventListenerKeyboard;
using cocos2d::Layer;
using cocos2d::Node;
using cocos2d::Scene;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::map;

Scene* KeyboardPoller::createScene() {
  Scene* scene = Scene::create();

  KeyboardPoller* layer = KeyboardPoller::create();
  scene->addChild(layer);
  return scene;
}

bool KeyboardPoller::init() {
  if (!Layer::init()) {
    return false;
  }

  EventListenerKeyboard* eventListener = EventListenerKeyboard::create();

  Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);
  eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode,
                                    Event* event) {
    // If a key already exists, do nothing as it will already have a time stamp
    // Otherwise, set's the timestamp to now
    if (keys.find(keyCode) == keys.end()) {
      keys[keyCode] = high_resolution_clock::now();
    }
  };
  eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode,
                                     Event* event) {
    // remove the key.  std::map.erase() doesn't care if the key doesnt exist
    keys.erase(keyCode);
  };

  this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener,
                                                                 this);

  return true;
}

bool KeyboardPoller::isKeyPressed(const EventKeyboard::KeyCode& code) {
  // Check if the key is currently pressed by seeing it it's in the std::map
  // keys
  // In retrospect, keys is a terrible name for a key/value paried datatype
  // isnt it?
  if (keys.find(code) != keys.end()) {
    return true;
  }
  return false;
}

double KeyboardPoller::keyPressedDuration(const EventKeyboard::KeyCode& code) {
  if (!isKeyPressed(code)) {
    // Not pressed, so no duration obviously
    return 0;
  }

  // Return the amount of time that has elapsed between now and when the user
  // first started holding down the key in milliseconds
  // Obviously the start time is the value we hold in our std::map keys
  return duration_cast<milliseconds>(high_resolution_clock::now() - keys[code])
      .count();
}
// Because cocos2d-x requres createScene to be static, we need to make other
// non-pointer members static
map<EventKeyboard::KeyCode, high_resolution_clock::time_point>
    KeyboardPoller::keys;
