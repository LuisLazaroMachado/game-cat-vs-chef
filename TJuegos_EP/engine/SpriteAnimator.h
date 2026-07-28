#pragma once

// ===========================================================================
//  SpriteAnimator.h
//  Componente que anima un spritesheet por frames.
//  AddAnimation(), play(), update().
// ===========================================================================

#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Component.h"

class SpriteRenderer; // ← declaración adelantada, antes de SpriteAnimator
// Una animación es una lista de índices de frame y su velocidad
struct Animation {
    std::vector<int> frames; // índices de celdas del spritesheet
    float            fps;    // cuántos frames por segundo
};

class SpriteAnimator : public Component {
public:
    SpriteRenderer* targetRenderer = nullptr; // ← dentro de la clase
    bool enabled = true;

    SpriteAnimator(int frameW, int frameH, int columns);
    void addAnimation(const std::string& name,
        const std::vector<int>& frames,
        float fps);
    void play(const std::string& name);
    void update(float dt) override;
    SDL_FRect getCurrentFrameRect() const;

private:
    int m_frameW = 0;
    int m_frameH = 0;
    int m_columns = 0;
    std::unordered_map<std::string, Animation> m_animations;
    std::string m_currentName = "";
    int         m_currentFrame = 0;
    float       m_timer = 0;
};