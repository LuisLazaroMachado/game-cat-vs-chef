#pragma once

// ===========================================================================
//  Paloma.h
//  Enemigo volador: sigue al jugador en cualquier dirección.
// ===========================================================================

#include "Enemy.h"

class GameObject; // declaración adelantada

class Paloma : public Enemy {
public:
    float       moveSpeed = 100.0f;  // píxeles por segundo
    GameObject* target = nullptr; // el jugador a seguir

    void setTarget(GameObject* t) { target = t; }
    void update(float dt) override;
};