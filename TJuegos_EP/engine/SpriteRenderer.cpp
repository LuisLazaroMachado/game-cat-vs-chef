// ===========================================================================
//  SpriteRenderer.cpp
// ===========================================================================

#include "SpriteRenderer.h"
#include "AssetManager.h"
#include "GameObject.h"

// Constructor: carga la textura usando el AssetManager global
SpriteRenderer::SpriteRenderer(SDL_Renderer* r, const std::string& path) {
    m_renderer = r;
    texture = assets.load(r, path); // usa la caché del profe
    if (texture) {
        // obtiene el tamaño real de la textura cargada
        float w, h;
        SDL_GetTextureSize(texture, &w, &h);
        width = (int)w;
        height = (int)h;
    }
}

// Guarda el recorte del spritesheet
void SpriteRenderer::setSourceRect(int x, int y, int w, int h) {
    m_srcRect = { (float)x, (float)y, (float)w, (float)h };
    m_hasSrcRect = true;
}

// Dibuja el sprite en pantalla usando la posición del Transform
void SpriteRenderer::render() {
    if (!texture || !m_renderer) return;

    // lee posición y escala del Transform del GameObject dueño
    float px = owner->transform->x;
    float py = owner->transform->y;
    float sx = owner->transform->scaleX;
    float sy = owner->transform->scaleY;

    // tamaño final en pantalla = tamaño del frame * escala
    float frameW = m_hasSrcRect ? m_srcRect.w : (float)width;
    float frameH = m_hasSrcRect ? m_srcRect.h : (float)height;

    SDL_FRect dst = { px, py, frameW * sx, frameH * sy };

    // voltear horizontalmente si flipX está activado
    SDL_FlipMode flip = flipX ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    // efecto arcoíris cuando la coraza está activa
    if (armorEffect) {
        armorColorTimer += 0.05f; // velocidad del ciclo de color
        // SDL_sinf genera valores entre -1 y 1, los convertimos a 0-255
        Uint8 r = (Uint8)((SDL_sinf(armorColorTimer * 1.0f) * 0.5f + 0.5f) * 255.0f);
        Uint8 g = (Uint8)((SDL_sinf(armorColorTimer * 1.5f) * 0.5f + 0.5f) * 255.0f);
        Uint8 b = (Uint8)((SDL_sinf(armorColorTimer * 2.0f) * 0.5f + 0.5f) * 255.0f);
        SDL_SetTextureColorMod(texture, r, g, b);
    }
    else {
        // restaurar color normal
        SDL_SetTextureColorMod(texture, 255, 255, 255);
    }

    SDL_RenderTextureRotated(m_renderer, texture,
        m_hasSrcRect ? &m_srcRect : nullptr,
        &dst, 0.0, nullptr, flip);
}