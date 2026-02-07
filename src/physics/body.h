#pragma once
#include "math/vec2.h"

struct Body {
  Vec2 position;
  Vec2 velocity;
  Vec2 acceleration;
  float mass;
  float inverseMass;
  float restitution; // Bounciness [0, 1]
  float radius;      // For circle collision

  Body(Vec2 pos, float m, float r)
      : position(pos), mass(m), radius(r), velocity(0, 0), acceleration(0, 0),
        restitution(0.8f) {
    inverseMass = (mass > 0.0f) ? 1.0f / mass : 0.0f;
  }

  void applyForce(const Vec2 &force) {
    if (inverseMass == 0.0f)
      return; // Static body
    acceleration = acceleration + (force * inverseMass);
  }

  void integrate(float dt) {
    if (inverseMass == 0.0f)
      return;

    // Symplectic Euler
    velocity = velocity + (acceleration * dt);
    position = position + (velocity * dt);

    // Reset acceleration
    acceleration = Vec2(0, 0);
  }
};
