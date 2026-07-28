// ===========================================================================
//  Camera.cpp
// ===========================================================================

#include "Camera.h"
#include "GameObject.h"

SDL_FRect Camera::worldToScreen(float wx, float wy, float ww, float wh) const {
    // posición de la cámara = posición del GameObject que la contiene
    float camX = owner->transform->x;
    float camY = owner->transform->y;

    // igual a la fórmula del profe:
    // 1. resta posición de la cámara (qué parte del mundo vemos)
    // 2. multiplica por zoom
    // 3. centra en pantalla
    float sx = (wx - camX) * zoom + screenW * 0.5f;
    float sy = (wy - camY) * zoom + screenH * 0.5f;

    return { sx, sy, ww * zoom, wh * zoom };
}