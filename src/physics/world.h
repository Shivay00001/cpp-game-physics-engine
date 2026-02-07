#pragma once
#include "body.h"
#include <vector>


class World {
public:
  std::vector<Body *> bodies;
  Vec2 gravity;

  World() : gravity(0.0f, -9.81f) {}

  ~World() {
    for (auto b : bodies)
      delete b;
  }

  void addBody(Body *body) { bodies.push_back(body); }

  void step(float dt) {
    for (auto body : bodies) {
      body->applyForce(gravity * body->mass);
      body->integrate(dt);
      checkGroundCollision(body);
    }
  }

  void checkGroundCollision(Body *body) {
    if (body->position.y - body->radius < 0.0f) {
      body->position.y = body->radius;
      body->velocity.y = -body->velocity.y * body->restitution;

      // Friction approximation
      body->velocity.x *= 0.95f;

      if (std::abs(body->velocity.y) < 0.5f)
        body->velocity.y = 0.0f;
    }
  }
};
