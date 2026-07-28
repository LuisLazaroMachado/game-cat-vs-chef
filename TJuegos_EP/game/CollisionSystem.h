#pragma once
// ===========================================================================
//  CollisionSystem.h
//  Centraliza todas las colisiones jugables: cajas, powerups, proyectiles,
//  perros, paloma y tigre.
// ===========================================================================
#include "../engine/GameObject.h"
#include "Player.h"
#include "LevelBuilder.h"
#include "SoundManager.h"
#include "CollisionManager.h"

class CollisionSystem {
public:
    // playerBounds se pasa por referencia porque el powerup TIGRE la modifica.
    static void checkAll(
        GameObject* player, Player* pc, SDL_FRect& playerBounds,
        SpriteRenderer* playerSprite, SpriteAnimator* playerAnim, SpriteAnimator* tigerAnimComp,
        LevelData& level, SoundManager& audio, SDL_Renderer* renderer
    );
};