// ===========================================================================
//  SpriteAnimator.cpp
// ===========================================================================

#include "SpriteAnimator.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

SpriteAnimator::SpriteAnimator(int frameW, int frameH, int columns)
    : m_frameW(frameW), m_frameH(frameH), m_columns(columns) {}

void SpriteAnimator::addAnimation(const std::string& name,
    const std::vector<int>& frames,
    float fps) {
    m_animations[name] = { frames, fps }; // guarda la animación en el mapa
}

void SpriteAnimator::play(const std::string& name) {
    // si ya está corriendo esta animación, no reinicia
    if (m_currentName == name) return;
    m_currentName = name;
    m_currentFrame = 0;
    m_timer = 0.0f;
}

void SpriteAnimator::update(float dt) {
    if (!enabled) return;

    if (m_currentName.empty()) return;

    auto it = m_animations.find(m_currentName);
    if (it == m_animations.end()) return;

    const Animation& anim = it->second;

    // acumula tiempo y avanza el frame cuando corresponde
    m_timer += dt;
    float frameDuration = 1.0f / anim.fps; // segundos por frame
    if (m_timer >= frameDuration) {
        m_timer -= frameDuration;
        m_currentFrame = (m_currentFrame + 1) % (int)anim.frames.size(); // loop
    }

    // calcula el recorte y se lo pasa al SpriteRenderer del mismo GameObject
    SDL_FRect rect = getCurrentFrameRect();
    SpriteRenderer* sr = targetRenderer
        ? targetRenderer
        : owner->getComponent<SpriteRenderer>();
    if (sr) sr->setSourceRect((int)rect.x, (int)rect.y,
        (int)rect.w, (int)rect.h);
}

SDL_FRect SpriteAnimator::getCurrentFrameRect() const {
    if (m_currentName.empty()) return { 0,0,0,0 };

    auto it = m_animations.find(m_currentName);
    if (it == m_animations.end()) return { 0,0,0,0 };

    const Animation& anim = it->second;
    int cellIndex = anim.frames[m_currentFrame]; // índice global de la celda

    // convierte índice global → columna y fila dentro del spritesheet
    int col = cellIndex % m_columns;
    int row = cellIndex / m_columns;

    return {
        (float)(col * m_frameW), // x en el spritesheet
        (float)(row * m_frameH), // y en el spritesheet
        (float)m_frameW,
        (float)m_frameH
    };
}