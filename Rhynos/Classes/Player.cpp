#include "Player.h"

using namespace cocos2d;

Player::Player(Sprite* sprite) : pSprite(sprite) {}

inline void Player::setProperties(ValueMap* properties) {
  this->_properties = properties;
  this->_health = properties->at("Health").asInt();
  this->_maxHealth = this->_health;
  this->_damage = properties->at("Damage").asInt();
  this->_acc = properties->at("Acceleration").asFloat();
  this->_dec = properties->at("Decceleration").asFloat();
  this->_jmp = properties->at("JumpAcceleration").asFloat();
  this->_maxSpeed = properties->at("MaxSpeed").asFloat();
  this->_maxJumpTime = properties->at("MaxJumpTime").asFloat();
  this->_status = PlayerStatus::normal;
}

inline int Player::getHealth() { return this->_health; }

inline int Player::getMaxHealth() { return this->_maxHealth; }

inline int Player::getDamage() { return this->_damage; }

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
  this->_dec = this->getProperties()->at("Decceleration").asFloat();
}
inline void Player::setJumpAcceleration(float jmp) { this->_jmp = jmp; }
inline void Player::setJumpAccelerationToDefault() {
  this->_jmp = this->getProperties()->at("JumpAcceleration").asFloat();
}

bool Player::isFloating() { return this->_floating; }

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
  int x = this->getProperties()->at("x").asInt();
  int y = this->getProperties()->at("y").asInt();
  return Point(x, y);
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
  const b2Vec2& worldCenter = this->_body->GetWorldCenter();
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
    this->_body->ApplyForce(force, worldCenter, false);
  }
}

void Player::applyMoveLeft() {
  // check if player was moving backward
  const b2Vec2& worldCenter = this->_body->GetWorldCenter();
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
    this->_body->ApplyForce(force, worldCenter, false);
  }
}

void Player::applyJump() {
  // apply jump impluse / force
  const b2Vec2& worldCenter = this->_body->GetWorldCenter();
  if (this->canJump()) {
    if (this->_jumpTime < 0) {
      this->_body->ApplyLinearImpulse(b2Vec2(0, this->_jmp), worldCenter,
                                      false);
    } else {
      this->_body->ApplyForce(b2Vec2(0, this->_jmp), worldCenter, false);
    }
    this->_jumpTime += TimeStep;
  }
  // apply drag force if horizontal velocity exceeds Air and in air
  const b2Vec2& linearVelocity = this->_body->GetLinearVelocity();
  if (linearVelocity.x >= Air && linearVelocity.y != 0) {
    this->_body->ApplyForce(b2Vec2(0, Drag), worldCenter, false);
  }
}

void Player::resetJumpTime() { this->_jumpTime = 0.0; }

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
