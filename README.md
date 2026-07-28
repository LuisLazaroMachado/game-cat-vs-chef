# 🐱 Cat vs Chef

Mini-juego de plataformas 2D desarrollado en **C++** con **SDL3**, como proyecto del curso de desarrollo de videojuegos. El jugador controla a un gatito que debe atravesar 3 niveles, esquivando enemigos y enfrentando a un jefe final: **el Chef**, que intenta cocinarlo.

> 📸 *(Agrega aquí un GIF o screenshot del gameplay — esto es lo primero que la gente ve, vale mucho la pena)*

---

## 🎮 Sobre el juego

El jugador debe atravesar 3 niveles esquivando enemigos (perros que patrullan y una paloma que persigue) y recolectando power-ups desde cajas: un pescado que otorga invulnerabilidad temporal, y un pescado especial que transforma al gato en un **tigre gigante** capaz de eliminar enemigos al contacto. Al final del recorrido espera el Chef, un jefe final que ataca con proyectiles (sartenes) que pueden ser reflejados de vuelta.

- **Condición de victoria:** derrotar al Chef.
- **Condición de derrota:** la vida del jugador llega a 0.

---

## 🕹️ Controles

| Tecla | Acción |
|---|---|
| `←` / `→` | Moverse horizontalmente |
| `ESPACIO` | Saltar (mantener presionado = salto más alto). Segundo salto en el aire = *parry* |
| `SHIFT` izquierdo | Dash (esquiva rápida) |
| `ESC` | Salir del juego |
| `↑` / `↓` (en menú) | Cambiar selección |
| `ESPACIO` / `ENTER` (en menú) | Confirmar selección / avanzar pantallas |

---

## ✨ Mecánicas implementadas

- **Movimiento y físicas:** gravedad, salto variable, dash, doble salto (parry).
- **Enemigos:**
  - 🐕 Perros: patrullan de ida y vuelta, mueren al ser pisados.
  - 🕊️ Paloma: persigue activamente al jugador.
  - 👨‍🍳 Boss (Chef): lanza proyectiles, recibe daño al ser reflejados.
- **Power-ups:**
  - 🐟 Pescado: invulnerabilidad temporal.
  - 🐯 Pescado tigre: transforma al jugador en un tigre gigante que elimina perros al contacto.
- **Niveles:** 3 niveles con enemigos y cajas configurables por nivel.
- **Audio:** música de fondo por nivel, tema especial para el jefe final, efectos de salto y daño.
- **Interfaz:** menú principal, pantalla de historia/contexto, pantalla informativa de enemigos y power-ups, pantallas de victoria y derrota.

---

## 🛠️ Tecnologías

- **C++**
- **SDL3**
- **SDL3_image**
- **SDL3_mixer**

---

## 🏗️ Arquitectura del proyecto

```
main.cpp
 ├─ SoundManager         → carga/reproduce audio
 ├─ PlayerBuilder        → crea al jugador
 ├─ LevelBuilder         → crea perros, paloma, boss, cajas, proyectiles, plataformas
 │                          └─ avanza de nivel (advanceLevel)
 ├─ Scene (motor)        → actualiza y renderiza todos los GameObject
 │    ├─ Player          → movimiento, salto, dash, power-ups
 │    ├─ Perro           → patrulla
 │    ├─ Paloma          → persigue al jugador
 │    ├─ Boss            → ataca, recibe daño
 │    ├─ Box / PowerUp   → cajas y power-ups
 │    └─ Projectile      → sartenes del Boss
 ├─ CollisionSystem      → resuelve TODAS las colisiones jugables cada frame
 ├─ GameStateManager     → dibuja Menú / Historia / Info / GameOver / Victoria
 └─ HUD                  → interfaz de vida en pantalla
```

### Carpeta `engine/` — motor base del curso

| Archivo | Responsabilidad |
|---|---|
| `Scene.h` | Contenedor de todos los `GameObject` de la escena; llama `update()`/`render()` en cada uno |
| `GameObject.h` | Entidad base del motor; contiene componentes (Transform, Player, Enemy, etc.) |
| `Component.h` | Clase base de todos los componentes |
| `Transform.h` | Posición y escala de cada objeto |
| `SpriteRenderer` / `SpriteAnimator` | Dibujan y animan spritesheets |
| `Camera` / `FollowCamera` | Cámara que sigue al jugador |
| `AssetManager` | Carga y cachea texturas |

### Carpeta `game/` — lógica propia del juego

| Archivo | Responsabilidad |
|---|---|
| `Player` | Movimiento, salto, dash, power-ups, animación según estado (IDLE, WALK, JUMP, DASH, ARMORED, TIGER) |
| `PlayerBuilder` | Construye el `GameObject` del jugador (sprite, animaciones, componente Player) |
| `Enemy.h` | Clase base de enemigos simples (vida, estado, daño) |
| `Perro` | Enemigo que patrulla entre dos límites y rebota |
| `Paloma` | Enemigo que persigue al jugador |
| `Boss` | Jefe final: se mueve hacia el jugador, ataca con proyectiles, tiene estados de daño |
| `Box` | Cajas que al ser golpeadas desde abajo liberan un power-up |
| `PowerUp` | Power-ups que caen de las cajas (física simple de gravedad) |
| `Projectile` | Sartenes que lanza el Boss; pueden ser reflejadas por el jugador |
| `CollisionManager.h` | Funciones estáticas de colisión: `checkAABB`, `isStompingEnemy` |
| `CollisionSystem` | Centraliza todas las colisiones jugables del loop principal |
| `LevelBuilder` | Construye el nivel completo y maneja el avance de nivel (`advanceLevel`) |
| `GameStateManager` | Controla las pantallas que no son gameplay: Menú, Historia, Info, Game Over, Victoria |
| `SoundManager.h` | Gestor de audio con SDL3_mixer |
| `HUD` | Interfaz en pantalla (vida del jugador, barra de vida del Boss) |

---

## 🔄 Flujo del juego

1. `main()` inicializa SDL3 (video + audio), crea la ventana y el renderer.
2. Se crea un `SoundManager` y se cargan todos los sonidos.
3. Se construye la `Scene` y el jugador mediante `PlayerBuilder`.
4. Se crea la cámara (`Camera` + `FollowCamera`) para que siga al jugador.
5. `LevelBuilder` construye todo el contenido jugable (enemigos, Boss, cajas, proyectiles).
6. Comienza el **game loop**:
   - Se calcula `dt` y se procesan eventos de teclado/ventana.
   - Si el estado es `PLAYING`: se actualiza la escena, se revisan colisiones con plataformas, se avanza de nivel si corresponde, y `CollisionSystem` resuelve todas las colisiones jugables.
   - Se revisa condición de victoria/derrota.
   - Se actualiza la música según la pantalla/nivel actual.
   - Se renderiza la escena y el HUD, o la pantalla correspondiente (Menú, Historia, GameOver, Victoria) según el estado.
7. Al cerrar el juego se libera el audio, las texturas y SDL.

---

## 🎨 Créditos de assets

Los assets de sonido y sprites fueron obtenidos de:

- [Pixabay – Final Boss Music](https://pixabay.com/es/music/search/final%20boss/)
- [itch.io – Spritesheet Assets](https://itch.io/game-assets/tag-spritesheet)

---

## ▶️ Jugarlo sin compilar

Puedes descargar el ejecutable ya compilado (junto a sus DLLs) desde la sección **[Releases](../../releases)** de este repositorio. Descomprime el `.zip` y ejecuta `TJuegos_EP.exe`.

## 🚀 Cómo compilar desde el código fuente

Este proyecto usa librerías de desarrollo externas que no están incluidas en el repositorio (por tamaño). Para compilarlo:

1. Clona el repositorio.
2. Descarga las versiones de desarrollo (**VC**, no MinGW) de:
   - [SDL3](https://github.com/libsdl-org/SDL/releases)
   - [SDL3_image](https://github.com/libsdl-org/SDL_image/releases)
   - [SDL3_mixer](https://github.com/libsdl-org/SDL_mixer/releases)
   - [GLEW](https://glew.sourceforge.net/)
3. Colócalas en tu sistema (por ejemplo `C:\SDL3`, `C:\SDL_Image`, `C:\SDL_Mixer`, `C:\GLEW`) y en Visual Studio, en las propiedades del proyecto, configura:
   - **C/C++ → General → Directorios de inclusión adicionales:**
     ```
     C:\SDL3\include;C:\SDL_Image\include;C:\GLEW\include;C:\SDL_Mixer\include;%(AdditionalIncludeDirectories)
     ```
   - **Vinculador → General → Directorios de bibliotecas adicionales:**
     ```
     C:\SDL3\lib\x64;C:\SDL_Image\lib\x64;C:\GLEW\lib\Release\x64;C:\SDL_Mixer\lib\x64;%(AdditionalLibraryDirectories)
     ```
   - **Vinculador → Entrada → Dependencias adicionales:**
     ```
     SDL3.lib;SDL3_image.lib;SDL3_mixer.lib;glew32.lib;opengl32.lib;%(AdditionalDependencies)
     ```
   - **Eventos de compilación → Evento posterior a la compilación:**
     ```
     xcopy /y "C:\SDL3\lib\x64\SDL3.dll" "$(OutDir)"
     xcopy /y "C:\SDL_Image\lib\x64\SDL3_image.dll" "$(OutDir)"
     copy /y "C:\GLEW\bin\Release\x64\glew32.dll" "$(OutDir)"
     xcopy /y "C:\SDL_Mixer\lib\x64\SDL3_mixer.dll" "$(OutDir)"
     ```
4. Compila en modo `Debug` o `Release` para `x64`.
5. Ejecuta el `.exe` generado.

---

## 📄 Licencia

Este proyecto se distribuye bajo la licencia MIT. Ver [LICENSE](LICENSE) para más detalles.
