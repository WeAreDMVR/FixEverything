#include "box2d/Box2D/Box2D.h"

const float TimeStep = 1 / 60.0;
const float PixelsPerMeter = 45.0;
const float Gravity = -0.249;
const float Friction = 0.46875;
const float Density = 1.0;
const float Restitution = 0.0;
const float Drag = -0.21;
const float Air = 50;

class World {
public:
	b2World* init();
}