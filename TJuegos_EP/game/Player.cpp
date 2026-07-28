// ===========================================================================
//  Player.cpp
// ===========================================================================

#include "Player.h"
#include "../engine/GameObject.h"
#include "../engine/SpriteAnimator.h"
#include "../engine/SpriteRenderer.h"
#include "../engine/AssetManager.h"

void Player::update(float dt) {
    m_lastDt = dt; // guardar dt para updateAnimation
    const bool* keys = SDL_GetKeyboardState(nullptr);
    handleDash(dt, keys);
    handleMovement(dt, keys);
    handleJump(dt, keys);
    handlePowerUpTimers(dt);
    updateAnimation();
}

// ── Movimiento horizontal ─────────────────────────────────────────────────
void Player::handleMovement(float dt, const bool* keys) {
    // durante el dash no se puede mover horizontalmente
    if (state == PlayerState::DASH) return;

    float speed = (state == PlayerState::TIGER) ? tigerSpeed : moveSpeed;
    float moveX = 0.0f;

    if (keys[SDL_SCANCODE_LEFT])  moveX = -1.0f;
    if (keys[SDL_SCANCODE_RIGHT]) moveX = 1.0f;

    owner->transform->x += moveX * speed * dt;

    // actualizar dirección que mira
    if (moveX < 0.0f) facingLeft = true;
    else if (moveX > 0.0f) facingLeft = false;

    // actualizar flip del sprite
    SpriteRenderer* sr = owner->getComponent<SpriteRenderer>();
    if (sr) sr->flipX = facingLeft;

    // cambiar estado solo si no está en el aire
    if (onGround && state != PlayerState::ARMORED &&
        state != PlayerState::TIGER) {
        state = (moveX != 0.0f) ? PlayerState::WALK : PlayerState::IDLE;
    }
}

// ── Salto variable ────────────────────────────────────────────────────────
void Player::handleJump(float dt, const bool* keys) {
    if (state == PlayerState::DASH) return;

    bool jumpDown = keys[SDL_SCANCODE_SPACE]; // tecla de salto

    // detecta si se acaba de presionar (no estaba antes, ahora sí)
    m_jumpPressed = jumpDown && !m_jumpWasDown;
    m_jumpHeld = jumpDown;
    m_jumpWasDown = jumpDown;

    // saltar si está en el suelo y se presionó el salto
    if (m_jumpPressed && onGround) {
        velY = jumpForce; // fuerza completa
        onGround = false;
        m_jumpCount = 1;
        if (state != PlayerState::ARMORED && state != PlayerState::TIGER)
            state = PlayerState::JUMP;
    }

    // salto variable: si suelta el botón antes, corta el salto
    if (!m_jumpHeld && velY < shortJumpForce) {
        velY = shortJumpForce; // limita la velocidad máxima hacia arriba
    }

    // doble salto → parry (segundo salto en el aire)
    if (m_jumpPressed && !onGround && m_jumpCount == 1) {
        m_jumpCount = 2;
        parryActivated = true; // señal para que main.cpp detecte el parry
        SDL_Log("Parry activado!");
    }

    // aplicar gravedad siempre que esté en el aire
    if (!onGround) {
        velY += gravity * dt; // acelera hacia abajo
        owner->transform->y += velY * dt;
    }

    // ¡EL BLOQUE DEL SUELO FIJO SE ELIMINÓ DE AQUÍ!
    // Ahora las colisiones dependen 100% de tus plataformas reales.
}

// ── Dash ──────────────────────────────────────────────────────────────────
void Player::handleDash(float dt, const bool* keys) {
    // activar dash con tecla Shift izquierdo
    if (keys[SDL_SCANCODE_LSHIFT] && state != PlayerState::DASH &&
        state != PlayerState::TIGER) { // el tigre no puede hacer dash
        state = PlayerState::DASH;
        m_dashTimer = dashDuration;
        // dash en la dirección que mira el personaje
        m_dashDirection = facingLeft ? -1.0f : 1.0f;
    }

    // ejecutar el dash mientras dure el timer
    if (state == PlayerState::DASH) {
        m_dashTimer -= dt;
        owner->transform->x += m_dashDirection * dashSpeed * dt;

        // cuando termina el dash vuelve al estado anterior
        if (m_dashTimer <= 0.0f) {
            m_dashTimer = 0.0f;
            state = onGround ? PlayerState::IDLE : PlayerState::JUMP;
        }
    }
}

// ── Timers de power ups ───────────────────────────────────────────────────
void Player::handlePowerUpTimers(float dt) {
    // ── Cooldown de daño (i-frames) ─────────────────────────────────────
    if (hurtCooldown > 0.0f) {
        hurtCooldown -= dt;
        if (hurtCooldown < 0.0f) hurtCooldown = 0.0f;
    }
    // ── Coraza ────────────────────────────────────────────────────────
    if (armorTimer > 0.0f) {
        armorTimer -= dt;
        if (armorTimer <= 0.0f) {
            armorTimer = 0.0f;
            SpriteRenderer* sr = owner->getComponent<SpriteRenderer>();
            if (sr) sr->armorEffect = false;
            if (state == PlayerState::ARMORED)
                state = PlayerState::IDLE;
        }
    }
    // ── Tigre ─────────────────────────────────────────────────────────
    if (tigerTimer > 0.0f) {
        tigerTimer -= dt;
        if (tigerTimer <= 0.0f) {
            tigerTimer = 0.0f;
            // termina el tigre sin importar el estado actual
            owner->transform->scaleX = 0.2f;
            owner->transform->scaleY = 0.2f;
            SpriteAnimator* catAnim = owner->getComponent<SpriteAnimator>();
            if (catAnim) catAnim->enabled = true;
            if (tigerAnim) tigerAnim->enabled = false;
            SpriteRenderer* sr = owner->getComponent<SpriteRenderer>();
            if (sr) {
                float tigerBottom = owner->transform->y + (sr->height * owner->transform->scaleY);
                sr->texture = assets.load(sr->m_renderer, "assets/spritesheet_cat.png");
                sr->width = 556;
                sr->height = 504;
                sr->setSourceRect(0, 0, 556, 504);
                float catHeight = 504.0f * 0.2f;
                owner->transform->y = tigerBottom - catHeight;
            }
            if (state == PlayerState::TIGER)
                state = PlayerState::IDLE;
            SDL_Log("Power up TIGRE terminado");
        }
    }
}

// ── Animación según estado ────────────────────────────────────────────────
void Player::updateAnimation() {
    SpriteAnimator* anim = owner->getComponent<SpriteAnimator>();
    if (!anim) return;

    if (state == PlayerState::TIGER) {
        anim->enabled = false; // desactiva el animador del gato
        // scene.update() ya actualiza tigerAnim automáticamente
        return;
    }

    anim->enabled = true;
    switch (state) {
    case PlayerState::IDLE:    anim->play("cat_idle");  break;
    case PlayerState::WALK:    anim->play("cat_walk");  break;
    case PlayerState::JUMP:    anim->play("cat_jump");  break;
    case PlayerState::DASH:    anim->play("cat_dash");  break;
    case PlayerState::ARMORED: anim->play("cat_idle");  break;
    default: break;
    }
}