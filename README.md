# 🐱 Cat vs Chef

A 2D platformer built from scratch in **C++** with **SDL3**, developed as part of a game development course. The player controls a small cat who must fight through 3 levels, dodging enemies and facing a final boss — **the Chef**, who is trying to cook him.

![Gameplay](media/gameplay.gif)
---

## 🎮 About the game

The player fights through 3 levels, dodging enemies (patrolling dogs and a pursuing pigeon) and collecting power-ups dropped from boxes: a fish that grants temporary invulnerability, and a special fish that transforms the player into a **giant tiger** capable of eliminating enemies on contact. At the end of the run, the player faces the Chef, a boss that attacks with projectiles (frying pans) which can be reflected back at him.

- **Win condition:** defeat the Chef.
- **Lose condition:** player health reaches 0.

---

## 🕹️ Controls

| Key | Action |
|---|---|
| `←` / `→` | Move horizontally |
| `SPACE` | Jump (hold for a higher jump). Second jump in mid-air = *parry* |
| `Left SHIFT` | Dash (quick evade) |
| `ESC` | Quit game |
| `↑` / `↓` (in menu) | Change selection |
| `SPACE` / `ENTER` (in menu) | Confirm selection / advance screens |

---

## ✨ Features

- **Movement & physics:** gravity, variable jump height, dash, double jump (parry).
- **Enemies:**
  - 🐕 Dogs: patrol back and forth, defeated by stomping.
  - 🕊️ Pigeon: actively chases the player.
  - 👨‍🍳 Boss (Chef): throws projectiles, takes damage when they're reflected back.
- **Power-ups:**
  - 🐟 Fish: temporary invulnerability.
  - 🐯 Tiger fish: transforms the player into a giant tiger that eliminates dogs on contact.
- **Levels:** 3 levels with per-level enemy and box configuration.
- **Audio:** per-level background music, a dedicated boss theme, and jump/damage sound effects.
- **UI/Screens:** main menu, story/context screen, an info screen for enemies and power-ups, and win/lose screens.

---

## 🛠️ Tech stack

- **C++**
- **SDL3**
- **SDL3_image**
- **SDL3_mixer**

---

## 🧠 Technical highlights

- **Component-based architecture:** custom lightweight `GameObject` / `Component` system (Transform, SpriteRenderer, SpriteAnimator, Player, Enemy, etc.), inspired by common game-engine design patterns.
- **Builder pattern:** dedicated `PlayerBuilder` and `LevelBuilder` classes to assemble complex game objects and levels, keeping `main.cpp` focused on the game loop rather than construction logic.
- **Centralized collision system:** a single `CollisionSystem` resolves all gameplay collisions (platforms, boxes, power-ups, projectiles, enemies) once per frame, including tunneling-safe platform collision (prevents the player from falling through the floor at high fall speed).
- **Finite-state behavior:** explicit state machines for the player (`IDLE`, `WALK`, `JUMP`, `DASH`, `ARMORED`, `TIGER`) and the boss, driving both gameplay logic and animation.
- **Audio engine integration:** wrapper around SDL3_mixer's new Track/Mixer API, handling music transitions between levels and fire-and-forget sound effect mixing.
- **Game state management:** a dedicated `GameStateManager` for non-gameplay screens (menu, story, info, game over, victory), decoupled from the core gameplay loop.
- **Asset caching:** centralized `AssetManager` to load and cache textures, avoiding redundant disk reads.
- **Cross-machine build & distribution:** configured external native dependencies (SDL3, SDL3_image, SDL3_mixer, GLEW) via Visual Studio project properties, plus a packaged, dependency-free release build for end users.

---

## 🏗️ Project architecture

```
main.cpp
 ├─ SoundManager         → loads/plays audio
 ├─ PlayerBuilder        → builds the player
 ├─ LevelBuilder         → builds dogs, pigeon, boss, boxes, projectiles, platforms
 │                          └─ advances levels (advanceLevel)
 ├─ Scene (engine)       → updates and renders all GameObjects
 │    ├─ Player          → movement, jump, dash, power-ups
 │    ├─ Perro (Dog)     → patrol behavior
 │    ├─ Paloma (Pigeon) → chases the player
 │    ├─ Boss            → attacks, takes damage
 │    ├─ Box / PowerUp   → boxes and power-ups
 │    └─ Projectile      → Boss's frying pans
 ├─ CollisionSystem      → resolves ALL gameplay collisions every frame
 ├─ GameStateManager     → draws Menu / Story / Info / Game Over / Victory
 └─ HUD                  → on-screen health interface
```

### `engine/` — base course engine

| File | Responsibility |
|---|---|
| `Scene.h` | Container for all `GameObject`s in the scene; calls `update()`/`render()` on each one |
| `GameObject.h` | Base engine entity; holds components (Transform, Player, Enemy, etc.) |
| `Component.h` | Base class for all components |
| `Transform.h` | Position and scale of each object |
| `SpriteRenderer` / `SpriteAnimator` | Draw and animate spritesheets |
| `Camera` / `FollowCamera` | Camera that follows the player |
| `AssetManager` | Loads and caches textures |

### `game/` — game-specific logic

| File | Responsibility |
|---|---|
| `Player` | Movement, jump, dash, power-ups, state-driven animation (IDLE, WALK, JUMP, DASH, ARMORED, TIGER) |
| `PlayerBuilder` | Builds the player `GameObject` (sprite, animations, Player component) |
| `Enemy.h` | Base class for simple enemies (health, state, damage) |
| `Perro` | Enemy that patrols between two bounds and bounces back |
| `Paloma` | Enemy that chases the player |
| `Boss` | Final boss: moves toward the player, attacks with projectiles, has damage states |
| `Box` | Boxes that drop a power-up when hit from below |
| `PowerUp` | Power-ups dropped from boxes (simple gravity physics) |
| `Projectile` | Frying pans thrown by the Boss; can be reflected by the player |
| `CollisionManager.h` | Static collision functions: `checkAABB`, `isStompingEnemy` |
| `CollisionSystem` | Centralizes all gameplay collisions in the main loop |
| `LevelBuilder` | Builds the full level and handles level advancement (`advanceLevel`) |
| `GameStateManager` | Controls non-gameplay screens: Menu, Story, Info, Game Over, Victory |
| `SoundManager.h` | Audio manager built on SDL3_mixer |
| `HUD` | On-screen interface (player health, boss health bar) |

---

## 🔄 Game flow

1. `main()` initializes SDL3 (video + audio) and creates the window and renderer.
2. A `SoundManager` is created and all sounds are loaded.
3. The `Scene` and the player are built via `PlayerBuilder`.
4. The camera (`Camera` + `FollowCamera`) is created to follow the player.
5. `LevelBuilder` constructs all playable content (enemies, Boss, boxes, projectiles).
6. The **game loop** begins:
   - `dt` is computed and keyboard/window events are processed.
   - If the state is `PLAYING`: the scene updates, platform collisions are checked, the level advances if needed, and `CollisionSystem` resolves all gameplay collisions.
   - Win/lose conditions are checked.
   - Music is updated based on the current screen/level.
   - The scene and HUD are rendered, or the corresponding screen (Menu, Story, Game Over, Victory) is drawn based on the current state.
7. On exit, audio, textures, and SDL are released.

---

## 🎨 Asset credits

Sound and sprite assets were sourced from:

- [Pixabay – Final Boss Music](https://pixabay.com/es/music/search/final%20boss/)
- [itch.io – Spritesheet Assets](https://itch.io/game-assets/tag-spritesheet)

---

## ▶️ Play without building

You can download the pre-built executable (with all required DLLs) from the **[Releases](../../releases)** section of this repository. Unzip it and run `TJuegos_EP.exe`.

## 🚀 Building from source

This project relies on external native libraries that are not included in the repository (due to size). To build it:

1. Clone the repository.
2. Download the **VC** (not MinGW) development versions of:
   - [SDL3](https://github.com/libsdl-org/SDL/releases)
   - [SDL3_image](https://github.com/libsdl-org/SDL_image/releases)
   - [SDL3_mixer](https://github.com/libsdl-org/SDL_mixer/releases)
   - [GLEW](https://glew.sourceforge.net/)
3. Place them on your system (e.g. `C:\SDL3`, `C:\SDL_Image`, `C:\SDL_Mixer`, `C:\GLEW`), then in Visual Studio, under the project properties, configure:
   - **C/C++ → General → Additional Include Directories:**
     ```
     C:\SDL3\include;C:\SDL_Image\include;C:\GLEW\include;C:\SDL_Mixer\include;%(AdditionalIncludeDirectories)
     ```
   - **Linker → General → Additional Library Directories:**
     ```
     C:\SDL3\lib\x64;C:\SDL_Image\lib\x64;C:\GLEW\lib\Release\x64;C:\SDL_Mixer\lib\x64;%(AdditionalLibraryDirectories)
     ```
   - **Linker → Input → Additional Dependencies:**
     ```
     SDL3.lib;SDL3_image.lib;SDL3_mixer.lib;glew32.lib;opengl32.lib;%(AdditionalDependencies)
     ```
   - **Build Events → Post-Build Event:**
     ```
     xcopy /y "C:\SDL3\lib\x64\SDL3.dll" "$(OutDir)"
     xcopy /y "C:\SDL_Image\lib\x64\SDL3_image.dll" "$(OutDir)"
     copy /y "C:\GLEW\bin\Release\x64\glew32.dll" "$(OutDir)"
     xcopy /y "C:\SDL_Mixer\lib\x64\SDL3_mixer.dll" "$(OutDir)"
     ```
4. Build in `Debug` or `Release` mode for `x64`.
5. Run the generated `.exe`.

---

## 👤 Author

**Luis Lazaro Machado** — [GitHub](https://github.com/LuisLazaroMachado)

---

## 📄 License

This project is distributed under the MIT License. See [LICENSE](LICENSE) for details.
