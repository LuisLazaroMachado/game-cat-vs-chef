// ===========================================================================
//  Perro.cpp
// ===========================================================================

#include "Perro.h"
#include "../engine/GameObject.h"
#include "../engine/SpriteAnimator.h"
#include "../engine/SpriteRenderer.h"

void Perro::update(float dt) {
    if (state == EnemyState::DEAD) return;  // si está muerto no se mueve

    // resetear hurt después de un momento (animación de daño)
    if (state == EnemyState::HURT) {
        state = EnemyState::WALK; // vuelve a caminar después del hurt
    }

    // mover en la dirección actual
    float direction = facingLeft ? -1.0f : 1.0f;
    owner->transform->x += direction * moveSpeed * dt;

    // rebotar en los límites — igual que el profe con los bordes
    if (owner->transform->x <= leftLimit) {
        owner->transform->x = leftLimit;
        facingLeft = false; // cambia dirección: ahora va a la derecha
    }
    if (owner->transform->x >= rightLimit) {
        owner->transform->x = rightLimit;
        facingLeft = true;  // cambia dirección: ahora va a la izquierda
    }

    // actualizar flip del sprite según dirección
    SpriteRenderer* sr = owner->getComponent<SpriteRenderer>();
    if (sr) sr->flipX = facingLeft; // flip invertido porque el sprite mira a la derecha

    // animación
    SpriteAnimator* anim = owner->getComponent<SpriteAnimator>();
    if (!anim) return;

    switch (state) {
    case EnemyState::WALK: anim->play("dog_walk"); break;
    case EnemyState::HURT: anim->play("dog_hurt"); break;
    case EnemyState::DEAD: anim->play("dog_dead"); break;
    }
}