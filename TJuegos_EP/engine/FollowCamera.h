#pragma once

// ===========================================================================
//  FollowCamera.h
//  Componente que hace que la cámara siga a un GameObject objetivo
//  con zona muerta y suavizado.
// ===========================================================================

#include "Component.h"

class GameObject;

class FollowCamera : public Component {
public:
    GameObject* target = nullptr; // el GameObject a seguir (player)
    float       deadZoneWidth = 200.0f; // zona muerta horizontal en píxeles
    float       deadZoneHeight = 150.0f; // zona muerta vertical en píxeles
    float       smoothSpeed = 5.0f;   // qué tan rápido sigue al objetivo

    void setTarget(GameObject* t) { target = t; }

    // Mueve suavemente la cámara hacia el objetivo cada frame
    void update(float dt) override;
};