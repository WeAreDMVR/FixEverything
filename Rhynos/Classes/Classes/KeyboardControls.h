#pragma once

#include "cocos2d.h"

class KeyboardControls : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();  
    CREATE_FUNC(GraphicsScene);
};

