#include "World.h"

b2World* World::init() {
  b2Vec2 gravity(0, Gravity);
  b2World* world = new b2World(gravity);
  world->SetAllowSleeping(true);
  world->SetContinuousPhysics(true);
  // TODO: _world->setContactListener();
  return world;
}

void World::step(b2World* world) {
  static int velocityIterations = 8;
  static int positionIterations = 2;

  world->step(TimeInterval, velocityIterations, positionIterations);
}
