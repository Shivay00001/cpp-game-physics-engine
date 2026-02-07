# C++ Game Physics Engine

[![C++17](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.0+-green.svg)](https://cmake.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A **high-performance 2D physics engine** written in modern C++17. This repository implements core rigid body dynamics, including discrete collision detection, impulse-based resolution, and symplectic Euler integration.

## 🚀 Features

- **Rigid Body Dynamics**: Simulation of mass, velocity, acceleration, and forces.
- **Collision Detection**: AABB (Axis-Aligned Bounding Box) and Circle collision checks.
- **Impulse Resolution**: Elastic and inelastic collision response with restitution.
- **Optimized Math**: Custom lightweight Vector2 library for SIMD-ready operations.
- **No External Dependencies**: Zero-dependency implementation (standard library only).

## 📁 Project Structure

```
cpp-game-physics-engine/
├── src/
│   ├── math/
│   │   └── vec2.h        # Vector mathematics
│   ├── physics/
│   │   ├── body.h        # Rigid body definition
│   │   └── world.h       # Physics world & solver
│   └── main.cpp          # Simulation demo
├── CMakeLists.txt
└── README.md
```

## 🛠️ Quick Start

```bash
# Clone
git clone https://github.com/Shivay00001/cpp-game-physics-engine.git
cd cpp-game-physics-engine

# Build
mkdir build && cd build
cmake ..
cmake --build .

# Run Simulation
./PhysicsEngine
```

## 📄 License

MIT License
