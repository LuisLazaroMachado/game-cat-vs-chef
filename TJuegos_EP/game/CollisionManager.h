#pragma once

// ===========================================================================
//  CollisionManager.h
//  Maneja todas las colisiones del juego de forma centralizada.
//  Basado en el estilo del profe: funciones simples con SDL_FRect.
// ===========================================================================

#include <SDL3/SDL.h>

class CollisionManager {
public:
    // Verifica si dos rectángulos se superponen (AABB)
    // Igual al método más simple que usa SDL3
    static bool checkAABB(const SDL_FRect& a, const SDL_FRect& b) {
        return SDL_HasRectIntersectionFloat(&a, &b);
    }

    // Verifica si el jugador cayó sobre un enemigo (golpe desde arriba)
    // El jugador debe estar cayendo y su parte inferior debe tocar la cabeza del enemigo
    static bool isStompingEnemy(const SDL_FRect& player,
        const SDL_FRect& enemy,
        float playerVelY) {
        // el jugador debe estar cayendo (velY > 0)
        if (playerVelY <= 0.0f) return false;

        // parte inferior del jugador toca la mitad superior del enemigo
        float playerBottom = player.y + player.h;
        float enemyMiddle = enemy.y + enemy.h * 0.5f;

        return checkAABB(player, enemy) && playerBottom <= enemyMiddle + 20.0f;
    }
};