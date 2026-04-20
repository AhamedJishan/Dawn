# Dawn

> A wave-based arena shooter built on a custom C++ engine, written from scratch over ~2.5 months.

https://github.com/user-attachments/assets/478c0555-c2e3-4fc3-83b5-d18a995c1712

**Language:** C++ &nbsp;|&nbsp; **Renderer:** OpenGL &nbsp;|&nbsp; **Build System:** Premake5 &nbsp;|&nbsp; **Platform:** Windows

---

## Overview

Dawn is a first-person arena shooter where the player fights through escalating waves of kamikaze enemies. The goal of this project was to build **a complete, shippable game** on top of a **custom engine built from scratch** — prioritising depth of understanding over scope, and completion over complexity.

The project is split into two layers:

- **Engine** — a static library (`Engine.lib`) providing all core systems: windowing, rendering, input, audio, asset management, and a scene/actor/component framework.
- **Game** — a thin application layer that uses the engine to implement the actual game logic, actors, and scenes.

---

## Controls

- W, A, S, D — Move  
- Mouse — Look  
- Left Click — Shoot  
- Left Shift — Dash  

---

## Gameplay

- First-person movement with a **dash ability** (directional, with cooldown UI)
- **Wave-based combat** — enemies spawn in increasingly difficult waves
- **Kamikaze AI** — enemies charge the player and deal contact damage
- **Upgrade system** — choose stat upgrades between waves
- **Gun mechanics** — bullet spread, camera recoil, gun sway
- Main menu and in-game HUD

---

## Engine Architecture
 
### Application & Lifecycle
 
`Application` is the core runtime. It owns all engine systems (`Window`, `Renderer`, `InputSystem`, `AudioSystem`, `AssetManager`) and drives the game loop. The client side provides a `Game` subclass — its only job is to bootstrap the first scene. After that, scenes drive everything.
 
```
EntryPoint
  ├── DawnGame  (Game subclass — loads first scene)
  └── Application
        ├── Window / Renderer / InputSystem / AudioSystem / AssetManager
        └── Scene  (active, swapped via LoadScene<T>())
              ├── Actor[]
              │     └── Component[]
              ├── SphereCollider[]  (registered separately for collision resolution)
              ├── ParticleSystem[]
              └── EnvironmentSettings  (bloom, fog, lighting — per scene)
```
 
Scene switching is deferred: `Application::LoadScene<T>()` stores a pending scene and swaps it at the start of the next frame, avoiding mid-update teardown.
 
### Scene / Actor / Component
 
`Scene` is where most of the engine's runtime behaviour lives. It manages the actor list (including a pending-actors buffer to safely add actors mid-update), runs collision resolution each frame, and exposes per-scene `EnvironmentSettings` — so each scene configures its own directional light, ambient colour, bloom radius/strength, and fog independently.
 
`Actor`s own a list of `Component`s. This is a simple, hand-rolled composition model — not a full ECS, but it cleanly separates game object logic from scene management and made iteration fast. The `Scene`/`Actor` relationship uses `friend` declarations to keep `AddActor`/`RemoveActor` internal — actors register themselves on construction rather than requiring the caller to manage it.

### The Renderer

The Renderer is owned and driven by the Application. Each frame, the application calls `mRenderer->Draw()`.

The renderer does not directly query scene objects. Instead, rendering is component-driven. It maintains an internal list of MeshRenderer components and iterates over it each frame.

#### Mesh Submission

- Renderable objects register themselves via the MeshRenderer component:
- When a MeshRenderer is created, it automatically registers itself with the Renderer. On destruction, it unregisters itself
- This creates a decoupled flow where:
  - gameplay code creates components
  - components register with systems
  - systems operate on internal lists

#### Render Passes

Rendering is performed in multiple passes:
- **HDR Geometry Pass** : Scene is rendered to an HDR framebuffer using Phong shading with normal mapping.
- **Bloom Pass** : Dual-pass bloom (downsample → upsample) based on Jorge Jimenez, SIGGRAPH 2014 (Call of Duty / Sledgehammer Games).
- **Post-Processing Pass** :  Final composition. Bloom texture blended with HDR scene, ACES tone mapping applied, Output written to default framebuffer.

### Input System

The InputSystem is owned by the Application and built on top of GLFW. It uses a polling-based approach, updating input state once per frame.

Keyboard and mouse input are stored using current and previous state buffers. This allows queries such as:

- `GetKey()` — key held
- `GetKeyDown()` — key pressed this frame
- `GetKeyUp()` — key released this frame

A static `Input` facade provides a clean interface for accessing input anywhere in the codebase

```C++
bool shiftPressed = Input::GetKey(Key::LeftShift);
```

### Asset Management

The `AssetManager` is owned by the `Application` and is responsible for loading and caching engine resources.

Assets are cached internally to avoid redundant disk I/O and GPU uploads. They are stored in string-keyed maps:

- Textures
- Shaders
- Raw models (CPU-side data)
- Meshes (GPU-side data)

Resources are referenced by file path or name and are loaded only once. Once loaded, they persist for the entire lifetime of the application.

#### CPU vs GPU Resource Separation

The engine distinguishes between CPU-side and GPU-side data:

- RawModel / RawMesh — CPU-side data (vertex data, indices, etc.)
- Mesh — GPU-side representation (VAO, buffers, draw-ready)

This separation allows raw data to be reused while generating GPU resources as needed.

#### Asset Access

A static Assets facade provides a simple interface for accessing resources anywhere in the codebase:

```C++
Texture* texture = Assets::GetTexture("Assets/Textures/diffuse.png");
Shader* shader = Assets::GetShader("phong");
```

### Audio

Audio is handled via the FMOD Studio API, wrapped in a custom AudioSystem owned by the Application.

Audio is event-driven and uses FMOD banks for organisation (gameplay, UI, main menu).

#### Playback

There are two ways to play audio:

1) **Direct playback via AudioSystem**
    ```c++
    Application::Get()->GetAudioSystem()->PlayEvent("event:/button_click");
    ```
    This can optionally take a world position for 3D audio. The position is fixed at the time of playback.

2) **Component-based playback (Audio Component)**
   ```c++
   mAudioComponent = new Audio(this);
   mEnemyPresence = mAudioComponent->PlayEvent("event:/enemy_presence");
   ```
   When used via an Audio component, the sound is attached to the owning actor.
   As the actor moves, the sound’s position is updated in real-time.

#### SoundEvent Wrapper
`SoundEvent` is a lightweight wrapper over FMOD event instances, providing control over:

- playback (start/stop/restart)
- volume and pitch
- parameters
- pause state

This allows sounds to be managed and updated independently after being triggered.

### Particle System

A simple custom particle system is implemented for gameplay VFX such as muzzle flashes and explosions.

Particle systems are owned and managed by the `Scene`, which handles its lifetime.

#### Simulation

Particles are simulated on the CPU using a lightweight pool:

- Each particle stores position, velocity, and normalized lifetime (t)
- Emission is controlled via rate and initial burst
- Dead particles are removed using a swap-and-pop approach

Behavior over lifetime is defined using small keyframe curves:

- scaleOverTime
- colorOverTime

#### Rendering

Particles are rendered using instanced rendering:

- Each particle is represented as a cube
- Per-instance data (position, lifetime) is uploaded each frame
- A single draw call renders all particles in the system

#### Design Notes

Designed for small-scale gameplay effects rather than large simulations
Currently uses per-system GPU buffers, which can lead to higher draw calls at scale
A shared/global buffer approach would improve batching and performance

---

## Third-Party Libraries

| Library | Purpose |
|---|---|
| GLFW | Window creation, OpenGL context, input events |
| GLAD | OpenGL function loader |
| Assimp | 3D model loading (.obj) |
| FMOD Studio | Audio playback and mixing |
| stb_image | Texture loading |
| GLM | Math (vectors, matrices, quaternions) |
| ImGui | Debug overlays and UI|

---

## Project Structure

```
Dawn/
├── Engine/                         # Core engine (static library)
│   ├── src/
│   │   ├── Core/                   # Application, Scene, Actor, Component, AssetManager
│   │   │   └── Components/         # Engine-level components (Camera, MeshRenderer, Audio, Collider)
│   │   │
│   │   ├── Rendering/              # Renderer, Mesh, Shader, Texture, Framebuffer, Materials, ParticleSystem
│   │   │   └── Materials/          # Phong, Sky, etc.
│   │   │
│   │   ├── Input/                  # InputSystem, Keyboard, Mouse, KeyCodes
│   │   ├── Audio/                  # FMOD wrapper (AudioSystem, SoundEvent)
│   │   ├── Physics/                # Basic collision and raycasting
│   │   ├── ImGui/                  # Debug UI integration
│   │   ├── Utils/                  # Logging, random utilities
│   │   └── Vendor/                 # [Built with project] Third-party source (glad, imgui, stb)
│   │
│   └── vendor/                     # [Prebuilt] Third-party binaries & headers (GLFW, FMOD, Assimp, glm)
│
├── Game/                           # Game built on top of the engine
│   └── src/
│       ├── Actors/                 # Player, Enemy, Gun, WaveManager, UpgradeManager
│       ├── Components/             # Game-specific components (FPSCamera, Damageable)
│       ├── Scenes/                 # MainMenuScene, GameScene
│       └── EntryPoint.cpp          # Application entry point
│
├── Assets/                         # Shaders, models, textures, audio banks, fonts
├── bin/                            # Built binaries (Debug/Release)
├── bin-int/                        # Intermediate build files
├── premake5.lua                    # Build configuration
└── README.md
```
The engine is built as a static library, with the game implemented as a separate client application on top.

---

## Build Instructions

**Prerequisites:** Visual Studio 2026, Premake5

```bash
git clone https://github.com/AhamedJishan/Dawn.git
cd Dawn

# Generate Visual Studio solution
premake5 vs2026

# Open Dawn.slnx and build in Debug or Release
```

All dependencies (GLFW, Assimp, FMOD, GLAD) are pre-compiled and included under `Engine/vendor/`. No package manager required.

---

## What I Learned

This project was my first attempt at building both a game and the underlying engine simultaneously. It helped me develop a deeper understanding of how real-time systems are structured and interact. Some things that came out of it:

- Designing a custom engine architecture — managing the relationship between Application, Game, and Scene, and structuring systems to remain decoupled but coordinated
- Building a component-driven workflow — where systems operate on registered components (e.g. MeshRenderer, AudioComponent) instead of directly querying game objects
- Implementing a real-time rendering pipeline — including HDR framebuffers, bloom (downsample/upsample), and ACES tone mapping
- Understanding data flow between CPU and GPU — especially through asset management (Raw vs GPU resources) and instanced rendering (particles)
- Managing game state and lifecycle safely — including deferred scene switching to avoid invalid state during updates
- Designing systems with clear responsibility boundaries — separating rendering, input, audio, and gameplay logic
- Identifying limitations and trade-offs — such as draw call overhead in the particle system and areas where batching or different architectures would improve scalability
- Integrating third-party libraries (FMOD, Assimp, ImGui) while keeping engine code independent from them

---

## About

Built by [Jishan Ahamed](https://github.com/AhamedJishan) — Feb 2026 to Apr 2026.

This project was made to deepen my understanding of real-time rendering, engine architecture, and C++ systems programming — and to ship something complete.