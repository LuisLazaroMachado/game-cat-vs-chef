#pragma once

// ===========================================================================
//  Player.h
//  Componente principal del gatito jugador.
//  Maneja: movimiento, salto variable, dash y estados de animación.
// ===========================================================================

#include <SDL3/SDL.h>
#include <string>
#include "../engine/Component.h"
#include "../engine/SpriteAnimator.h"

// Estados posibles del jugador
enum class PlayerState {
    IDLE,    // quieto
    WALK,    // caminando
    JUMP,    // en el aire
    DASH,    // haciendo dash
    ARMORED, // con coraza (power up pescado)
    TIGER    // transformado en tigre (power up evolución)
};

class Player : public Component {
public:

    SpriteAnimator* tigerAnim = nullptr; // animador separado para el tigre

    // ── Configuración ────────────────────────────────────────────────────
    float moveSpeed = 250.0f;  // píxeles por segundo caminando
    float tigerSpeed = 150.0f;  // píxeles por segundo en modo tigre (más lento)
    float jumpForce = -800.0f; // fuerza inicial del salto (negativo = hacia arriba)
    float shortJumpForce = -300.0f; // fuerza del saltito corto
    float gravity = 1200.0f; // gravedad en píxeles por segundo al cuadrado
    float dashSpeed = 600.0f;  // píxeles por segundo durante el dash
    float dashDuration = 0.15f;   // duración del dash en segundos
    float groundY = 500.0f;  // posición Y del suelo (temporal hasta tener tiles)
    float m_lastDt = 0.016f; // guarda el último dt para usarlo en updateAnimation
    bool isInvulnerable() const { return armorTimer > 0.0f || tigerTimer > 0.0f || hurtCooldown > 0.0f; }

    // ── Estado actual ────────────────────────────────────────────────────
    PlayerState state = PlayerState::IDLE;
    float       velY = 0.0f;   // velocidad vertical actual
    bool        onGround = false;  // si está tocando el suelo
    int         health = 10;      // puntos de vida del jugador
    bool        facingLeft = false;  // dirección que mira

    // ── Power up timers ──────────────────────────────────────────────────
    float armorTimer = 0.0f;  // tiempo restante de coraza (7 segundos)
    float tigerTimer = 0.0f;  // tiempo restante de tigre (10 segundos)

    // ── Invulnerabilidad tras recibir daño ─────────────────────────────
    float hurtCooldown = 0.0f; // mientras sea > 0, no puede recibir más daño

    // ── Lógica por frame ─────────────────────────────────────────────────
    void update(float dt) override;
    bool parryActivated = false; // señal de que se activó el parry este frame

private:
    // ── Variables internas ───────────────────────────────────────────────
    float m_dashTimer = 0.0f;   // tiempo restante del dash actual
    float m_dashDirection = 1.0f;   // dirección del dash: 1=derecha, -1=izquierda
    bool  m_jumpHeld = false;  // si el jugador mantiene presionado el salto
    bool  m_jumpPressed = false;  // si el jugador acaba de presionar el salto
    bool  m_jumpWasDown = false;  // estado del salto en el frame anterior
    int   m_jumpCount = 0;      // contador de saltos (para doble salto/parry)

    // ── Métodos privados ─────────────────────────────────────────────────
    void handleMovement(float dt, const bool* keys);  // movimiento horizontal
    void handleJump(float dt, const bool* keys);      // salto variable
    void handleDash(float dt, const bool* keys);      // dash
    void handlePowerUpTimers(float dt);               // reduce timers de power ups
    void updateAnimation();                           // cambia la animación según estado
};