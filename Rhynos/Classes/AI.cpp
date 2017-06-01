#include <cstdlib>
#include "AI.hpp"
#include "World.h"
#include "Player.h"
using namespace cocos2d;
using std::string;
using std::to_string;

static const Size tileSize(75.0, 75.0);
static const Size mapSize(40.0, 12.0);

AI::AI(Sprite* sprite) : Player(sprite) {
  this->target = Point(3000, 20);
  this->failProb = 0.1;
  this->type = "AI";
  this->prev = Point(-100, -100);
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
  if (prob < 0) {
    return 0;
  }

  // Deal with obstacles
  if (prob < .5 && this->getPositionY() < 400) {
    this->applyJump();
    this->applyMoveRight();
  }

  return 1;
}

Point tileCoordForPosition(const Point& position) {
    const int x = position.x / tileSize.width;
    const int y = mapSize.height - (position.y / tileSize.height) - 1;
    return Point(x, y);
}


void AI::analyzeMap(const TMXTiledMap* map) {
    this->setLayer(2);
    int val = std::rand();
    float prob = val / (float)RAND_MAX;
    
    // Check win
    if (this->atTarget())
        return;
    
    
    auto y_pos = this->getPositionY();
    auto x_pos = this->getPositionX();
    
    auto pt = tileCoordForPosition(Point(x_pos, y_pos));
    auto currLayer = this->getLayerNum();
    
    // Check for out of bounds
    if (this->isOffMap() || pt.x > mapSize.width - 5 || pt.y > mapSize.height - 5) {
        return;
    }
    
    for (int j= currLayer; j < currLayer + 3; j++) {
        // Cycle through the three layer starting at the current one
        int i = (j % 3);
        if (i==0) {
            i = 3;
        }

        TMXLayer* layer1 = map->getLayer("meta" + to_string(i));
    
    	// Start at top right then go to bottom left
        for (int y = pt.y+5; y > pt.y; y--) {
            for (int x = pt.x+5; x > pt.x; x--) {
                Sprite* tileSprite = layer1->getTileAt(pt);

            	// Test where its starting
            	layer1->removeChild(tileSprite);
                if (true) {
                    tileSprite->setAnchorPoint(Point(0.5, 0.5));
                    
                    const int tileGID = layer1->getTileGIDAt(pt);
                    const cocos2d::ValueMap properties = map->getPropertiesForGID(tileGID).asValueMap();
                    cocos2d::ValueMap* ptr_properties = new ValueMap(properties);
                


 					// The next tile is there on this layer so just keep going straight	               
                    if (x == pt.x+1 && y == pt.y+1 && tileGID !=0) {
                        CCLOG("straight");
                        this->applyMoveRight();
                        this->setLayer(i);
                        goto action;
                    }
                    
                    // Jumping will get to another tile
                    if (x > pt.x + 2 && tileGID != 0) {
                        CCLOG("here");
                        this->applyJump();
                        this->applyMoveRight();
                        goto action;
                    }
                    
                    // There's a straight line so u can just jump
                    if (tileGID == 0 && x < pt.x + 3 && y < pt.y+2) {
                        this->applyJump();
                        this->setLayer(i);
                        goto action;
                    }
                    
                }
            }
        }
    }
    
action:
    
    if ((this->prev.x == x_pos) && (this->prev.y == y_pos)) {
        CCLOG("WHY NO WORK");
        //this->applyJump();
    }
    
    this->prev = Point(x_pos, y_pos);
    this->applyMoveRight();
}








