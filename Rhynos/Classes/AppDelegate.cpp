#include "AppDelegate.h"

#include "Rhyno.h"

using namespace cocos2d;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

bool AppDelegate::applicationDidFinishLaunching() {
  auto director = Director::getInstance();
  auto glview = director->getOpenGLView();
  if (!glview) {
    glview = GLViewImpl::create("Rhyno");
    glview->setFrameSize(1500, 900);
    glview->setDesignResolutionSize(1500, 900, ResolutionPolicy::EXACT_FIT);
    director->setOpenGLView(glview);
  }

  auto scene = Rhyno::createScene();
  director->runWithScene(scene);
  CCLOG("(%f, %f)", director->getWinSizeInPixels().width,
        director->getWinSizeInPixels().height);

  return true;
}

void AppDelegate::applicationDidEnterBackground() {}

void AppDelegate::applicationWillEnterForeground() {}
