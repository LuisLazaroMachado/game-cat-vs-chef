#pragma once
// ===========================================================================
//  LevelBuilder.h
//  Construye el nivel (perros, cajas, paloma, boss, proyectiles)
//  usando las abstracciones del motor (Scene, GameObject, Component).
// ===========================================================================
#include "../engine/Scene.h"
#include "../engine/GameObject.h"
#include "../engine/SpriteRenderer.h"
#include "../engine/SpriteAnimator.h"
#include "Perro.h"
#include "Paloma.h"
#include "Box.h"
#include "PowerUp.h"
#include "Boss.h"
#include "Projectile.h"

struct PerroSpawnData {
    int level;
    float x, y;
    float leftLimit, rightLimit;
};

struct BoxSpawnData {
    int level;
    float x, y;
    PowerUpType powerUpType;
};

struct Platform {
    float x, y, w, h;
};

struct LevelData {
    static const int NUM_PERROS = 5;
    static const int NUM_BOXES = 5;
    static const int MAX_PROJECTILES = 5;
    static const int NUM_PLATFORMS = 2;

    Platform platforms[NUM_PLATFORMS];

    PerroSpawnData perroSpawns[NUM_PERROS];
    BoxSpawnData   boxSpawns[NUM_BOXES];

    GameObject* perros[NUM_PERROS];
    Perro* perroComps[NUM_PERROS];

    GameObject* boxObjs[NUM_BOXES];
    Box* boxComps[NUM_BOXES];
    GameObject* puObjs[NUM_BOXES];
    PowerUp* puComps[NUM_BOXES];

    GameObject* projectileObjs[MAX_PROJECTILES];
    Projectile* projectileComps[MAX_PROJECTILES];

    GameObject* palomaObj = nullptr;
    Paloma* palomaComp = nullptr;

    GameObject* bossObj = nullptr;
    Boss* bossComp = nullptr;
};

class LevelBuilder {
public:
    static void build(Scene& scene, SDL_Renderer* renderer, GameObject* player, LevelData& level);
    static void advanceLevel(LevelData& level, int& currentLevel);
};