#include "Player.h"

using namespace cocos2d;

Player::Player(Sprite* sprite) : pSprite(sprite) { this->type = "Player"; }

void Player::setProperties(const ValueMap* properties) {
  this->_properties = properties;
  this->_health = properties->at("Health").asInt();
  this->_maxHealth = this->_health;
  this->_acc = properties->at("Acceleration").asFloat();
  this->_dec = properties->at("Deceleration").asFloat();
  this->_jmp = properties->at("JumpAcceleration").asFloat();
  this->_maxSpeed = properties->at("MaxSpeed").asFloat();
  this->_maxJumpTime = properties->at("MaxJumpTime").asFloat();
  this->_status = PlayerStatus::normal;
    
  int x = properties->at("x").asInt();
  int y = properties->at("y").asInt();
  this->defaultPosition = Point(x, y);
    
}

inline int Player::getHealth() { return this->_health; }

inline int Player::getMaxHealth() { return this->_maxHealth; }

inline void Player::setStatus(PlayerStatus status) {
  this->_status = status;
  switch (status) {
    case PlayerStatus::normal:
      this->_destructable = true;
    case PlayerStatus::invincible:
      this->_destructable = false;
  }
}

inline PlayerStatus Player::getStatus() { return this->_status; }

inline void Player::setAcceleration(float acc) { this->_acc = acc; }
inline void Player::setAccelerationToDefault() {
  this->_acc = this->getProperties()->at("Acceleration").asFloat();
}
inline void Player::setDecceleration(float dec) { this->_dec = dec; }
inline void Player::setDeccelerationToDefault() {
  this->_dec = this->getProperties()->at("Deceleration").asFloat();
}
inline void Player::setJumpAcceleration(float jmp) { this->_jmp = jmp; }
inline void Player::setJumpAccelerationToDefault() {
  this->_jmp = this->getProperties()->at("JumpAcceleration").asFloat();
}

inline void Player::setPlayerNum(int num) { this->_playerNum = num; }

inline int Player::getPlayerNum() { return this->_playerNum; }

bool Player::isFloating() { return this->_floating; }
bool Player::isDead() { return this->_health == 0; }
bool Player::isOffMap() { return this->getPositionY() < 0.0; }
bool Player::checkWin(Point p) {
    if (this->getPositionX() > p.x) {
        return true;
    }
    return false;
}

bool Player::isDestructable() { return this->_destructable; }

int Player::hurtBy(int damage) {
  if (this->_destructable) {
    this->_health = std::max(this->getHealth() - damage, 0);
  }
  return this->getHealth();
}

int Player::healBy(int health) {
  this->_health = std::min(this->getHealth() + health, this->getMaxHealth());
  return this->getHealth();
}

Point Player::getDefaultPosition() {
    return this->defaultPosition;
}

Point Player::getCurrentPosition() {
  int x = this->getPositionX();
  int y = this->getPositionY();
  return Point(x, y);
}

void Player::destroy(b2World* world) { this->removeBodyFromWorld(world); }

void Player::respawnAt(b2World* world, Point p) {
  this->addBodyToWorldAtPosition(world, p);
  this->createRectangularFixture();
}

// currently using forces for a slow acceleration to top speed
void Player::applyMoveRight() {
  // check if player was moving backward
  b2Vec2 force;
  bool accelerating;
  if (this->_body->GetLinearVelocity().x < 0) {
    force = b2Vec2(this->_dec, 0);
    accelerating = false;
  } else {
    force = b2Vec2(this->_acc, 0);
    accelerating = true;
  }
  // rate limit to max speed
  const b2Vec2& linearVelocity = this->_body->GetLinearVelocity();
  if ((std::abs(linearVelocity.x) < this->_maxSpeed && accelerating) ||
      !accelerating) {
    this->_body->ApplyForceToCenter(force, true);
  }
}

void Player::applyMoveLeft() {
  // check if player was moving backward
  b2Vec2 force;
  bool accelerating;
  if (this->_body->GetLinearVelocity().x > 0) {
    force = b2Vec2(-this->_dec, 0);
    accelerating = false;
  } else {
    force = b2Vec2(-this->_acc, 0);
    accelerating = true;
  }
  // rate limit to max speed
  const b2Vec2& linearVelocity = this->_body->GetLinearVelocity();
  if ((std::abs(linearVelocity.x) < this->_maxSpeed && accelerating) ||
      !accelerating) {
    this->_body->ApplyForceToCenter(force, true);
  }
}

void Player::applyJump() {
  // apply jump impluse / force
  const b2Vec2& worldCenter = this->_body->GetWorldCenter();
  if (this->canJump()) {
    if (this->_jumpTime <= 0) {
      this->_body->ApplyLinearImpulse(b2Vec2(0, this->_jmp), worldCenter, true);
      applyJumpSound();
    } else {
      this->_body->ApplyForceToCenter(b2Vec2(0, this->_jmp), true);
    }
    this->_jumpTime += TimeStep;
  }
  // apply drag force if horizontal velocity exceeds Air and in air
  const b2Vec2& linearVelocity = this->_body->GetLinearVelocity();
  if (linearVelocity.x >= Air && linearVelocity.y != 0) {
    this->_body->ApplyForceToCenter(b2Vec2(Drag, 0), true);
  }
}

void Player::resetJumpTime() { this->_jumpTime = 0.0; }

void Player::applyJumpSound() { 
  CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("audio/tap.wav");
}

// currently does not account for slopes!
// players can't initiate a jump in free-fall
bool Player::canJump() {
  if (this->_body->GetLinearVelocity().y >= 0 &&
      this->_jumpTime < this->_maxJumpTime) {
    return true;
  } else {
    return false;
  }
}

void Player::setLayer(int layerNum) {
  this->_layernum = layerNum;
  b2Filter filter;
  int layerBits;
  switch (layerNum) {
    case 1:
      layerBits = Layer1Bits;
      break;
    case 2:
      layerBits = Layer2Bits;
      break;
    case 3:
      layerBits = Layer3Bits;
  }
  // iterate over all fixtures
  for (b2Fixture* fixture = this->_body->GetFixtureList(); fixture;
       fixture = fixture->GetNext()) {
    filter = fixture->GetFilterData();
    // set category bits
    filter.categoryBits = PlayerBits;
    // set mask bits
    filter.maskBits = layerBits;
    fixture->SetFilterData(filter);
  }
}

void Player::applyInput(const cocos2d::EventKeyboard::KeyCode key) {
  switch (key) {
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
      applyMoveRight();
      break;
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
      applyMoveLeft();
      break;
    case EventKeyboard::KeyCode::KEY_SPACE:
      applyJump();
      break;
    case EventKeyboard::KeyCode::KEY_1:
      setLayer(1);
      break;
    case EventKeyboard::KeyCode::KEY_2:
      setLayer(2);
      break;
    case EventKeyboard::KeyCode::KEY_3:
      setLayer(3);
      break;
    default:
      break;
  }
}
