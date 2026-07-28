#pragma once

// ===========================================================================
//  Perro.h
//  Enemigo simple: se mueve de izquierda a derecha y rebota en los bordes.
// ===========================================================================

#include "Enemy.h"

class Perro : public Enemy {
public:
    float moveSpeed = 120.0f;  // píxeles por segundo
    float leftLimit = -300.0f; // límite izquierdo del recorrido
    float rightLimit = 300.0f; // límite derecho del recorrido

    void update(float dt) override;
};