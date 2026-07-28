#pragma once

// ===========================================================================
//  Boss.h
//  Boss final: Chifero que lanza wantanes y sartenes al jugador.
// ===========================================================================

#include <SDL3/SDL.h>
#include "../engine/Component.h"

// Estados del boss
enum class BossState {
    IDLE,      // quieto
    WALK,      // caminando
    ATTACK,    // lanzando proyectil
    HURT,      // recibiendo daño
    DEAD       // muerto
};

class GameObject;

class Boss : public Component {
public:
    // ── Configuración ────────────────────────────────────────────────────
    float moveSpeed = 80.0f;   // más lento que los enemigos normales
    float groundY = 500.0f;  // posición Y del suelo
    int   health = 2;      // el boss tiene 10 puntos de vida
    int   maxHealth = 10;      // para calcular porcentaje de vida en HUD

    // ── Estado ───────────────────────────────────────────────────────────
    BossState   state = BossState::WALK;
    bool        facingLeft = true;
    GameObject* target = nullptr; // el jugador

    // ── Ataque ───────────────────────────────────────────────────────────
    float attackTimer = 0.0f;  // timer entre ataques
    float attackCooldown = 1.0f;  // segundos entre cada lanzamiento
    bool  shouldSpawnProjectile = false; // señal para spawnear proyectil

    void setTarget(GameObject* t) { target = t; }
    void takeDamage(int amount);
    void update(float dt) override;

    // Devuelve el rectángulo de colisión del boss
    SDL_FRect getBounds() const;

private:
    float m_hurtTimer = 0.0f; // tiempo que dura la animación de daño
    void updateAnimation();
};