// ===========================================================================
//  LevelBuilder.cpp
// ===========================================================================
#include "LevelBuilder.h"

void LevelBuilder::build(Scene& scene, SDL_Renderer* renderer, GameObject* player, LevelData& level) {
    // ── Perros ──────────────────────────────────────────────────────────
    PerroSpawnData perroSpawns[] = {
        { 1, 300.0f, 394.2f, 100.0f, 500.0f },
        { 1, 850.0f, 394.2f, 700.0f, 1150.0f },
        { 1, 550.0f, 220.0f, 320.0f, 950.0f },
        { 3, 200.0f, 394.2f, 50.0f,  450.0f },
        { 3, 900.0f, 394.2f, 700.0f, 1150.0f },
    };
    for (int i = 0; i < LevelData::NUM_PERROS; i++) {
        level.perroSpawns[i] = perroSpawns[i];
        GameObject* p = scene.createGameObject("Perro_" + std::to_string(i));
        p->transform->x = perroSpawns[i].x;
        p->transform->y = perroSpawns[i].y;
        p->transform->scaleX = 0.2f;
        p->transform->scaleY = 0.2f;
        p->active = (perroSpawns[i].level == 1);
        p->addComponent<SpriteRenderer>(renderer, "assets/spritesheet_dog.png");
        SpriteAnimator* pAnim = p->addComponent<SpriteAnimator>(556, 504, 4);
        pAnim->addAnimation("dog_hurt", { 0,1,2,3 }, 8.0f);
        pAnim->addAnimation("dog_walk", { 8,9,10,11 }, 10.0f);
        pAnim->addAnimation("dog_dead", { 16,17,18,19 }, 8.0f);
        pAnim->play("dog_walk");
        Perro* pc2 = p->addComponent<Perro>();
        pc2->leftLimit = perroSpawns[i].leftLimit;
        pc2->rightLimit = perroSpawns[i].rightLimit;
        level.perros[i] = p;
        level.perroComps[i] = pc2;
    }

    // ── Paloma ──────────────────────────────────────────────────────────
    GameObject* palomaObj = scene.createGameObject("Paloma_1");
    palomaObj->transform->x = 0.0f;
    palomaObj->transform->y = 0.0f;
    palomaObj->transform->scaleX = 0.2f;
    palomaObj->transform->scaleY = 0.2f;
    palomaObj->active = false;
    palomaObj->addComponent<SpriteRenderer>(renderer, "assets/spritesheet_pigeon.PNG");
    SpriteAnimator* palomaAnim = palomaObj->addComponent<SpriteAnimator>(635, 490, 3);
    palomaAnim->addAnimation("pigeon_walk", { 0,1,2 }, 8.0f);
    palomaAnim->play("pigeon_walk");
    Paloma* palomaComp = palomaObj->addComponent<Paloma>();
    palomaComp->setTarget(player);
    level.palomaObj = palomaObj;
    level.palomaComp = palomaComp;

    // ── Boss ────────────────────────────────────────────────────────────
    GameObject* bossObj = scene.createGameObject("Boss");
    bossObj->transform->x = 900.0f;
    bossObj->transform->y = 383.0f;
    bossObj->transform->scaleX = 2.0f;
    bossObj->transform->scaleY = 2.0f;
    bossObj->active = false;
    bossObj->addComponent<SpriteRenderer>(renderer, "assets/spritesheet_ChefSheet.png");
    SpriteAnimator* bossAnim = bossObj->addComponent<SpriteAnimator>(48, 56, 18);
    bossAnim->addAnimation("chef_walk", { 0,1,2,3,4,5,6,7 }, 8.0f);
    bossAnim->addAnimation("chef_attack", { 13,14,15,16,17 }, 10.0f);
    bossAnim->play("chef_walk");
    Boss* bossComp = bossObj->addComponent<Boss>();
    bossComp->groundY = 495.0f;
    bossComp->setTarget(player);
    bossComp->health = 1;
    bossComp->maxHealth = 1;
    level.bossObj = bossObj;
    level.bossComp = bossComp;

    // ── Proyectiles ─────────────────────────────────────────────────────
    for (int i = 0; i < LevelData::MAX_PROJECTILES; i++) {
        GameObject* proj = scene.createGameObject("Proyectil");
        proj->transform->scaleX = 0.1f;
        proj->transform->scaleY = 0.1f;
        proj->active = false;
        proj->addComponent<SpriteRenderer>(renderer, "assets/spritesheet_sarten.png");
        level.projectileObjs[i] = proj;
        level.projectileComps[i] = proj->addComponent<Projectile>();
    }

    // ── Cajas ───────────────────────────────────────────────────────────
    BoxSpawnData boxSpawns[] = {
        { 1, 100.0f, 200.0f, PowerUpType::FISH_ARMOR },
        { 1, 300.0f, 000.0f, PowerUpType::FISH_TIGER },
        { 2, 1100.0f, 000.0f, PowerUpType::FISH_ARMOR },
        { 2, 300.0f, 000.0f, PowerUpType::FISH_TIGER },
        { 3, 100.0f, 200.0f, PowerUpType::FISH_ARMOR },
    };
    for (int i = 0; i < LevelData::NUM_BOXES; i++) {
        level.boxSpawns[i] = boxSpawns[i];
        GameObject* b = scene.createGameObject("Caja_" + std::to_string(i));
        b->transform->x = boxSpawns[i].x;
        b->transform->y = boxSpawns[i].y;
        b->transform->scaleX = 0.02f;
        b->transform->scaleY = 0.02f;
        b->active = (boxSpawns[i].level == 1);
        b->addComponent<SpriteRenderer>(renderer, "assets/spritesheet_caja.png");
        SpriteAnimator* bAnim = b->addComponent<SpriteAnimator>(4167, 4167, 2);
        bAnim->addAnimation("box_closed", { 0 }, 1.0f);
        bAnim->addAnimation("box_open", { 1 }, 1.0f);
        bAnim->play("box_closed");
        level.boxObjs[i] = b;
        level.boxComps[i] = b->addComponent<Box>();

        GameObject* pu = scene.createGameObject("PU_" + std::to_string(i));
        pu->transform->scaleX = 0.05f;
        pu->transform->scaleY = 0.05f;
        pu->active = false;
        pu->addComponent<SpriteRenderer>(renderer, "assets/fish.png");
        level.puObjs[i] = pu;
        level.puComps[i] = pu->addComponent<PowerUp>();
        level.puComps[i]->type = boxSpawns[i].powerUpType;
    }

    // ── Plataformas ─────────────────────────────────────────────────────
    level.platforms[0] = { 0.0f, 495.0f, 1280.0f, 40.0f };
    level.platforms[1] = { 260.0f, 320.0f, 760.0f, 20.0f };
}

void LevelBuilder::advanceLevel(LevelData& level, int& currentLevel) {
    currentLevel++;

    if (currentLevel == 2) {
        for (int i = 0; i < LevelData::NUM_PERROS; i++) {
            if (level.perroSpawns[i].level <= 2) {
                level.perros[i]->active = true;
                level.perroComps[i]->state = EnemyState::WALK;
                level.perroComps[i]->health = 1;
            }
        }
        for (int i = 0; i < LevelData::NUM_BOXES; i++)
            level.boxObjs[i]->active = (level.boxSpawns[i].level == 2);
        level.palomaObj->active = true;
        level.palomaComp->state = EnemyState::WALK;
        level.palomaComp->health = 1;
    }
    else if (currentLevel == 3) {
        for (int i = 0; i < LevelData::NUM_PERROS; i++) {
            level.perros[i]->active = true;
            level.perroComps[i]->state = EnemyState::WALK;
            level.perroComps[i]->health = 1;
        }
        for (int i = 0; i < LevelData::NUM_BOXES; i++)
            level.boxObjs[i]->active = (level.boxSpawns[i].level == 3);
        level.palomaObj->active = true;
        level.palomaComp->state = EnemyState::WALK;
        level.palomaComp->health = 1;
        level.bossObj->active = true;
    }
    SDL_Log("¡Avanzaste al Nivel %d!", currentLevel);
}