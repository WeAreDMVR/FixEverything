#include "Player.h"
#include "World.h"

#include <Box2D/Box2D.h>
#include <iostream>
#include "Level.h"

// Physics collision callbacks
class MyContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact) {
        // Was a player involved?
        Player* p = NULL;
        AI* ai = NULL;

        // Determine if either fixture belongs to a player
        void* fixtureDataA = contact->GetFixtureA()->GetBody()->GetUserData();
        void* fixtureDataB = contact->GetFixtureB()->GetBody()->GetUserData();

        // If fixture user data exists, read it as a string which
        // identifies the type of its owner
        if (fixtureDataA) {
            pSprite* tA = static_cast<pSprite*>(fixtureDataA);
            std::string typeA = tA->type;
            if (typeA == "Player") {
                p = static_cast<Player*>(tA);
            } else if (typeA == "AI") {
                p = static_cast<AI*>(tA);
            } else if (typeA == "Obstacle") {
                // Read user data as an obstacle, or do further processing
            } else {
                // Don't care
            }
        }

        // Same for the other fixture
        if (fixtureDataB) {
            pSprite* tB = static_cast<pSprite*>(fixtureDataB);
            std::string typeB = tB->type;
            if (typeB == "Player") {
                p = static_cast<Player*>(tB);
            } else if (typeB == "AI") {
                p = static_cast<AI*>(tB);
            } else if (typeB == "Obstacle") {
                // Read user data as an obstacle etc.
            } else {
                // Don't care
            }
        }

    // Now that we know what was involved in the collision, handle it
    if (p) {
      // Perhaps call a player function

      // Reset Jump time because we collided with something
      // Could be more specific
      p->resetJumpTime();
    }
  }

  void EndContact(b2Contact* contact) {
    // Check fixtures as above
    // Perhaps change a flag in a class, like playerIsTouchingGround
  }
};

b2World* World::init() {
  const b2Vec2 gravity(0, Gravity);
  b2World* world = new b2World(gravity);
  world->SetAllowSleeping(false);
  world->SetContinuousPhysics(true);
  world->SetContactListener(new MyContactListener());
  return world;
}

void World::step(b2World* world) {
  static constexpr int velocityIterations = 8;
  static constexpr int positionIterations = 2;

  world->Step(TimeStep, velocityIterations, positionIterations);
}
