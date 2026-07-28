// ===========================================================================
//  PowerUp.cpp
// ===========================================================================

#include "PowerUp.h"
#include "../engine/GameObject.h"

void PowerUp::update(float dt) {
    if (collected) return;
    if (onGround) return; // ya tocó el suelo, se queda quieto

    // física simple: gravedad
    velY += 800.0f * dt; // gravedad igual al player
    owner->transform->x += velX * dt;
    owner->transform->y += velY * dt;

    // colisión con el suelo
    if (owner->transform->y >= groundY) {
        owner->transform->y = groundY;
        velX = 0.0f;
        velY = 0.0f;
        onGround = true; // se queda quieto en el suelo
    }
}

SDL_FRect PowerUp::getBounds() const {
    float w = 1545.0f * owner->transform->scaleX;
    float h = 980.0f * owner->transform->scaleY;
    return {
        owner->transform->x,
        owner->transform->y,
        w,
        h
    };
}