#pragma once

// ===========================================================================
//  SpriteRenderer.h
//  Componente que dibuja una textura en pantalla.
//  (SpriteRenderer.h del engine de clase).
// ===========================================================================

#include <SDL3/SDL.h>
#include <string>
#include "Component.h"

class SpriteRenderer : public Component {
public:
    SDL_Texture* texture = nullptr;  // textura a dibujar
    bool         flipX = false;    // voltear horizontalmente (mirar izquierda/derecha)
    int          width = 0;        // ancho del sprite en píxeles
    int          height = 0;        // alto del sprite en píxeles
    // efecto de coraza: cicla colores como Mario con estrella
    bool  armorEffect = false; // activar/desactivar efecto
    float armorColorTimer = 0.0f; // timer interno del ciclo de color

    SDL_Renderer* m_renderer = nullptr; // referencia al renderer de SD

    // Constructor: recibe el renderer y la ruta del PNG
    SpriteRenderer(SDL_Renderer* r, const std::string& path);

    // Establece un recorte específico del spritesheet (para frames estáticos)
    void setSourceRect(int x, int y, int w, int h);

    // Dibuja el sprite aplicando la posición y escala del Transform del dueño
    void render() override;

private:
    SDL_FRect m_srcRect = { 0,0,0,0 }; // recorte del spritesheet
    bool      m_hasSrcRect = false;     // si es false dibuja la textura completa
};