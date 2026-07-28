#pragma once

// ===========================================================================
//  Projectile.h
//  Proyectil que lanza el Boss (wantán o sartén).
//  El jugador puede reflejarlo con el parry (doble salto).
// ===========================================================================

#include <SDL3/SDL.h>
#include "../engine/Component.h"

enum class ProjectileType {
    WANTAN, // wantán: proyectil normal
    SARTAN  // sartén: proyectil más rápido
};

class Projectile : public Component {
public:
    ProjectileType type = ProjectileType::WANTAN;
    float          speedX = -200.0f; // velocidad horizontal
    float          speedY = -100.0f; // velocidad vertical inicial
    float          gravity = 400.0f;  // gravedad del proyectil
    bool           reflected = false;   // si fue reflejado por parry
    bool           active = true;    // si está activo

    void update(float dt) override;
    SDL_FRect getBounds() const;
};