#pragma once

// ===========================================================================
//  PowerUp.h
//  Componente que representa un power up en el mundo.
//  Cuando el jugador lo toca, activa su efecto en el Player.
// ===========================================================================

#include <SDL3/SDL.h>
#include "../engine/Component.h"

// Tipos de power up disponibles
enum class PowerUpType {
    FISH_ARMOR,   // pescado → coraza 7 segundos
    FISH_TIGER    // pescado → tigre 10 segundos (variante)

};


class PowerUp : public Component {
public:
    PowerUpType type = PowerUpType::FISH_ARMOR;
    bool        collected = false;  // si ya fue recogido no colisiona
    float       bobTimer = 0.0f;  // timer para animación de flotado

    float velX = 0.0f;  // velocidad horizontal del pescado
    float velY = 0.0f;  // velocidad vertical del pescado
    float groundY = 300.0f; // suelo donde cae el pescado
    bool  onGround = false;  // si ya tocó el suelo

    void update(float dt) override;

    // Devuelve el rectángulo de colisión del power up en el mundo
    SDL_FRect getBounds() const;
};