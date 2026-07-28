#pragma once
// ===========================================================================
//  PlayerBuilder.h
//  Construye el GameObject del jugador con sus componentes.
// ===========================================================================
#include "../engine/Scene.h"
#include "../engine/GameObject.h"
#include "../engine/SpriteRenderer.h"
#include "../engine/SpriteAnimator.h"
#include "Player.h"

struct PlayerHandles {
    GameObject* obj = nullptr;
    SpriteRenderer* sprite = nullptr;
    SpriteAnimator* anim = nullptr;
    SpriteAnimator* tigerAnim = nullptr;
    Player* comp = nullptr;
};

class PlayerBuilder {
public:
    static PlayerHandles build(Scene& scene, SDL_Renderer* renderer);
};