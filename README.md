# Dawn
A small C++ game and custom engine built with OpenGL, focused on simplicity and completion.

<div align="center">
  <video src="https://github.com/user-attachments/assets/9e4a5ef2-10b7-48a9-b7d3-9432543e6095" width="800" autoplay loop muted playsinline></video>
  <br/>
  <em>Current build — work in progress</em>
</div>

## TODO

- TODO: fix enemy sounds playing even on pause screen

- [ ] Remove killstrea, and 1 shot kill mechanic
- [ ] Implement dash
    - Cooldown system (5s → 2s scales down)
    - Dash kills enemies on contact
    - Add brief invulnerability
    - Add visual feedback (screen effect)
- [ ] Create upgrade selection UI (between waves)
    - Show 3 random upgrades
    - Player selects 1
    - 5–10 second timer
- [ ] CORE UPGRADES
    - Damage upgrade, Reduce hits to kill (5 → 2 cap)
    - Spread upgrade, 1 → 3 → 5 bullets, Cone-based shooting
    - Dash cooldown upgrade, Reduce cooldown per level
- [ ] After each wave, pause game, show upgrade choices, resume game
- [ ] Refine Recoil, gun bob, and sway. Make shooting feel fun.
- [ ] Add Slight delay before showing game over
- [ ] Add a goal, Time taken
- [WIP] Final Art Asset Swap: Phase out the Blender greybox and drop in your final modeled/textured assets (Arena, Gun, Enemies).

### Balance
- [ ] Enemy HP, 5 hits base, 2 hits at max level
- [ ] Enemy deals 35 dmg, so 3 enemies can kill player


### Polish, Game Feel & Shipping
- [ ] Release Mode Profiling: Compile in Release mode (-O2 or -O3) and stress-test 50+ enemies to verify O(N^2) steering/collision loops perform well.
- [WIP] Juice & Feedback: Add shooting visuals (muzzle flash, camera kick), damage feedback (camera shake, red screen vignette), and tweak recoil/bob variables.
- [WIP] Non-Game UI Polish: Refine the ImGui styling, fonts, and button feedback for the Main Menu and Pause screens.
- [ ] Audio Mixing: Do a cohesive final balance pass on the master volumes for BGM, explosions, fuses, and UI.
- [ ] The Blind Playtest: Hand the executable to someone else and watch them play without giving instructions to check difficulty and pacing.
- [ ] Release Build: Package the .exe, configure dynamic asset paths, and bundle the FMOD .dll files for a standalone build.

### Stretch Goals (If Time Permits)
- [ ] The Tactical Dash: Implement the high-skill i-frame dash mechanic to break through enemy walls.
- [ ] Arsenal Expansion: Add one new enemy type and one new gun.