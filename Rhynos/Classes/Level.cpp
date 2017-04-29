#include "Level.h"

#include "Player.h"

#include "cocos2d.h"

#include <string>

using cocos2d::Event;
using cocos2d::EventKeyboard;
using cocos2d::EventListenerKeyboard;
using cocos2d::Point;
using cocos2d::Size;
using cocos2d::Sprite;
using cocos2d::TMXLayer;
using cocos2d::TMXTiledMap;
using cocos2d::ValueMap;
using std::string;
using std::to_string;

Level::Level() {}

Level* Level::createWithMap(const string& tmxFile) {
  Level* ret = new (std::nothrow) Level();
  const TMXTiledMap* map = TMXTiledMap::create(tmxFile);
  b2World* world = World::init();
  if (ret && map && ret->init()) {
    ret->_map = map;
    ret->_world = world;
    ret->autorelease();
    //ret->schedule(schedule_selector(Level::update), TimeStep);
    ret->scheduleUpdate();
    return ret;
  } else {
    return nullptr;
  }

  // Set up the keyboard listener
  auto keyListener = cocos2d::EventListenerKeyboard::create();
  keyListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event) {
        switch(keyCode) {
          case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                break;
      }
  };

  ret->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener,ret);
}

void Level::loadLayers() {
  // Physics Layers handling
  // isolate the "metax" layers from the map
  const string& meta = "meta";
  for (int i = 1; i <= 3; i++) {
    auto layer = this->_map->getLayer(meta + to_string(i));
    if (layer != nullptr) {
      // hide layer
      layer->setVisible(false);
      // add meta tiles to world
      this->createFixtures(layer);
    }
  }
  // TODO
  // Graphics Layers handling
}

void Level::createFixtures(TMXLayer* layer) {
  // create a rectangular fixture for each tile
  const Size& layerSize = layer->getLayerSize();
  for (int y = 0; y < layerSize.height; y++) {
    for (int x = 0; x < layerSize.width; x++) {
      // generate fixture if a sprite in this position
      Sprite* tileSprite = layer->getTileAt(Point(x, y));
      if (tileSprite) {
        // get properties of the tile
        const int tileGID = layer->getTileGIDAt(Point(x, y));

        const cocos2d::ValueMap properties =
            this->_map->getPropertiesForGID(tileGID).asValueMap();
        // create pSprite
        this->_sprites.emplace_back(new pSprite(tileSprite));
        pSprite* psprite = this->_sprites.back();
        const Point& position = positionForTileCoord(Point(x, y));
        psprite->setPosition(position);
        psprite->setProperties(&properties);
        // load pSprite
        psprite->addBodyToWorld(this->_world);
        const auto& tileSize = this->_map->getTileSize();
        psprite->createRectangularFixture(layer, tileSize, x, y);
      }
    }
  }
}

// TODO
void Level::loadObjects() {
  // get objectGroups of map
  const auto& objectGroups = _map->getObjectGroups();
  for (const auto& objectGroup : objectGroups) {
    // get objects from each objectGroup
    const auto& objects = objectGroup->getObjects();
    for (const auto& object : objects) {
      auto properties = object.asValueMap();
      const auto& type = properties.at("type");
      if (!type.isNull()) {
        this->addObject(type.asString(), properties);
      }
    }
  }
}

pSprite* Level::addObject(const string& className, const ValueMap& properties) {
  // create object
  const auto& name = properties.at("name").asString().c_str();
  // create sprite (Assumes a Spritesheet has been loaded)
  // <name> property should have the name of the .png image for the sprite
  cocos2d::Sprite* sprite = Sprite::createWithSpriteFrameName(name);
  this->addChild(sprite);
  if (className == "Player") {
    // create Player
    this->_sprites.push_back(new Player(sprite));
  } else {
    // create pSprite
    this->_sprites.push_back(new pSprite(sprite));
  }
  pSprite* object = this->_sprites.back();
  const int x = properties.at("x").asInt();
  const int y = properties.at("y").asInt();
  object->setPosition(Point(x, y));
  object->setProperties(&properties);
  object->addBodyToWorld(this->_world);
  object->createRectangularFixture();
  return object;
}

Point Level::positionForTileCoord(const Point& coordinate) {
  const Size& tileSize = _map->getTileSize();
  const int x = coordinate.x * tileSize.width;
  const int y =
      (tileSize.height * tileSize.height) - (coordinate.y * tileSize.height);
  return Point(x, y);
}

Point Level::tileCoordForPosition(const Point& position) {
  const Size& tileSize = _map->getTileSize();
  const int x = position.x / tileSize.width;
  const int y =
      ((tileSize.height * tileSize.height) - position.y) / tileSize.height;
  return Point(x, y);
}

double Level::getCurrentTime() {
  typedef std::chrono::high_resolution_clock hclock;
  typedef std::chrono::duration<double> dsec;

  static const auto& startTime = hclock::now();
  const dsec timeLapse = hclock::now() - startTime;

  return timeLapse.count();
}

void Level::update(float dt) {
  const double currentTime = this->getCurrentTime();
  if (!this->_lastTime) {
    this->_lastTime = currentTime;
  }

  double frameTime = currentTime - this->_lastTime;
  this->_lastTime = currentTime;

  while (frameTime > TimeStep) {
    World::step(this->_world);
    frameTime -= TimeStep;
  }

  for (pSprite* p : this->_sprites) {
    //Update sprites
    //CCLOG("sprite x: %f y: %f", p->getBodyPositionX(), p->getBodyPositionY());
  }

}
