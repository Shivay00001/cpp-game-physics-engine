# C++ Game Physics Engine

A lightweight, header-only-style 2D physics engine written in modern C++17. It implements rigid-body dynamics with force accumulation, symplectic Euler integration, gravity, restitution-based ground collision, and friction approximation — demonstrated through a real-time terminal simulation of a bouncing ball.

## ✨ Features

- **Rigid body dynamics** — mass, inverse-mass (static body support via `inverseMass == 0`), velocity, and acceleration
- **Force accumulation model** — `applyForce()` accumulates forces each frame, cleared after integration
- **Symplectic Euler integration** — stable semi-implicit integration suitable for real-time simulation
- **Gravity simulation** — world-level gravity (`0, -9.81 m/s²`) applied per-body scaled by mass
- **Ground collision & restitution** — positional correction, velocity reflection with configurable bounciness (`restitution`), and a sleep threshold to stop micro-bouncing
- **Friction approximation** — horizontal velocity damping on ground contact
- **Fixed-timestep loop** — 60 FPS simulation (`dt = 0.016s`) with real-time pacing via `std::this_thread::sleep_for`
- **CMake build system** — C++17, single-target build

## 🏗️ Architecture / How It Works

The engine is organized into three small, focused modules under `src/`:

```
src/
├── main.cpp            # Demo: simulates a bouncing ball for 2 seconds at 60 FPS
├── math/
│   └── vec2.h          # 2D vector: arithmetic, length, normalize, stream output
└── physics/
    ├── body.h          # Rigid body: forces, mass, restitution, integration
    └── world.h         # World: gravity, body registry, stepping, ground collision
```

### `Vec2` (`math/vec2.h`)
A minimal 2D vector struct supporting `+`, `-`, scalar multiplication, `length()`, in-place `normalize()`, and `operator<<` for debugging. This is the mathematical foundation for all positions, velocities, and forces.

### `Body` (`physics/body.h`)
Represents a circular rigid body with `position`, `velocity`, `acceleration`, `mass`, `inverseMass`, `restitution` (bounciness in `[0, 1]`), and `radius`.

- **`applyForce(force)`** — converts force to acceleration via Newton's second law (`a = F * inverseMass`) and *accumulates* it. Bodies with `inverseMass == 0` are treated as static and ignore forces.
- **`integrate(dt)`** — performs **symplectic (semi-implicit) Euler** integration: velocity is updated first, then position uses the *new* velocity. This is more stable than explicit Euler for game physics. Acceleration is reset to zero afterward so forces must be re-applied each frame.

### `World` (`physics/world.h`)
Owns all bodies (raw pointers, deleted in the destructor) and drives the simulation:

- **`step(dt)`** — for each body: applies gravity as a force (`gravity * mass`, so acceleration is independent of mass), integrates, then resolves ground collision.
- **`checkGroundCollision(body)`** — if the body's bottom edge (`position.y - radius`) penetrates the ground plane (`y = 0`):
  1. **Positional correction**: snaps the body back to the surface.
  2. **Restitution response**: reflects vertical velocity scaled by `restitution`.
  3. **Friction approximation**: damps horizontal velocity by 5% per contact.
  4. **Sleep threshold**: zeroes vertical velocity below `0.5 m/s` to prevent jitter.

### Simulation loop (`main.cpp`)
Creates a `World`, spawns a ball at `(0, 10)` with mass `1.0`, radius `0.5`, and initial horizontal velocity `(2, 0)`. It then runs 120 fixed steps of `dt = 0.016s` (2 simulated seconds), printing position/velocity telemetry every 10 steps and sleeping ~16 ms per frame to run in real time.

## 🚀 Building & Running (Native)

**Requirements:** CMake ≥ 3.10, a C++17 compiler (GCC, Clang, or MSVC).

```bash
mkdir build && cd build
cmake ..
cmake --build .
./PhysicsEngine
```

Expected output: a telemetry table of the ball's position and velocity as it falls, bounces, and settles.

## 🐳 Running with Docker

A `Dockerfile` is included for containerized execution on any laptop or server.

> ⚠️ **Note:** The provided `Dockerfile` currently references `main.cpp` in the repository root (`RUN g++ -o engine main.cpp`), but the actual source lives in `src/main.cpp` and includes the `physics/` and `math/` headers. **Before building, update the Dockerfile** to match the real layout:

```dockerfile
FROM gcc:latest
WORKDIR /app
COPY . .
RUN g++ -std=c++17 -I src -o engine src/main.cpp
CMD ["./engine"]
```

Then build and run:

```bash
docker build -t cpp-game-physics-engine .
docker run --rm cpp-game-physics-engine
```

Alternatively, skip the Dockerfile and compile via CMake inside a container:

```bash
docker run --rm -v "$PWD":/app -w /app gcc:latest \
  bash -c "mkdir -p build && cd build && cmake .. && make && ./PhysicsEngine"
```

There is **no `docker-compose.yml`** in this repository; since the application is a single self-contained executable with no services or ports, plain `docker build` / `docker run` is the correct and sufficient workflow.

## ✅ Workability Assessment

**Honest evaluation: this is a working early-stage prototype / learning project — not production-ready.**

**What works:**
- The core code in `src/` compiles cleanly as C++17 and runs: gravity, integration, bouncing, friction, and settling all behave correctly for the single-ball demo.
- The architecture (math / body / world separation) is clean and a good foundation.
- The CMake configuration is correct and matches the actual source layout.

**Issues and gaps that must be addressed before serious use:**
1. **Broken Dockerfile** — it compiles a non-existent root `main.cpp` and ignores the `src/` tree and CMake entirely. The container build will fail as shipped (see fix above).
2. **Stray duplicate `main.cpp`** — a second, unrelated `main.cpp` (a trivial threading demo printing "Engine Core Initialized") exists at the repo root and conflicts with the real entry point in `src/`. It should be removed.
3. **No body-to-body collision** — only ground-plane collision exists. There is no circle-circle or broadphase/narrowphase collision detection, which is essential for a "game physics engine."
4. **Raw pointer ownership** — `World` stores `Body*` and manually deletes them; `std::unique_ptr<Body>` would eliminate leak/double-free risk.
5. **No rotation/angular physics** — no torque, angular velocity, or inertia; bodies are point/circle masses only.
6. **No tests, no CI** — there is no test suite or verification pipeline.
7. **Hardcoded demo parameters** — gravity, restitution, friction coefficient, and the sleep threshold are magic numbers baked into the source.
8. **No rendering** — output is console text only; integration with a renderer (SDL, SFML, etc.) is left to the user.

**Verdict:** solid educational foundation with correct core math, suitable for learning and extension — but it requires fixes (Dockerfile, duplicate main) and substantial feature work (collision detection, memory safety, tests) before it could be considered production-grade.

## 📄 License

This project is distributed under the **VisionQuantech Custom Commercial License** (see `LICENSE`):

- **Free** for personal, educational, and non-commercial use.
- **Revenue share (15–30%)** required for individual/indie commercial use.
- **Separate commercial license required** for business/enterprise use — contact **visionquantech@proton.me**.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.