// ===========================================================================
//  PlayerBuilder.cpp
// ===========================================================================
#include "PlayerBuilder.h"

PlayerHandles PlayerBuilder::build(Scene& scene, SDL_Renderer* renderer) {
    PlayerHandles h;

    h.obj = scene.createGameObject("Player");
    h.obj->transform->x = 0.0f;
    h.obj->transform->y = 300.0f;
    h.obj->transform->scaleX = 0.2f;
    h.obj->transform->scaleY = 0.2f;

    h.sprite = h.obj->addComponent<SpriteRenderer>(renderer, "assets/spritesheet_cat.png");

    h.anim = h.obj->addComponent<SpriteAnimator>(556, 504, 4);
    h.anim->addAnimation("cat_hurt", { 0, 1, 2, 3 }, 8.0f);
    h.anim->addAnimation("cat_jump", { 4, 5, 6, 7 }, 8.0f);
    h.anim->addAnimation("cat_walk", { 8, 9, 10, 11 }, 10.0f);
    h.anim->addAnimation("cat_idle", { 12, 13, 14, 15 }, 6.0f);
    h.anim->addAnimation("cat_dead", { 16, 17, 18, 19 }, 8.0f);
    h.anim->addAnimation("cat_dash", { 20, 21, 22, 23 }, 10.0f);
    h.anim->play("cat_idle");

    h.comp = h.obj->addComponent<Player>();
    h.comp->groundY = 495.0f;

    h.sprite->width = 556;
    h.sprite->height = 504;

    h.tigerAnim = h.obj->addComponent<SpriteAnimator>(48, 48, 8);
    h.tigerAnim->addAnimation("tiger_walk", { 0,1,2,3,4,5,6,7 }, 10.0f);
    h.tigerAnim->enabled = false;
    h.comp->tigerAnim = h.tigerAnim;
    h.tigerAnim->targetRenderer = h.sprite;

    return h;
}