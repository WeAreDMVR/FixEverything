#include "World.h"

b2World* World::init() {
	b2Vec2 gravity(0, GRV);
	b2World* world = new b2World(gravity);
	world->setAllowSleeping(true);
	world->setContinuousPhysics(true);
	// TODO: _world->setContactListener();
	return world;
}

void World::step() {
	
}
