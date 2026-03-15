# Dawn
A small C++ game and custom engine built with OpenGL, focused on simplicity and completion.

## TODO

### Phase 1: Core Game Feel & Systems
- [Done] Gunplay Polish: Swap sine-wave recoil for a snappy spring/damped system.
- [Postponed] Shooting Visuals: Add muzzle flash (simple quad) and camera kick/recoil.
- Player Movement Polish: Tweak acceleration/deceleration and wall collisions to make navigation feel incredibly smooth.
- [WIP] Wave Spawning System: Create wave/round logic and distribute spawn points to handle horde pacing.
- Enemy Separation Logic: Implement simple steering/repulsion behavior to prevent overlapping meshes.

### Phase 2: The "Vibe" & Graphics
- HDR & Bloom Pipeline: Refactor rendering to a floating-point framebuffer (GL_RGBA16F), extract brights, apply Gaussian blur, and tonemap.
- Skybox: Implement a cubemap for a starry or cloudy night sky.
- Scene Art & Lighting: Swap placeholder ground/cubes for the glowing flowers.
- Atmosphere Blending: Fine-tune the exponential squared fog to blend seamlessly with the skybox and bloom.

### Phase 3: Polish & Shipping
- Visual Damage Feedback: Add camera shake for nearby explosions and a red screen vignette for taking damage.
- Win State & Transitions: Complete the game loop with a Victory screen and final kill count display.
- Audio Mixing: Balance the master volumes of BGM, explosions, and UI.
- Release Build: Package the .exe, configure dynamic asset paths, and bundle FMOD .dll files for a standalone, double-click-to-play build.

### Polish
- shooting
- enemy chase
- wave system
- Dash into enemy to attack

### Rendering
- SpriteRenderer
- SkinnedMeshRenderer
- Skybox
- bloom + hdr