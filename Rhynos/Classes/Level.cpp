#include "Level.h"

using namespace cocos2d;

Level::Level() {}

Level* Level::createWithMap(const std::string& tmxFile) {
  Level* ret = new (std::nothrow) Level();
  TMXTiledMap* map = TMXTiledMap::create(tmxFile);
  b2World* world = World::init();
  if (ret && map && ret->init()) {
    ret->_map = map;
    ret->_world = world;
    ret->autorelease();
    ret->schedule(schedule_selector(Level::update), TimeStep);
    return ret;
  } else {
    return nullptr;
  }
}

void Level::loadLayers() {
  // Physics Layers handling
  // isolate the "metax" layers from the map
  const std::string& meta = "meta";
  for (int i = 1; i <= 3; i++) {
    auto layer = this->_map->getLayer(meta + std::to_string(i));
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
  Size layerSize = layer->getLayerSize();
  for (int y = 0; y < layerSize.height; y++) {
    for (int x = 0; x < layerSize.width; x++) {
      // generate fixture if a sprite in this position
      auto tileSprite = layer->getTileAt(Point(x, y));
      if (tileSprite) {
        // get properties of the tile
        int tileGID = layer->getTileGIDAt(Point(x, y));

        ValueMap& properties =
            this->_map->getPropertiesForGID(tileGID).asValueMap();
        // create pSprite
        pSprite* psprite = new pSprite(tileSprite);
        Point position = positionForTileCoord(Point(x, y));
        psprite->setPosition(position);
        psprite->setProperties(&properties);
        // load pSprite
        psprite->addBodyToWorld(this->_world);
        auto tileSize = this->_map->getTileSize();
        psprite->createRectangularFixture(layer, tileSize, x, y);
        this->_sprites.push_back(*psprite);
      }
    }
  }
}

// TODO
void Level::loadObjects() {
  // get objectGroups of map
  auto objectGroups = _map->getObjectGroups();
  for (auto& objectGroup : objectGroups) {
    // get objects from each objectGroup
    auto objects = objectGroup->getObjects();
    for (auto& object : objects) {
      auto properties = object.asValueMap();
      auto type = properties.at("type");
      if (!type.isNull()) {
        this->addObject(type.asString(), properties);
      }
    }
  }
}

pSprite* Level::addObject(const std::string className, ValueMap& properties) {
  // create object
  auto name = properties.at("name").asString().c_str();
  // create sprite (Assumes a Spritesheet has been loaded)
  // <name> property should have the name of the .png image for the sprite
  auto sprite = Sprite::createWithSpriteFrameName(name);
  this->addChild(sprite);
  pSprite* object;
  if (className == "Player") {
    // create Player
    object = new Player(sprite);
  } else {
    // create pSprite
    object = new pSprite(sprite);
  }
  int x = properties.at("x").asInt();
  int y = properties.at("y").asInt();
  object->setPosition(Point(x, y));
  object->setProperties(&properties);
  object->addBodyToWorld(this->_world);
  object->createRectangularFixture();
  this->_sprites.push_back(*object);
  return object;
}

Point Level::positionForTileCoord(Point coordinate) {
  Size tileSize = _map->getTileSize();
  int x = coordinate.x * tileSize.width;
  int y =
      (tileSize.height * tileSize.height) - (coordinate.y * tileSize.height);
  return Point(x, y);
}

Point Level::tileCoordForPosition(Point position) {
  Size tileSize = _map->getTileSize();
  int x = position.x / tileSize.width;
  int y = ((tileSize.height * tileSize.height) - position.y) / tileSize.height;
  return Point(x, y);
}

double Level::getCurrentTime() {
  typedef std::chrono::high_resolution_clock hclock;
  typedef std::chrono::duration::<double, std::chrono::seconds> dsec;

  static startTime = hclock::now();
  dsec timeLapse = hclock::now() - startTime;

  return timeLapse.count();
}

void Level::update(float dt) {
  currentTime = this->getCurrentTime();
  if (!this->_lastTime) this->_lastTime = currentTime;

  double frameTime = currentTime - this->_lastTime;
  this->_lastTime = currentTime;

  while (frameTime > TimeStep) {
    World::step(this->_world);
    frameTime -= TimeStep;
  }
}
