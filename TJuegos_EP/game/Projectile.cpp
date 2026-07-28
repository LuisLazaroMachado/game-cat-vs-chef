// ===========================================================================
//  Projectile.cpp
// ===========================================================================

#include "Projectile.h"
#include "../engine/GameObject.h"

void Projectile::update(float dt) {
    if (!active) return;

    // aplicar gravedad
    speedY += gravity * dt;

    // mover el proyectil
    owner->transform->x += speedX * dt;
    owner->transform->y += speedY * dt;

    // desactivar si sale de la pantalla
    if (owner->transform->x < -500.0f || owner->transform->x > 2000.0f ||
        owner->transform->y > 700.0f) {
        active = false;
        owner->active = false;
    }

    // desaparece al tocar el suelo
    if (owner->transform->y >= 500.0f) {
        active = false;
        owner->active = false;
    }

}

SDL_FRect Projectile::getBounds() const {
    float w = 48.0f * owner->transform->scaleX;
    float h = 48.0f * owner->transform->scaleY;
    return {
        owner->transform->x,
        owner->transform->y,
        w, h
    };
}