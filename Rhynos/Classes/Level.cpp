#include "Level.h"

#include "KeyboardPoller.h"
#include "World.h"

#include "cocos2d.h"

#include <algorithm>
#include <string>
#include <vector>

using namespace cocos2d;

using std::max;
using std::min;
using std::runtime_error;
using std::string;
using std::to_string;
using std::vector;

static const Size tileSize(75.0, 75.0);
static const Size mapSize(40.0, 12.0);
static const Size worldSize(tileSize.width* mapSize.width,
                            tileSize.height* mapSize.height);

Level::Level(const string& tmxFile) {
  TMXTiledMap* map = TMXTiledMap::create(tmxFile);
  b2World* world = World::init();
  init();
  _map = map;
  _world = world;
  autorelease();
  scheduleUpdate();
  KeyboardPoller* layer = KeyboardPoller::create();
  keyPoll = layer;
  addChild(layer);
  addChild(map);
}

void Level::loadLayers() {
  // Physics Layers handling
  // isolate the "metax" layers from the map
  this->over = false;
  const string& meta = "meta";
  for (int i = 1;; i++) {
    auto layer = this->_map->getLayer(meta + to_string(i));
    if (layer == nullptr) {
      // We've found all of the meta layers in the map
      break;
    }
    // hide layer
    layer->setVisible(true);
    // add meta tiles to world
    this->createFixtures(layer);
  }

  // Graphics Layers handling
  // isolate "fgx" and "bgx" layers from the map
  const string& fg = "fg";
  const string& bg = "bg";
  for (int i = 1;; i++) {
    auto FGLayer = this->_map->getLayer(fg + to_string(i));
    auto BGLayer = this->_map->getLayer(bg + to_string(i));

    if (FGLayer == nullptr || BGLayer == nullptr) {
      break;
    }

    // add fg tiles in front and bg layers behind all fg layers
    FGLayer->setPositionZ(10 - i);
    BGLayer->setPositionZ(5 - i);
  }
}

void Level::createFixtures(TMXLayer* layer) {
  // create a rectangular fixture for each tile
  for (int y = 0; y < mapSize.height; y++) {
    for (int x = 0; x < mapSize.width; x++) {
      // generate fixture if a sprite in this position
      Sprite* tileSprite = layer->getTileAt(Point(x, y));
      if (tileSprite) {
        // We work with the centers of all the objects
        tileSprite->setAnchorPoint(Point(0.5, 0.5));
        // get properties of the tile
        const int tileGID = layer->getTileGIDAt(Point(x, y));

        const cocos2d::ValueMap properties =
            this->_map->getPropertiesForGID(tileGID).asValueMap();
        cocos2d::ValueMap* ptr_properties = new ValueMap(properties);
        // create pSprite
        this->_sprites.emplace_back(new pSprite(tileSprite));
        pSprite* psprite = this->_sprites.back();
        // We set position based upon center of tiles
        const Point& position = positionForTileCoord(Point(x, y));
        psprite->setPosition(position);
        psprite->setProperties(ptr_properties);
        // load pSprite
        psprite->addBodyToWorld(this->_world);
        psprite->createRectangularFixture(layer, tileSize, x, y);
      }
    }
  }
}

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
    addPlayer(className, player);
    object = player;

  } else if (className == "AI") {
    // Create an AI
    AI* ai = new AI(sprite);
    ai->setProperties(&properties);
    addPlayer(className, ai);
    object = ai;
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

// Return center of Tile!
Point Level::positionForTileCoord(const Point& coordinate) {
  // We return the center of each tile
  const float x = coordinate.x * tileSize.width + (PixelsPerMeter / 2.0);
  const float y = (mapSize.height * tileSize.height) -
                  ((coordinate.y + 1) * tileSize.height) +
                  (PixelsPerMeter / 2.0);
  return Point(x, y);
}

// Don't know if this function is used, if so might need to adjust for phys pos
Point Level::tileCoordForPosition(const Point& position) {
  const int x = position.x / tileSize.width;
  const int y = mapSize.height - (position.y / tileSize.height) - 1;
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
    
    while (frameTime > TimeStep) {
        
        // Check if player needs to respawn
        if (this->_players["localhost"]->isOffMap()) {
            cocos2d::Point original = this->_players["localhost"]->getDefaultPosition();
            this->_players["localhost"]->setBodyPosition(positionForTileCoord(original));
        }
        
        if (this->_players["ai"]->isOffMap()) {
            cocos2d::Point original = this->_players["ai"]->getDefaultPosition();
            this->_players["ai"]->setBodyPosition(positionForTileCoord(original));
        }
        
        
        this->handleInput();
        
        // Have to cast AI to player cuz its in a list of players
        (static_cast<AI*> (this->_players["ai"]))->analyzeMap(this->_map);
        // Step Physics
        World::step(this->_world);
        frameTime -= TimeStep;
        this->_lastTime += TimeStep;
    }
    
    // Update non-player sprites
    for (pSprite* p : this->_sprites) {
        // Not positive that all of these things actually have
        // sprites/bodies.
        // p->updateSprite();
    }
    
    // Update Players
    this->_players["localhost"]->updateSprite();
    this->_players["ai"]->updateSprite();
    
    const Vec2& rhynoPos = this->_players["localhost"]->getCurrentPosition();
    const Size& winSize = Director::getInstance()->getWinSizeInPixels();
    
    float camera_x = min(worldSize.width - (winSize.width / 2), rhynoPos.x);
    camera_x = max(camera_x, winSize.width / 2);
    
    float camera_y = min(worldSize.height - (winSize.height / 2), rhynoPos.y);
    camera_y = max(camera_y, winSize.height / 2);
    
    Camera::getDefaultCamera()->setPosition(Point(camera_x, camera_y));
    
    if (didWin(camera_x, camera_y) || this->over) {
        if (this->keyPoll->isKeyPressed(cocos2d::EventKeyboard::KeyCode::KEY_ENTER)) {
            Director::getInstance()->popScene();
        }
    }
}

// Write function where if the player is localhost then update their UI
// http://www.cocos2d-x.org/wiki/How_To_Create_A_HUD




bool Level::didWin(float x, float y) {
    // Currently only doing for AI and player
    
    bool playerWin = this->_players["localhost"]->checkWin(Point(2000, 500));
    bool AIWin = (static_cast<AI*> (this->_players["ai"]))->atTarget();
    
    const char* msg;
    if (playerWin && !AIWin) {
        msg = "Player 1 wins! \n Press Enter to go to the main menu.";
        this->over = true;
    } else if (AIWin && !playerWin) {
        msg = "The AI wins! \n Press Enter to go to the main menu.";
        this->over = true;
    } else {
        return false;
    }
    
    if (this->over) {
        auto label = Label::createWithTTF(msg, "fonts/Marker Felt.ttf", 48);
        // position the label on the center of the screen
        label->setPosition(Vec2(x, y));
        
        // Keeep the message in the middle of the screen
        label->setName("winning");
        this->removeChildByName("winning", true);
        this->addChild(label, 1, "winning");
    }
    return true;
}

void Level::handleInput() {
    
    if (this->over) {
        return;
    }
    
    // Arrows
    if (this->keyPoll->isKeyPressed(
                                    cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
        
        this->_players["localhost"]->applyMoveRight();
    }
    if (this->keyPoll->isKeyPressed(
                                    cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
        
        this->_players["localhost"]->applyMoveLeft();
    }
    if (this->keyPoll->isKeyPressed(cocos2d::EventKeyboard::KeyCode::KEY_SPACE)) {
        
        this->_players["localhost"]->applyJump();
    }
    if (this->keyPoll->isKeyPressed(cocos2d::EventKeyboard::KeyCode::KEY_1)) {
        
        this->_players["localhost"]->setLayer(1);
    }
    if (this->keyPoll->isKeyPressed(cocos2d::EventKeyboard::KeyCode::KEY_2)) {
        
        this->_players["localhost"]->setLayer(2);
    }
    if (this->keyPoll->isKeyPressed(cocos2d::EventKeyboard::KeyCode::KEY_3)) {
        
        this->_players["localhost"]->setLayer(3);
    }
}
