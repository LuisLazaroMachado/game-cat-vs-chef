// ===========================================================================
//  Box.cpp
// ===========================================================================

#include "Box.h"
#include "../engine/GameObject.h"
#include "../engine/SpriteAnimator.h"

void Box::hit() {
    if (state == BoxState::OPEN) return; // ya fue golpeada antes

    state = BoxState::OPEN;

    // cambiar al frame de abierta en el spritesheet
    SpriteAnimator* anim = owner->getComponent<SpriteAnimator>();
    if (anim) anim->play("box_open");
}

void Box::update(float dt) {
    // por ahora la caja no necesita lógica de update
    // el spawn del power up lo maneja el Level
}

SDL_FRect Box::getBounds() const {
    float w = 4167.0f * owner->transform->scaleX;
    float h = 4167.0f * owner->transform->scaleY;
    return {
        owner->transform->x,
        owner->transform->y,
        w,
        h
    };
}