#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "pSprite.h"

enum class PlayerStatus { normal, invincible };

class Player : public pSprite {
 public:
  Player(cocos2d::Sprite* sprite);
  void setProperties(const cocos2d::ValueMap* properties);
  void createRectangularFixture();
  inline int getHealth();
  inline int getMaxHealth();
  inline int getDamage();
  inline void setStatus(PlayerStatus status);
  inline PlayerStatus getStatus();
  inline void setAcceleration(float acc);
  inline void setAccelerationToDefault();
  inline void setDecceleration(float decc);
  inline void setDeccelerationToDefault();
  inline void setJumpAcceleration(float jmp);
  inline void setJumpAccelerationToDefault();
  inline void setPlayerNum(int num);
  inline int getPlayerNum();

  bool isFloating();
  bool isDestructable();

  int hurtBy(int damage);
  int healBy(int health);

  cocos2d::Point getDefaultPosition();
  cocos2d::Point getCurrentPosition();
  // NOTE: do not remove or add sprite to Level
  // Level must do that
  void destroy(b2World* world);
  void respawnAt(b2World* world, cocos2d::Point p);

  void applyMoveRight();
  void applyMoveLeft();
  void applyJump();
  void resetJumpTime();

  void setLayer(int layerNum, bool solid);

 private:
  typedef pSprite super;

  bool canJump();

 protected:
  // current health of player
  int _health;
  // maximum health of player
  int _maxHealth;
  // damage player deals to other objects on contact
  int _damage;
  // whether or not the player is floating
  bool _floating;
  // whether or not the player can be destoryed (die)
  bool _destructable;
  // the player's default acceleration
  float _acc;
  // the player's default decceleration
  float _dec;
  // the player's jump (vertical) acceleration
  float _jmp;
  // the player's max speed
  float _maxSpeed;
  // the player's current jump time
  float _jumpTime;
  // the player's max jump time
  float _maxJumpTime;
  // the player's status
  PlayerStatus _status;
  // the player's number
  int _playerNum;
};

#endif  // _PLAYER_H_
