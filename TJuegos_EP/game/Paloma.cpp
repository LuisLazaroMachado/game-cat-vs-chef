// ===========================================================================
//  Paloma.cpp
// ===========================================================================

#include "Paloma.h"
#include "../engine/GameObject.h"
#include "../engine/SpriteAnimator.h"
#include "../engine/SpriteRenderer.h"


void Paloma::update(float dt) {
    SDL_Log("Paloma x=%.1f y=%.1f state=%d dt=%.4f", owner->transform->x, owner->transform->y, (int)state, dt);

    if (state == EnemyState::DEAD) return;
    if (!target) return; // si no tiene objetivo no hace nada

    // resetear hurt
    if (state == EnemyState::HURT) {
        state = EnemyState::WALK;
    }

    // calcular dirección hacia el jugador
    float dx = target->transform->x - owner->transform->x;
    float dy = target->transform->y - owner->transform->y;

    // calcular distancia para normalizar el vector dirección
    float distance = SDL_sqrtf(dx * dx + dy * dy);

    // solo se mueve si no está encima del jugador
    if (distance > 5.0f) {
        // normalizar: divide por la distancia para obtener dirección unitaria
        float nx = dx / distance; // componente X normalizada (-1 a 1)
        float ny = dy / distance; // componente Y normalizada (-1 a 1)

        owner->transform->x += nx * moveSpeed * dt;
        owner->transform->y += ny * moveSpeed * dt;

        // flip según hacia dónde va
        facingLeft = (dx < 0.0f);
        SpriteRenderer* sr = owner->getComponent<SpriteRenderer>();
        if (sr) sr->flipX =!facingLeft;
    }

    // animación
    SpriteAnimator* anim = owner->getComponent<SpriteAnimator>();
    if (!anim) return;

    switch (state) {
    case EnemyState::WALK: anim->play("pigeon_walk"); break;
    case EnemyState::HURT: anim->play("pigeon_walk"); break;
    case EnemyState::DEAD: anim->play("pigeon_walk"); break;
    }
}