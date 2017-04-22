#include "pSprite.h"

class Player: public pSprite {
public:
	Player(Sprite* sprite);
	inline void setProperties(ValueMap& properties);
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
	
	bool isFloating();
	bool isDestructable();
	
	int hurtBy(int damage);
	int healBy(int health);
	
	Point getDefaultPosition();
	Point getCurrentPosition();
	// NOTE: do not remove or add sprite to Level
	// Level must do that
	void destroy();
	void respawnAt(Point p);

	void applyMoveRight();
	void applyMoveLeft();
	void applyJump();
	void resetJumpTime();

private:
	typedef pSprite super;
	typedef Vec2 Point;

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

}

enum class PlayerStatus {
	normal,
	invincible
}