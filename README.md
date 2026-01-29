# Water Cube Simulation

## Project Overview

This project is a **physics-first water simulation and visualization system**.  
The long-term goal is to build a **real-time simulator** of water dynamics inside a **1 × 1 × 1 cube**, where disturbances, wave propagation, damping, and other physical parameters can be adjusted interactively and visualized immediately.

The emphasis of the project is on:
- physically meaningful wave behavior
- numerical stability
- clean separation between **simulation** and **rendering**

Rendering is treated as a visualization layer on top of the physics, not the core objective.

---

## Long-Term Goal

Build a C++-based real-time simulator that:
- models water surface dynamics inside a cube
- supports adjustable disturbances (location, strength, damping, wave speed)
- visualizes the evolving surface interactively using OpenGL
- can be extended toward more realistic sloshing and 3D behavior

Ultimately, the simulator should behave like a controllable physical system rather than a precomputed animation.

---

## Current Status

The project is actively under development and currently consists of:

### Physics
- A 2D heightfield surface model \( h(x, y, t) \)
- Velocity-based wave propagation
- Damping and restoring forces
- Stable time integration suitable for real-time updates

### Visualization
- OpenGL rendering using GLFW + GLAD
- Dynamic grid mesh updated every frame
- Animated surface deformation driven by per-vertex height updates
- Debug wireframe and solid rendering modes

At the current stage, the visualization is used to validate and inspect the behavior of the underlying physics model.

---

## Architecture Philosophy

- **Simulation is independent of rendering**
- Physics data lives in CPU memory
- Rendering consumes simulation output each frame
- No reliance on offline CSVs or post-processing for core behavior

This structure allows the simulation to be reused, extended, or tested independently of the graphics layer.

---

## Planned Extensions

- Replace test wave functions with the full physical solver in real time
- Interactive disturbances (e.g. mouse input)
- Standing waves and sloshing modes
- Improved camera and projection
- Visual enhancements (lighting, shading, transparency)
- Potential extension toward volumetric or multi-layer models

---

## Notes

This repository represents an **ongoing experimental simulator**, not a finished product.  
Code structure and parameters may evolve as physical modeling and visualization are refined.
