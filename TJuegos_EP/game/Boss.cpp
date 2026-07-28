// ===========================================================================
//  Boss.cpp
// ===========================================================================

#include "Boss.h"
#include "../engine/GameObject.h"
#include "../engine/SpriteAnimator.h"
#include "../engine/SpriteRenderer.h"

void Boss::takeDamage(int amount) {
    if (state == BossState::DEAD) return;
    health -= amount;
    SDL_Log("Boss recibe daño! Vida restante: %d", health);
    if (health <= 0) {
        health = 0;
        state = BossState::DEAD;
        owner->active = false;
        SDL_Log("Boss derrotado!");
    }
    else {
        state = BossState::HURT;
        m_hurtTimer = 1.0f; // dura 1.0 segundos en hurt
    }
}

void Boss::update(float dt) {
    if (state == BossState::DEAD) return;

    // ── Timer de hurt ─────────────────────────────────────────────────
    if (state == BossState::HURT) {
        m_hurtTimer -= dt;
        if (m_hurtTimer <= 0.0f) {
            state = BossState::WALK;
        }
        updateAnimation();
        return;
    }

    // ── Movimiento hacia el jugador ───────────────────────────────────
    if (target) {
        float dx = target->transform->x - owner->transform->x;
        facingLeft = (dx < 0.0f);

        // se mueve hacia el jugador
        float direction = facingLeft ? -1.0f : 1.0f;
        owner->transform->x += direction * moveSpeed * dt;

        // flip del sprite
        SpriteRenderer* sr = owner->getComponent<SpriteRenderer>();
        if (sr) sr->flipX = !facingLeft;
    }

    // colisión simple con el suelo
    if (owner->transform->y >= groundY) {
        owner->transform->y = groundY;
    }

    // ── Timer de ataque ───────────────────────────────────────────────
    attackTimer += dt;
    if (attackTimer >= attackCooldown) {
        attackTimer = 0.0f;
        state = BossState::ATTACK;
        shouldSpawnProjectile = true; // el Level lo detecta y spawna el proyectil
        SDL_Log("Boss lanza proyectil!");
    }

    // vuelve a caminar después de atacar
    if (state == BossState::ATTACK) {
        state = BossState::WALK;
    }

    updateAnimation();
}

SDL_FRect Boss::getBounds() const {
    float w = 48.0f * owner->transform->scaleX;
    float h = 56.0f * owner->transform->scaleY;
    return {
        owner->transform->x,
        owner->transform->y,
        w, h
    };
}

void Boss::updateAnimation() {
    SpriteAnimator* anim = owner->getComponent<SpriteAnimator>();
    if (!anim) return;

    switch (state) {
    case BossState::WALK:   anim->play("chef_walk");   break;
    case BossState::ATTACK: anim->play("chef_attack"); break;
    case BossState::HURT:   anim->play("chef_walk");   break;
    case BossState::DEAD:   anim->play("chef_walk");   break;
    default: break;
    }
}