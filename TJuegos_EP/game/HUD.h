#pragma once

// ===========================================================================
//  HUD.h
//  Dibuja la vida del jugador y los timers de power ups en pantalla.
//  Usa SDL_RenderFillRect para dibujar sin necesitar fuentes externas.
// ===========================================================================

#include <SDL3/SDL.h>
#include "Boss.h"
class Player;

class HUD {
public:
    SDL_Renderer* renderer = nullptr;
    Player* player = nullptr;
    Boss* boss = nullptr;

    HUD(SDL_Renderer* r, Player* p) : renderer(r), player(p) {}

    // Dibuja todo el HUD — llamar después de scene.render()
    void render();

private:
    void drawHealthBar();     // barra de vida del jugador
    void drawPowerUpTimers(); // timers de coraza y tigre
    void drawBossHealthBar(); 
};