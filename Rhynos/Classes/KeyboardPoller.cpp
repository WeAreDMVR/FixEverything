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

bool KeyboardPoller::init() {
  if (!Layer::init()) {
    return false;
  }

  EventListenerKeyboard* eventListener = EventListenerKeyboard::create();

  // Key press event handler
  eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode,
                                    Event* event) {
    // Set key to pressed
    if (keys.find(keyCode) == keys.end()) {
      keys[keyCode] = true;
    }
  };

  // Key release event handler
  eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
    // remove the key.  std::map.erase() doesn't care if the key doesnt exist
    keys.erase(keyCode);
  };

  this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener,this);

  return true;
}

bool KeyboardPoller::isKeyPressed(const EventKeyboard::KeyCode& code) {
  // Check if the key is currently pressed by seeing if it's
  // in the keys map
  if (keys.find(code) != keys.end()) {
    return true;
  }
  return false;
}

// Because cocos2d-x requres createScene to be static, we need to make other
// non-pointer members static
map<EventKeyboard::KeyCode, bool> KeyboardPoller::keys;
