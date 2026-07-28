// ===========================================================================
//  HUD.cpp
// ===========================================================================

#include "HUD.h"
#include "Player.h"
#include "Boss.h"
#include "../engine/GameObject.h" 

void HUD::render() {
    drawHealthBar();
    drawPowerUpTimers();
    // en render() del HUD solo muestra la barra si el boss está activo
    if (boss && boss->owner && boss->owner->active) 
        drawBossHealthBar();
}

void HUD::drawHealthBar() {
    if (!player) return;

    // fondo de la barra de vida (gris oscuro)
    SDL_FRect bgBar = { 20.0f, 20.0f, 200.0f, 20.0f };
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_RenderFillRect(renderer, &bgBar);

    // barra de vida actual (rojo → verde según vida)
    float healthRatio = (float)player->health / 5.0f; // max 5 de vida
    SDL_FRect healthBar = { 20.0f, 20.0f, 200.0f * healthRatio, 20.0f };

    // color cambia según vida: verde=lleno, amarillo=medio, rojo=poco
    if (healthRatio > 0.6f)
        SDL_SetRenderDrawColor(renderer, 50, 200, 50, 255);  // verde
    else if (healthRatio > 0.3f)
        SDL_SetRenderDrawColor(renderer, 200, 200, 50, 255); // amarillo
    else
        SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);  // rojo

    SDL_RenderFillRect(renderer, &healthBar);

    // borde blanco de la barra
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderRect(renderer, &bgBar);

    // indicadores de vida individuales (corazones rectangulares)
    for (int i = 0; i < 5; i++) {
        SDL_FRect heart = {
            20.0f + i * 44.0f, // espaciado entre corazones
            50.0f,
            36.0f, 36.0f
        };
        // relleno: rojo si tiene vida, gris si no
        if (i < player->health)
            SDL_SetRenderDrawColor(renderer, 220, 50, 50, 255); // rojo
        else
            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);  // gris

        SDL_RenderFillRect(renderer, &heart);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderRect(renderer, &heart);
    }
}


void HUD::drawPowerUpTimers() {
    if (!player) return;

    // ── Timer de coraza ───────────────────────────────────────────────
    if (player->armorTimer > 0.0f) {
        // fondo del timer
        SDL_FRect bgArmor = { 20.0f, 100.0f, 150.0f, 15.0f };
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_RenderFillRect(renderer, &bgArmor);

        // barra del timer (azul, se reduce con el tiempo)
        float armorRatio = player->armorTimer / 7.0f; // max 7 segundos
        SDL_FRect armorBar = { 20.0f, 100.0f, 150.0f * armorRatio, 15.0f };
        SDL_SetRenderDrawColor(renderer, 50, 150, 255, 255); // azul
        SDL_RenderFillRect(renderer, &armorBar);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderRect(renderer, &bgArmor);
    }

    // ── Timer de tigre ────────────────────────────────────────────────
    if (player->tigerTimer > 0.0f) {
        // fondo del timer
        SDL_FRect bgTiger = { 20.0f, 125.0f, 150.0f, 15.0f };
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_RenderFillRect(renderer, &bgTiger);

        // barra del timer (naranja, se reduce con el tiempo)
        float tigerRatio = player->tigerTimer / 10.0f; // max 10 segundos
        SDL_FRect tigerBar = { 20.0f, 125.0f, 150.0f * tigerRatio, 15.0f };
        SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255); // naranja
        SDL_RenderFillRect(renderer, &tigerBar);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderRect(renderer, &bgTiger);
    }
}


void HUD::drawBossHealthBar() {
    if (!boss) return;
    SDL_FRect bgBar = { 440.0f, 20.0f, 400.0f, 25.0f };
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_RenderFillRect(renderer, &bgBar);
    float ratio = (float)boss->health / (float)boss->maxHealth;
    SDL_FRect bossBar = { 440.0f, 20.0f, 400.0f * ratio, 25.0f };
    SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
    SDL_RenderFillRect(renderer, &bossBar);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderRect(renderer, &bgBar);
}

