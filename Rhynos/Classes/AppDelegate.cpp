#include "AppDelegate.h"

#include "Rhyno.h"

using cocos2d::Director;
using cocos2d::GLViewImpl;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

bool AppDelegate::applicationDidFinishLaunching() {
  auto director = Director::getInstance();
  auto glview = director->getOpenGLView();
  if (!glview) {
    glview = GLViewImpl::create("Rhyno");
    glview->setFrameSize(900, 600);
    director->setOpenGLView(glview);
  }

  auto scene = Rhyno::createScene();
  director->runWithScene(scene);

  return true;
}

void AppDelegate::applicationDidEnterBackground() {}

void AppDelegate::applicationWillEnterForeground() {}
