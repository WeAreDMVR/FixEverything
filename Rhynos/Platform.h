#ifdef _PLATFORM_H_
#define _PLATFORM_H_

#include "pSprite.h"

class Platform : public pSprite {
 public:
  Platform(cocos2d::Sprite* sprite);
  inline void setProperties(cocos2d::ValueMap* properties);

}
#endif // _PLATFORM_H_