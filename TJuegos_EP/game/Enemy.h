#pragma once

// ===========================================================================
//  Enemy.h
//  Clase base para todos los enemigos del juego.
//  El Perro y la Paloma heredan de esta clase.
// ===========================================================================

#include <SDL3/SDL.h>
#include "../engine/Component.h"
#include "../engine/GameObject.h"

// Estados posibles de cualquier enemigo
enum class EnemyState {
    WALK,  // caminando / moviéndose
    HURT,  // recibiendo daño
    DEAD   // muerto
};

class Enemy : public Component {
public:
    int        health = 1;      // puntos de vida (enemigos simples = 1)
    float      groundY = 500.0f; // posición Y del suelo
    EnemyState state = EnemyState::WALK;
    bool       facingLeft = true;  // dirección que mira

    // Recibe daño — si llega a 0 muere
    virtual void takeDamage(int amount) {
        if (state == EnemyState::DEAD) return;
        health -= amount;
        if (health <= 0) {
            health = 0;
            state = EnemyState::DEAD;
            owner->active = false; // desactiva el GameObject
        }
        else {
            state = EnemyState::HURT;
        }
    }

    // Cada enemigo implementa su propia lógica de movimiento
    void update(float dt) override {}
};