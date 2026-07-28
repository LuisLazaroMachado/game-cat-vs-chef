#pragma once

// ===========================================================================
//  Camera.h
//  Componente de cámara 2D ortográfica.
//  Convierte posiciones del mundo a posiciones en pantalla.
//  Camera.h del engine de clase.
// ===========================================================================

#include <SDL3/SDL.h>
#include "Component.h"

class Camera : public Component {
public:
    float zoom = 1.0f;  // 1.0 = normal, 2.0 = acercado, 0.5 = alejado
    int   screenW = 1280;  // ancho de la ventana en píxeles
    int   screenH = 720;   // alto de la ventana en píxeles

    // Convierte una posición del MUNDO a una posición en PANTALLA
    // Igual a la función worldToScreen del profe
    SDL_FRect worldToScreen(float wx, float wy, float ww, float wh) const;

    // Referencia al renderer, la Scene la asigna al crear la cámara
    SDL_Renderer* m_renderer = nullptr;
};