
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <unordered_map> 

#include "engine/Scene.h"
#include "engine/GameObject.h"
#include "engine/SpriteRenderer.h"
#include "engine/SpriteAnimator.h"
#include "engine/Transform.h"
#include "engine/Camera.h"
#include "engine/FollowCamera.h"
#include "engine/AssetManager.h"
#include "game/Player.h"
#include "game/Enemy.h"
#include "game/Perro.h"

#include "game/Paloma.h"
#include "game/Box.h"
#include "game/PowerUp.h"
#include "game/CollisionManager.h"
#include "game/HUD.h"
#include "game/Boss.h"
#include "game/Projectile.h"
#include "game/LevelBuilder.h"
#include "game/SoundManager.h"
#include "game/CollisionSystem.h"
#include "game/GameStateManager.h"
#include "game/PlayerBuilder.h"

int main(int argc, char* argv[]) {

    // ── Inicializar SDL3 ──────────────────────────────────────────────────
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("Error al inicializar SDL: %s", SDL_GetError());
        return 1;
    }

    // ── Crear ventana y renderer ──────────────────────────────────────────
    SDL_Window* window = SDL_CreateWindow("Gato Plataformero", 1280, 720, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    if (!window || !renderer) {
        SDL_Log("Error al crear ventana/renderer: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SoundManager audio;
    audio.init();
    audio.loadGameSounds();

    // ── Crear escena ──────────────────────────────────────────────────────
    Scene scene(renderer);

    // ── Crear el player (gatito) ──────────────────────────────────────────
    PlayerHandles ph = PlayerBuilder::build(scene, renderer);
    GameObject* player = ph.obj;
    SpriteRenderer* playerSprite = ph.sprite;
    SpriteAnimator* playerAnim = ph.anim;
    SpriteAnimator* tigerAnimComp = ph.tigerAnim;
    Player* playerComp = ph.comp;

    // ── HUD ───────────────────────────────────────────────────────────────
    HUD hud(renderer, playerComp);

    // ── Crear cámara que sigue al player ─────────────────────────────────
    GameObject* cameraObj = scene.createGameObject("MainCamera");
    Camera* cam = cameraObj->addComponent<Camera>();
    cam->m_renderer = renderer;
    cam->screenW = 1280;
    cam->screenH = 720;


LevelData level;
    LevelBuilder::build(scene, renderer, player, level);
    hud.boss = level.bossComp;

    // ── Variables del game loop ───────────────────────────────────────────
    bool        running = true;
    Uint64      lastTime = SDL_GetTicks();


    int currentLevel = 1;
    float levelEndX = 1200.0f;

    // Cargar solo 3 texturas de fondo
    SDL_Texture* bgTextures[3];
    bgTextures[0] = assets.load(renderer, "assets/nivel1.png");
    bgTextures[1] = assets.load(renderer, "assets/nivel2.png");
    bgTextures[2] = assets.load(renderer, "assets/nivel3.png");

    GameState gameState = GameState::MENU;
    int menuSelection = 0; // 0 = Iniciar, 1 = Salir
    float deathTimer = 0.0f;
    bool spaceWasDown = false; // para detectar salto y avanzar pantallas

    // ── Game loop ─────────────────────────────────────────────────────────
    while (running) {

        // Delta time
        Uint64 now = SDL_GetTicks();
        float  dt = (now - lastTime) / 1000.0f;
        lastTime = now;
        if (dt > 0.05f) dt = 0.05f; // cap para evitar saltos grandes

        // ── Eventos ──────────────────────────────────────────────────────
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false;
            if (event.type == SDL_EVENT_KEY_DOWN &&
                event.key.scancode == SDL_SCANCODE_ESCAPE) running = false;
        }

        // ── Input de teclado ────────────────────────────
        const bool* keys = SDL_GetKeyboardState(nullptr);
        Player* pc = player->getComponent<Player>();
        bool spaceDown = keys[SDL_SCANCODE_SPACE];
        bool spacePressed = spaceDown && !spaceWasDown;
        spaceWasDown = spaceDown;
        // ── Update y Render ───────────────────────────────────────────────
        float frameStartY = player->transform->y; // posición ANTES de moverse, para detectar atravesamientos
        if (gameState == GameState::PLAYING) {
            scene.update(dt);

        // límite izquierdo del mapa
        if (player->transform->x < 0.0f)
            player->transform->x = 0.0f;

        if (currentLevel == 3)
        {
            float playerWidth =
                playerSprite->width * player->transform->scaleX;

            if (player->transform->x > 1280.0f - playerWidth)
                player->transform->x = 1280.0f - playerWidth;
        }

        // ── Variables de colisión ─────────────────────────────────────────
        float currentFrameW = playerSprite->width * player->transform->scaleX;
        float currentFrameH = playerSprite->height * player->transform->scaleY;

        SDL_FRect playerBounds = {
            player->transform->x,
            player->transform->y,
            currentFrameW,
            currentFrameH
        };

        if (spacePressed) {
            SDL_Log(">>> INTENTANDO REPRODUCIR SALTO, gameState=%d", (int)gameState);
            audio.playSfx("salto");
        }

        if (pc) {
            pc->onGround = false;
        }

        // ── Bucle de Plataformas (con detección de atravesamiento) ─────────
        for (int i = 0; i < LevelData::NUM_PLATFORMS; i++) {
            Platform& plat = level.platforms[i];
            bool xOverlap = (playerBounds.x < plat.x + plat.w) &&
                (playerBounds.x + playerBounds.w > plat.x);
            if (!xOverlap) continue;

            float prevBottom = frameStartY + currentFrameH;   // dónde estaban los pies ANTES de moverse
            float currBottom = player->transform->y + currentFrameH; // dónde están AHORA

            // Si cae y "cruzó" la plataforma en este frame (aunque haya sido de un salto grande)
            if (pc->velY >= 0.0f && prevBottom <= plat.y + 10.0f && currBottom >= plat.y) {
                player->transform->y = plat.y - currentFrameH;
                pc->velY = 0.0f;
                pc->onGround = true;
                if (pc->state == PlayerState::JUMP) {
                    pc->state = PlayerState::IDLE;
                }
            }
        }


        // ── Cambio de nivel (Máximo 3 niveles) ──────────────────────────────
        if (player->transform->x >= levelEndX && currentLevel < 3) {
            player->transform->x = 0.0f;
            player->transform->y = 350.0f;
            cameraObj->transform->x = 0.0f;
            LevelBuilder::advanceLevel(level, currentLevel);
        }

        CollisionSystem::checkAll(player, pc, playerBounds, playerSprite, playerAnim, tigerAnimComp, level, audio, renderer);
     
        if (level.bossComp->state == BossState::DEAD && gameState == GameState::PLAYING) {
            gameState = GameState::VICTORY;
            audio.playMusic("victoria", false);
        }
        if (pc->health <= 0 && gameState == GameState::PLAYING) {
            gameState = GameState::GAMEOVER;
            deathTimer = 3.0f;
            audio.playMusic("muerte", false);
            }
        } // cierra if (gameState == PLAYING)

        // ── Música según pantalla/nivel ─────────────────────────────────────

        bool isMenuOrPlayingScreen = (gameState == GameState::MENU || gameState == GameState::INFO ||
            gameState == GameState::PLAYING);
        static bool loggedOnce = false;
        if (!loggedOnce) { SDL_Log(">>> Primer frame: isMenuOrPlayingScreen=%d currentLevel=%d", isMenuOrPlayingScreen, currentLevel); loggedOnce = true; }
        audio.updateLevelMusic(isMenuOrPlayingScreen, currentLevel, gameState == GameState::PLAYING);

        // ── Render ────────────────────────────────────────────────────────
        SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
        SDL_RenderClear(renderer);

        if (gameState == GameState::PLAYING) {
            SDL_FRect bgDst = { 0.0f, 0.0f, 1280.0f, 720.0f };
            SDL_RenderTexture(renderer, bgTextures[currentLevel - 1], nullptr, &bgDst);
            scene.render();
            hud.render();
        }
        else {
            GameStateManager::renderScreens(renderer, keys, spacePressed, dt, running,
                gameState, menuSelection, deathTimer, player, pc, level, currentLevel);
        }

        SDL_RenderPresent(renderer);
    }

    // ── Limpieza ─────────────────────────────────────────
    audio.clear();
    assets.clear();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}