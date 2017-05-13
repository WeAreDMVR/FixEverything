#include "World.h"
#include "Player.h"

#include <Box2D/Box2D.h>
#include <iostream>
#include <Level.h>

// Physics collision callbacks
class MyContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact) {

        CCLOG("BeginContact");

        // Was a player involved?
        Player* p = NULL;

        // Determine if either fixture belongs to a player
        void* fixtureDataA = contact->GetFixtureA()->GetBody()->GetUserData();
        void* fixtureDataB = contact->GetFixtureB()->GetBody()->GetUserData();

        // If fixture user data exists, read it as a string which
        // identifies the type of its owner
        if (fixtureDataA) {
            pSprite* tA = static_cast<pSprite*>(fixtureDataA);
            std::string typeA = tA->type;
            CCLOG("%s", typeA.c_str());
            if (typeA == "Player") {
                CCLOG("detected A");
                p = static_cast<Player*>(tA);
            } else if (typeA == "AI") {
                AI* p = static_cast<AI*>(tA);
                CCLOG("AI SHOULD MOVE A");
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
            CCLOG("%s", typeB.c_str());
            if (typeB == "Player") {
                CCLOG("Detected B");
                p = static_cast<Player*>(tB);
                //p->moveAI();
            } else if (typeB == "AI") {
                p = static_cast<AI*>(tB);
                CCLOG("AI SHOULD MOVE B");
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
  world->SetAllowSleeping(true);
  world->SetContinuousPhysics(true);
  world->SetContactListener(new MyContactListener());
  return world;
}

void World::step(b2World* world) {
  static constexpr int velocityIterations = 8;
  static constexpr int positionIterations = 2;

  world->Step(TimeStep, velocityIterations, positionIterations);
}

