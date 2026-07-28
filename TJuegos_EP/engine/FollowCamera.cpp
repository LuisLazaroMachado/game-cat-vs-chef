// ===========================================================================
//  FollowCamera.cpp
// ===========================================================================

#include "FollowCamera.h"
#include "GameObject.h"

void FollowCamera::update(float dt) {
    if (!target) return;

    // posición actual de la cámara (Transform del GameObject que tiene este componente)
    float camX = owner->transform->x;
    float camY = owner->transform->y;

    // posición del objetivo (player)
    float targetX = target->transform->x;
    float targetY = target->transform->y;

    // diferencia entre la cámara y el objetivo
    float diffX = targetX - camX;
    float diffY = targetY - camY;

    // solo mueve la cámara si el objetivo salió de la zona muerta
    // zona muerta: área central donde la cámara no se mueve aunque el player sí
    if (diffX > deadZoneWidth * 0.5f) diffX -= deadZoneWidth * 0.5f;
    else if (diffX < -deadZoneWidth * 0.5f) diffX += deadZoneWidth * 0.5f;
    else diffX = 0.0f; // dentro de la zona muerta, no mueve en X

    if (diffY > deadZoneHeight * 0.5f) diffY -= deadZoneHeight * 0.5f;
    else if (diffY < -deadZoneHeight * 0.5f) diffY += deadZoneHeight * 0.5f;
    else diffY = 0.0f; // dentro de la zona muerta, no mueve en Y

    // interpolación suave hacia el objetivo (igual al profe: smoothSpeed * dt)
    owner->transform->x += diffX * smoothSpeed * dt;
    owner->transform->y += diffY * smoothSpeed * dt;
}