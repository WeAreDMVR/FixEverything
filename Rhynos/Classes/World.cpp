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
        bool playerInvolved = false;

        // Determine if either fixture belongs to a player
        void* fixtureDataA = contact->GetFixtureA()->GetUserData();
        void* fixtureDataB = contact->GetFixtureB()->GetUserData();

        // If fixture user data exists, read it as a string which
        // identifies the type of its owner
        if (fixtureDataA) {
            std::string *tA = static_cast<std::string*>(fixtureDataA);
            std::string typeA = *tA;
            CCLOG("%s", typeA.c_str());
            if (typeA == "Player") {
                CCLOG("detected A");
                Player* p = static_cast<Player*>(contact->GetFixtureA()->GetBody()->GetUserData());
                playerInvolved = true;
                //p->moveAI();
                
                
                
                
            } else if (typeA == "Obstacle") {
                // Read user data as an obstacle, or do further processing
            } else {
                // Don't care

            }
        }

        // Same for the other fixture
        if (fixtureDataB) {
            std::string *tB = static_cast<std::string*>(fixtureDataB);
            std::string typeB = *tB;
            CCLOG("%s", typeB.c_str());
            if (typeB == "Player") {
                CCLOG("Detected B");
                Player* p = static_cast<Player*>(contact->GetFixtureA()->GetBody()->GetUserData());
                playerInvolved = true;
                //p->moveAI();
            } else if (typeB == "Obstacle") {
                // Read user data as an obstacle etc.
            } else {
                // Don't care

            }
        }

        // Now that we know what was involved in the collision, handle it
        if (playerInvolved) {
            // Perhaps call a player function, like
            // player->hurtBy(obstacle->dmg);
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

