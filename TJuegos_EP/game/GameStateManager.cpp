// ===========================================================================
//  GameStateManager.cpp
// ===========================================================================
#include "GameStateManager.h"
#include "../engine/AssetManager.h"
#include "Enemy.h"
#include "Boss.h"
#include "Box.h"

void GameStateManager::renderScreens(
    SDL_Renderer* renderer, const bool* keys, bool spacePressed, float dt,
    bool& running, GameState& gameState, int& menuSelection, float& deathTimer,
    GameObject* player, Player* pc, LevelData& level, int& currentLevel
) {
    if (gameState == GameState::MENU) {
        SDL_SetRenderScale(renderer, 3.0f, 3.0f);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDebugText(renderer, 130, 60, "GATO PLATAFORMERO");
        SDL_RenderDebugText(renderer, 150, 100, menuSelection == 0 ? "> INICIAR" : "  INICIAR");
        SDL_RenderDebugText(renderer, 150, 115, menuSelection == 1 ? "> SALIR" : "  SALIR");
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);
        if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_DOWN]) menuSelection = 1 - menuSelection;
        if (spacePressed || keys[SDL_SCANCODE_RETURN]) {
            if (menuSelection == 0) {
                gameState = GameState::STORY;
            }
            else {
                running = false;
            }
            SDL_Delay(200);
        }
    }
    else if (gameState == GameState::STORY) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_SetRenderScale(renderer, 2.0f, 2.0f);
        SDL_RenderDebugText(renderer, 40, 60, "Todo comenzo como un dia normal...");
        SDL_RenderDebugText(renderer, 40, 80, "hasta que el Chef del pueblo, hambriento");
        SDL_RenderDebugText(renderer, 40, 100, "de fama, decidio que el gato callejero");
        SDL_RenderDebugText(renderer, 40, 120, "seria el ingrediente principal de su");
        SDL_RenderDebugText(renderer, 40, 140, "plato mas famoso.");
        SDL_RenderDebugText(renderer, 40, 170, "Perros y palomas fueron enviados a");
        SDL_RenderDebugText(renderer, 40, 190, "atraparlo antes de que escape...");
        SDL_RenderDebugText(renderer, 40, 220, "Sobrevive. Encuentra los pescados magicos.");
        SDL_RenderDebugText(renderer, 40, 240, "Llega hasta el Chef. O seras la cena.");
        SDL_RenderDebugText(renderer, 40, 270, "Presiona ESPACIO para continuar");
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);
        if (spacePressed) {
            gameState = GameState::INFO;
        }
    }
    else if (gameState == GameState::INFO) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_FRect r, src;
        r = { 60, 40, 60, 54 };
        src = { 0, 0, 556, 504 };
        SDL_RenderTexture(renderer, assets.load(renderer, "assets/spritesheet_dog.png"), &src, &r);
        SDL_SetRenderScale(renderer, 2.0f, 2.0f);
        SDL_RenderDebugText(renderer, 65, 50, "PERRO: matalo saltando sobre su cabeza");
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);

        r = { 60, 110, 60, 46 };
        src = { 0, 0, 635, 490 };
        SDL_RenderTexture(renderer, assets.load(renderer, "assets/spritesheet_pigeon.PNG"), &src, &r);
        SDL_SetRenderScale(renderer, 2.0f, 2.0f);
        SDL_RenderDebugText(renderer, 65, 80, "PALOMA: solo corre, no la toques");
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);

        r = { 60, 180, 48, 56 };
        src = { 0, 0, 48, 56 };
        SDL_RenderTexture(renderer, assets.load(renderer, "assets/spritesheet_ChefSheet.png"), &src, &r);
        SDL_SetRenderScale(renderer, 2.0f, 2.0f);
        SDL_RenderDebugText(renderer, 65, 110, "CHEF: esquiva y cabecea las sartenes");
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);

        r = { 60, 250, 40, 40 };
        SDL_RenderTexture(renderer, assets.load(renderer, "assets/fish.png"), nullptr, &r);
        SDL_SetRenderScale(renderer, 2.0f, 2.0f);
        SDL_RenderDebugText(renderer, 65, 140, "PESCADO: te vuelve invulnerable 7 seg");
        SDL_RenderDebugText(renderer, 65, 150, "TIGRE: matas perros tocandolos, 10 seg");
        SDL_RenderDebugText(renderer, 65, 170, "Presiona ESPACIO para empezar");
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);
        if (spacePressed) {
            gameState = GameState::PLAYING;
        }
    }
    else if (gameState == GameState::GAMEOVER) {
        deathTimer -= dt;
        SDL_SetRenderScale(renderer, 3.0f, 3.0f);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDebugText(renderer, 90, 80, "El Chef sonrio satisfecho...");
        SDL_RenderDebugText(renderer, 90, 100, "El gatito nunca volvio a casa.");
        SDL_RenderDebugText(renderer, 90, 130, "FIN - Intentalo de nuevo");
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);
        if (deathTimer <= 0.0f) {
            gameState = GameState::MENU;
            currentLevel = 1;
            pc->health = 5;
            pc->armorTimer = 0.0f;
            pc->tigerTimer = 0.0f;
            pc->hurtCooldown = 0.0f;
            pc->state = PlayerState::IDLE;
            player->transform->x = 0.0f;
            player->transform->y = 300.0f;
            for (int i = 0; i < LevelData::NUM_PERROS; i++) {
                level.perros[i]->active = (level.perroSpawns[i].level == 1);
                level.perroComps[i]->state = EnemyState::WALK;
                level.perroComps[i]->health = 1;
            }
            level.palomaObj->active = false;
            level.palomaComp->state = EnemyState::WALK;
            level.palomaComp->health = 1;
            level.bossObj->active = false;
            level.bossComp->state = BossState::WALK;
            level.bossComp->health = 1;
            for (int i = 0; i < LevelData::NUM_BOXES; i++) {
                level.boxObjs[i]->active = (level.boxSpawns[i].level == 1);
                level.boxComps[i]->state = BoxState::CLOSED;
                level.puObjs[i]->active = false;
                level.puComps[i]->collected = false;
            }
        }
    }
    else if (gameState == GameState::VICTORY) {
        SDL_SetRenderScale(renderer, 3.0f, 3.0f);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderDebugText(renderer, 60, 70, "El Chef cayo derrotado.");
        SDL_RenderDebugText(renderer, 60, 90, "El gatito, agotado pero libre,");
        SDL_RenderDebugText(renderer, 60, 110, "por fin pudo volver a casa.");
        SDL_RenderDebugText(renderer, 60, 140, "GANASTE - EVITASTE SER LA CENA");
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    }
}