#include "KeyboardPoller.h"
#include "Level.h"

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
    ret->scheduleUpdate();
    KeyboardPoller* layer = KeyboardPoller::create();
    ret->keyPoll = layer;
    ret->addChild(layer);
    return ret;
  } else {
    return nullptr;
  }
}

void Level::loadLayers() {
  // Physics Layers handling
  // isolate the "metax" layers from the map
  const string& meta = "meta";
  for (int i = 1;; i++) {
    auto layer = this->_map->getLayer(meta + to_string(i));
    if (layer == nullptr) {
      // We've found all of the meta layers in the map
      break;
    }
    // hide layer
    layer->setVisible(false);
    // add meta tiles to world
    this->createFixtures(layer);
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
  // isolate the "objx" layers from the map
  const string& obj = "obj";
  for (int i = 1;; i++) {
    const auto objectGroup = this->_map->getObjectGroup(obj + to_string(i));
    if (objectGroup == nullptr) {
      // We've found all of the object layers in the map
      break;
    }
    // get objects from each objectGroup
    const auto objects = objectGroup->getObjects();
    for (const auto object : objects) {
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
  pSprite* object;
  if (className == "Player") {
    // create Player
    Player* player = new Player(sprite);
    player->setProperties(&properties);
    this->_players["localhost"] = player;
    object = player;

  } else {
    // create pSprite
    object = new pSprite(sprite);
    object->setProperties(&properties);
    this->_sprites.push_back(object);
  }
  const int x = properties.at("x").asInt();
  const int y = properties.at("y").asInt();
  object->setPosition(positionForTileCoord(Point(x, y)));
  object->addBodyToWorld(this->_world);
  object->createRectangularFixture();
  return object;
}

Point Level::positionForTileCoord(const Point& coordinate) {
  const Size& tileSize = _map->getTileSize();
  const Size& mapSize = _map->getMapSize();
  const int x = coordinate.x * tileSize.width;
  const int y =
      (mapSize.height * tileSize.height) - (coordinate.y * tileSize.height);
  return Point(x, y);
}

Point Level::tileCoordForPosition(const Point& position) {
  const Size& tileSize = _map->getTileSize();
  const Size& mapSize = _map->getMapSize();
  const int x = position.x / tileSize.width;
  const int y = mapSize.height - (position.y / tileSize.height);
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
    // Check inputs
    this->handleInput();
    // Step Physics
    World::step(this->_world);
    frameTime -= TimeStep;
  }

  // Update non-player sprites
  for (pSprite* p : this->_sprites) {
    // Not positive that all of these things actually have
    // sprites/bodies.
    // p->updateSprite();
  }

  // Update Players
  this->_players["localhost"]->updateSprite();
}

void Level::handleInput() {
  // Arrows
  if (this->keyPoll->isKeyPressed(
          cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
    CCLOG("right");
    this->_players["localhost"]->applyMoveRight();
  }
  if (this->keyPoll->isKeyPressed(
          cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
    CCLOG("right");
    this->_players["localhost"]->applyMoveLeft();
  }
  if (this->keyPoll->isKeyPressed(
          cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW)) {
    CCLOG("right");
    this->_players["localhost"]->applyJump();
  }
}
