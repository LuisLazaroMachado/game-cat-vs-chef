#pragma once

// ===========================================================================
//  Box.h
//  Caja de cartón que al recibir un golpe del jugador
//  aparece un power up (pescado).
// ===========================================================================

#include <SDL3/SDL.h>
#include <string>
#include "../engine/Component.h"

// Estados de la caja
enum class BoxState {
    CLOSED,  // cerrada, tiene power up adentro
    OPEN,    // abierta, ya soltó el power up
};

class Box : public Component {
public:
    BoxState    state = BoxState::CLOSED;
    bool        hasSpawned = false;  // si ya spawneó el power up

    // Devuelve el rectángulo de colisión de la caja
    SDL_FRect getBounds() const;

    // Llamar cuando el jugador golpea la caja
    void hit();

    void update(float dt) override;
};