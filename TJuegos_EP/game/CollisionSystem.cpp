// ===========================================================================
//  CollisionSystem.cpp
// ===========================================================================
#include "CollisionSystem.h"
#include "../engine/AssetManager.h"

void CollisionSystem::checkAll(
    GameObject* player, Player* pc, SDL_FRect& playerBounds,
    SpriteRenderer* playerSprite, SpriteAnimator* playerAnim, SpriteAnimator* tigerAnimComp,
    LevelData& level, SoundManager& audio, SDL_Renderer* renderer
) {
    // ── Colisiones caja/powerup ─────────────────────────────────────────
    for (int i = 0; i < LevelData::NUM_BOXES; i++) {
        if (level.boxObjs[i]->active && level.boxComps[i]->state == BoxState::CLOSED) {
            SDL_FRect bBounds = level.boxComps[i]->getBounds();
            if (CollisionManager::checkAABB(playerBounds, bBounds)) {
                if (pc->velY < 0.0f) {
                    level.boxComps[i]->hit();
                    level.puObjs[i]->transform->x = level.boxObjs[i]->transform->x;
                    level.puObjs[i]->transform->y = level.boxObjs[i]->transform->y - 20.0f;
                    level.puComps[i]->velX = -150.0f;
                    level.puComps[i]->velY = -200.0f;
                    level.puComps[i]->groundY = 495.0f;
                    level.puComps[i]->onGround = false;
                    level.puObjs[i]->active = true;
                }
            }
        }
        if (level.puObjs[i]->active && !level.puComps[i]->collected) {
            SDL_FRect puBounds = level.puComps[i]->getBounds();
            if (CollisionManager::checkAABB(playerBounds, puBounds)) {
                level.puComps[i]->collected = true;
                level.puObjs[i]->active = false;
                if (level.puComps[i]->type == PowerUpType::FISH_ARMOR) {
                    pc->armorTimer = 7.0f;
                    pc->state = PlayerState::ARMORED;
                    playerSprite->armorEffect = true;
                    SDL_Log("Power up CORAZA activado! 7 segundos");
                }
                else if (level.puComps[i]->type == PowerUpType::FISH_TIGER) {
                    pc->tigerTimer = 10.0f;
                    pc->state = PlayerState::TIGER;
                    float oldBottom = player->transform->y + (playerSprite->height * player->transform->scaleY);
                    player->transform->scaleX = 3.0f;
                    player->transform->scaleY = 3.0f;
                    playerAnim->enabled = false;
                    playerSprite->texture = assets.load(renderer, "assets/spritesheet_big_cat.png");
                    playerSprite->width = 48;
                    playerSprite->height = 48;
                    float newHeight = playerSprite->height * player->transform->scaleY;
                    player->transform->y = (oldBottom - newHeight) + 5.0f;
                    pc->velY = 0.0f;
                    pc->onGround = true;
                    playerBounds.y = player->transform->y;
                    playerBounds.h = newHeight;
                    playerBounds.w = playerSprite->width * player->transform->scaleX;
                    tigerAnimComp->enabled = true;
                    tigerAnimComp->play("tiger_walk");
                    SDL_Log("Power up TIGRE activado y físicamente asegurado!");
                }
            }
        }
    }

    // ── Spawn de proyectil cuando el Boss ataca ─────────────────────────
    if (level.bossComp->shouldSpawnProjectile) {
        level.bossComp->shouldSpawnProjectile = false;
        for (int i = 0; i < LevelData::MAX_PROJECTILES; i++) {
            if (!level.projectileObjs[i]->active) {
                level.projectileObjs[i]->active = true;
                level.projectileObjs[i]->transform->x = level.bossObj->transform->x;
                level.projectileObjs[i]->transform->y = level.bossObj->transform->y;
                float dx = player->transform->x - level.bossObj->transform->x;
                level.projectileComps[i]->speedX = (dx < 0.0f) ? -200.0f : 200.0f;
                level.projectileComps[i]->speedY = -700.0f;
                level.projectileComps[i]->reflected = false;
                level.projectileComps[i]->active = true;
                break;
            }
        }
    }

    // ── Colisión proyectil → player / parry / boss ──────────────────────
    for (int i = 0; i < LevelData::MAX_PROJECTILES; i++) {
        if (!level.projectileObjs[i]->active) continue;
        SDL_FRect projBounds = level.projectileComps[i]->getBounds();
        if (CollisionManager::checkAABB(playerBounds, projBounds)) {
            if (!level.projectileComps[i]->reflected) {
                if (!pc->onGround) {
                    level.projectileComps[i]->reflected = true;
                    level.projectileComps[i]->speedX = -level.projectileComps[i]->speedX;
                    level.projectileComps[i]->speedY = -300.0f;
                    SDL_Log("Sarten reflejada!");
                }
                else {
                    if (!pc->isInvulnerable()) {
                        pc->health--;
                        pc->hurtCooldown = 1.0f;
                        audio.playSfx("dano");
                        SDL_Log("Player recibe daño de sarten! Vida: %d", pc->health);
                    }
                    level.projectileComps[i]->active = false;
                    level.projectileObjs[i]->active = false;
                }
            }
        }
        if (level.projectileComps[i]->reflected && level.projectileObjs[i]->active) {
            SDL_FRect bossBounds = level.bossComp->getBounds();
            if (CollisionManager::checkAABB(projBounds, bossBounds)) {
                level.bossComp->takeDamage(1);
                audio.playSfx("dano");
                level.projectileComps[i]->active = false;
                level.projectileObjs[i]->active = false;
                SDL_Log("Boss recibe daño!");
            }
        }
    }

    // ── Player pisa a cualquier perro ───────────────────────────────────
    for (int i = 0; i < LevelData::NUM_PERROS; i++) {
        if (!level.perros[i]->active) continue;
        if (level.perroComps[i]->state == EnemyState::DEAD) continue;
        SDL_FRect perroBounds = {
            level.perros[i]->transform->x,
            level.perros[i]->transform->y,
            556.0f * level.perros[i]->transform->scaleX,
            504.0f * level.perros[i]->transform->scaleY
        };
        if (CollisionManager::checkAABB(playerBounds, perroBounds)) {
            if (CollisionManager::isStompingEnemy(playerBounds, perroBounds, pc->velY)) {
                level.perroComps[i]->takeDamage(1);
                audio.playSfx("dano");
                pc->velY = -300.0f;
                SDL_Log("Perro pisado!");
            }
            else {
                if (!pc->isInvulnerable()) {
                    pc->health--;
                    pc->hurtCooldown = 1.0f;
                    audio.playSfx("dano");
                    SDL_Log("Player recibe daño del perro! Vida: %d", pc->health);
                    level.perroComps[i]->takeDamage(1);
                }
            }
        }
    }

    // ── Player toca a la Paloma ─────────────────────────────────────────
    if (level.palomaComp->state != EnemyState::DEAD) {
        SDL_FRect palomaBounds = {
            level.palomaObj->transform->x,
            level.palomaObj->transform->y,
            635.0f * level.palomaObj->transform->scaleX,
            490.0f * level.palomaObj->transform->scaleY
        };
        if (CollisionManager::checkAABB(playerBounds, palomaBounds)) {
            if (!pc->isInvulnerable()) {
                pc->health--;
                pc->hurtCooldown = 1.0f;
                audio.playSfx("dano");
                SDL_Log("Player recibe daño de la paloma! Vida: %d", pc->health);
                level.palomaComp->takeDamage(1);
            }
        }
    }

    // ── Tigre destruye enemigos al tocarlos ─────────────────────────────
    if (pc->state == PlayerState::TIGER) {
        for (int i = 0; i < LevelData::NUM_PERROS; i++) {
            if (!level.perros[i]->active) continue;
            if (level.perroComps[i]->state == EnemyState::DEAD) continue;
            SDL_FRect perroBounds = {
                level.perros[i]->transform->x,
                level.perros[i]->transform->y,
                556.0f * level.perros[i]->transform->scaleX,
                504.0f * level.perros[i]->transform->scaleY
            };
            if (CollisionManager::checkAABB(playerBounds, perroBounds)) {
                level.perroComps[i]->takeDamage(1);
                audio.playSfx("dano");
            }
        }
    }
}