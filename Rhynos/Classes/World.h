#ifndef _WORLD_H_
#define _WORLD_H_

#include <Box2D/Box2D.h>

constexpr float TimeStep = 1 / 60.0;
constexpr float PixelsPerMeter = 35.0;
constexpr float Gravity = -20;
constexpr float Friction = 0.06;
constexpr float Density = 1.0;
constexpr float Restitution = 0.0;
constexpr float Drag = -0.21;
constexpr float Air = 50;

class World {
 public:
  static b2World* init();
  static void step(b2World* world);
};

#endif  // _WORLD_H_
