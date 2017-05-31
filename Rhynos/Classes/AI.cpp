#include <cstdlib>
#include "AI.hpp"
#include "Player.h"
using namespace cocos2d;

AI::AI(Sprite* sprite) : Player(sprite) {
  this->target = Point(3000, 20);
  this->failProb = 0.1;
  this->type = "AI";
}

int AI::atTarget() {
  // Only check x-coordinate for checkpoint for now
  int finish = this->getPositionX();

  if (finish >= this->target.x) {
    return 1;
  }
  return 0;
}

int AI::move() {
  int val = std::rand();
  float prob = val / (float)RAND_MAX;

  // Check if we made it boys
  if (this->atTarget()) {
    return 1;
  }

  // Check if we going to mess up
  if (prob < .1) {
    return 0;
  }

  // Deal with obstacles
  if (prob < .5 && this->getPositionY() < 400) {
    this->applyJump();
    this->applyMoveRight();
  }

  return 1;
}

void AI::applyJumpSound() { return; }
