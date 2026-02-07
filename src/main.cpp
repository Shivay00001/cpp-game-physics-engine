#include "physics/world.h"
#include <chrono>
#include <iostream>
#include <thread>


int main() {
  World world;

  // Create a ball
  Body *ball = new Body(Vec2(0.0f, 10.0f), 1.0f, 0.5f);
  ball->velocity = Vec2(2.0f, 0.0f); // Give it some horizontal velocity
  world.addBody(ball);

  float dt = 0.016f; // 60 FPS

  std::cout << "Starting Simulation..." << std::endl;
  std::cout << "Time(s) | Position Y | Velocity Y" << std::endl;
  std::cout << "-----------------------------------" << std::endl;

  for (int i = 0; i < 120; ++i) { // Simulate 2 seconds
    world.step(dt);

    if (i % 10 == 0) {
      std::cout << (i * dt) << "s  | " << ball->position.y << " m  | "
                << ball->velocity.y << " m/s" << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }

  std::cout << "Simulation finished." << std::endl;
  return 0;
}
