#pragma once

// ===========================================================================
//  Component.h
//  Clase base para todos los componentes del motor.
//  Cada componente se adjunta a un GameObject.
// ===========================================================================

// Declaración adelantada: Component conoce a GameObject sin incluirlo completo
class GameObject;

class Component {
public:
    GameObject* owner = nullptr;   // el GameObject al que pertenece este componente

    // Métodos virtuales que cada componente puede sobrescribir
    virtual void update(float dt) {}   // lógica por frame (dt = delta time en segundos)
    virtual void render() {}           // dibujo por frame
    virtual ~Component() {}            // destructor virtual para liberar memoria correctamente
};