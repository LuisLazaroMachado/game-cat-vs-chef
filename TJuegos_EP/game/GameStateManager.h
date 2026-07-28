#pragma once
// ===========================================================================
//  GameStateManager.h
//  Maneja las pantallas que NO son gameplay: Menu, Story, Info, GameOver, Victory.
// ===========================================================================
#include <SDL3/SDL.h>
#include "../engine/GameObject.h"
#include "Player.h"
#include "LevelBuilder.h"
#include "SoundManager.h"

enum class GameState { MENU, STORY, INFO, PLAYING, GAMEOVER, VICTORY };

class GameStateManager {
public:
    static void renderScreens(
        SDL_Renderer* renderer, const bool* keys, bool spacePressed, float dt,
        bool& running, GameState& gameState, int& menuSelection, float& deathTimer,
        GameObject* player, Player* pc, LevelData& level, int& currentLevel
    );
};