//
//  AI.cpp
//  Rhynos
//
//  Created by Long Pham on 5/3/17.
//
//

#include "AI.hpp"
#include "Player.h"
#include <cstdlib>
using namespace cocos2d;


AI::AI(Sprite* sprite) : Player(sprite) {
    this->target = Point(2000, 20);
    this->failProb = 0.1;
}


int AI::atTarget() {
    // Only check x-coordinate for checkpoint for now
    int finish = this->getPositionX();
    
    if (finish >= this->target.x) {
        CCLOG("we made it");
        return 1;
    }
    return 0;
}

int AI::moveOther(Player::Player* play) {
    int val = std::rand();
    float prob = val / (float) RAND_MAX;
    
    /* Check if we made it boys */
    if (this->atTarget()) {
        return 1;
    }
    
    /* Check if we going to mess up */
    if (prob < this->failProb) {
        return 0;
    }
    
    /* Deal with obstacles */
    play->applyMoveRight();
    play->applyJump();
    
    return 1;
    
}

