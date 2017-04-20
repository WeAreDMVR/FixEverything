#include "../box2d/Box2D/Box2D.h"

b2World *myWorld;

b2Body *phys_init() {
  b2Vec2 gravity(0, -9.8);
  myWorld = new b2World(gravity);

  // Ground Boxes
  b2BodyDef obstDef;
  obstDef.position.Set(0.0f, 0.0f);
  obstacle1 = myWorld->CreateBody(&obstDef);
  obstDef.position.Set(0.0f, -1.2f);
  obstacle2 = myWorld->CreateBody(&obstDef);
  obstDef.position.Set(0.0f, -2.4f);
  obstacle3 = myWorld->CreateBody(&obstDef);

  b2PolygonShape obstBox;
  obstBox.SetAsBox(0.5f, 0.5f);

  obstacle1->CreateFixture(&obstBox, 0.0f);
  obstacle2->CreateFixture(&obstBox, 0.0f);
  obstacle3->CreateFixture(&obstBox, 0.0f);

  // Make a dynamic body
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(0.0f, 4.0f);
  b2Body* body = myWorld->CreateBody(&bodyDef);

  b2PolygonShape dynamicBox;
  dynamicBox.SetAsBox(0.5f, 0.5f);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicBox;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;
  body->CreateFixture(&fixtureDef);
  return body;
}

void step(unsigned activate) {
  float32 timeStep = 1/60.0;
  int32 velocityIterations = 8;
  int32 positionIterations = 3;


  if (activate == 1) {
    obstacle1->SetActive(true);
    obstacle2->SetActive(false);
    obstacle3->SetActive(false);
  }
  if (activate == 2) {
    obstacle1->SetActive(false);
    obstacle2->SetActive(true);
    obstacle3->SetActive(false);
  }
  if (activate == 3) {
    obstacle1->SetActive(false);
    obstacle2->SetActive(false);
    obstacle3->SetActive(true);
  }

  myWorld->Step(timeStep, velocityIterations, positionIterations);
}
