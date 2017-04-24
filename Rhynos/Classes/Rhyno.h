#ifndef __RHYNO_H__
#define __RHYNO_H__

#include "cocos2d.h"
#include <Box2D/Box2D.h>

class Rhyno : public cocos2d::Scene
{
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
};

#endif // __RHYNO_H__
