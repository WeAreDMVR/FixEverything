//
//  AI.hpp
//  Rhynos
//
//  Created by Long Pham on 5/3/17.
//
//

#ifndef AI_hpp
#define AI_hpp

#include <stdio.h>
#include "Player.h"

using namespace cocos2d;

class AI : public Player {
    public:
        cocos2d::Point target;
        float failProb;
        AI(Sprite* player);

        int atTarget();
        int moveOther(Player::Player* play);
        int move();

};





#endif /* AI_hpp */
