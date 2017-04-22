#include "box2d/Box2D/Box2D.h"

const float PixelsPerMeter = 45.0;
const float Gravity = -0.249;
const float Friction = 0.46875;
const float Density = 1.0;
const float Restitution = 0.0;

class World {
public:
	b2World* init();
}