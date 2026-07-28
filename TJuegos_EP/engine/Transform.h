#pragma once

// ===========================================================================
//  Transform.h
//  Guarda la posición, escala y orientación de un GameObject en el mundo.
//  Es el componente más básico, todos los objetos lo tienen.
// ===========================================================================

#include "Component.h"

class Transform : public Component {
public:
    float x = 0.0f;   // posición horizontal en el mundo (píxeles)
    float y = 0.0f;   // posición vertical en el mundo (píxeles)
    float scaleX = 1.0f;   // escala horizontal (1.0 = tamaño original)
    float scaleY = 1.0f;   // escala vertical   (1.0 = tamaño original)
};