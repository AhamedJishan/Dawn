# Dawn
A small C++ game and custom engine built with OpenGL, focused on simplicity and completion.

<div align="center">
  <video src="https://github.com/user-attachments/assets/9e4a5ef2-10b7-48a9-b7d3-9432543e6095" width="800" autoplay loop muted playsinline></video>
  <br/>
  <em>Current build — work in progress</em>
</div>

## TODO

- TODO: fix enemy sounds playing even on pause screen
  
- [Done] Add arena bounds
- [Done] Bigger wave text, "Wave 1"
- [Done] add dmg multiplier
- [Done] enemy speed variety
- [Done] enemy explosion particle effect
- [Done] Enemy knockback on hit
- [Done] The Plasma Overload
    - 0x (Base): Teal / Cyan (Matches the arena, feels stable and cool).
    - 1x (Streak Started): Golden Yellow (Warming up).
    - 2x (Lethal): Fiery Orange/Red (Aggressive, high heat).
    - 3x (Max Power): Deep Bluish-Purple or Violet (The color of extremely high-energy plasma).
- [ ] Make enemy eyes red
- [ ] Refine Recoil, gun bob, and sway. Make shooting feel fun.
- [ ] Add Slight delay before showing game over
- [ ] Create custom Gun model that makes it possible for the neon charge up glow to be shown.
- [ ] Refine enemy model in blender.
- [ ] hell Mode (waves survived, enemies killed)
- [ ] Add a goal, Time taken

### Phase 1: Gameplay Lock & Integration (The "Friction")
- [Done] Refactor GameScene
- [Done] Refine Wave UI
- [WIP] Arena Greybox Integration: Update the WaveManager to spawn enemies from specific glm::vec3 gates that match your Blender layout.
- [WIP] Movement & Pacing Rebalance: Remove the player's infinite sprint, establish a strict base walk speed, and shorten the Kamikaze fuse timer.
- [ ] Ammo Economy: Implement a magazine size, a reload timer, and hook it up to the HUD.

### Phase 2: The "Vibe" & Graphics
- [Done] HDR, Bloom & Gamma Correction
- [Done] Skybox
- [WIP] Final Art Asset Swap: Phase out the Blender greybox and drop in your final modeled/textured assets (Arena, Gun, Enemies).

### Phase 3: Polish, Game Feel & Shipping
- [ ] Release Mode Profiling: Compile in Release mode (-O2 or -O3) and stress-test 50+ enemies to verify your O(N^2) steering/collision loops perform well.
- [WIP] Juice & Feedback: Add shooting visuals (muzzle flash, camera kick), damage feedback (camera shake, red screen vignette), and tweak recoil/bob variables.
- [WIP] Non-Game UI Polish: Refine the ImGui styling, fonts, and button feedback for the Main Menu and Pause screens.
- [ ] Audio Mixing: Do a cohesive final balance pass on the master volumes for BGM, explosions, fuses, and UI.
- [ ] The Blind Playtest: Hand the executable to someone else and watch them play without giving instructions to check difficulty and pacing.
- [ ] Release Build: Package the .exe, configure dynamic asset paths, and bundle the FMOD .dll files for a standalone build.

### Stretch Goals (If Time Permits)
- [ ] The Tactical Dash: Implement the high-skill i-frame dash mechanic to break through enemy walls.
- [ ] Arsenal Expansion: Add one new enemy type and one new gun.