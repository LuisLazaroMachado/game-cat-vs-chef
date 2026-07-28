#pragma once

// ===========================================================================
//  GameObject.h
//  Contenedor de componentes.
//  Cada entidad del juego (player, enemigo, caja) ES un GameObject.
// ===========================================================================

#include <vector>
#include <string>
#include "Component.h"
#include "Transform.h"

class GameObject {
public:
    std::string name;          // nombre descriptivo ("Player", "Perro_1", "Boss")
    bool        active = true; // si está inactivo no se actualiza ni se dibuja
    Transform*  transform;     // acceso rápido al transform (siempre existe)

    // Constructor: crea el GameObject con nombre y le agrega su Transform automáticamente
    GameObject(const std::string& n) : name(n) {
        transform = addComponent<Transform>();
    }

    // Agrega un componente nuevo al GameObject
    // Uso: player->addComponent<SpriteRenderer>("assets/sprites/cat.png")
    template<typename T, typename... Args>
    T* addComponent(Args&&... args) {
        T* comp = new T(std::forward<Args>(args)...); // crea el componente
        comp->owner = this;                            // le dice a quién pertenece
        m_components.push_back(comp);                 // lo guarda en la lista
        return comp;
    }

    // Busca y devuelve el primer componente de tipo T que tenga este GameObject
    template<typename T>
    T* getComponent() {
        for (auto* c : m_components) {
            T* result = dynamic_cast<T*>(c); // intenta castear al tipo pedido
            if (result) return result;
        }
        return nullptr; // no lo encontró
    }

    // Llama update() en todos los componentes activos
    void update(float dt) {
        if (!active) return;
        for (auto* c : m_components) c->update(dt);
    }

    // Llama render() en todos los componentes activos
    void render() {
        if (!active) return;
        for (auto* c : m_components) c->render();
    }

    // Destructor: libera todos los componentes de memoria
    ~GameObject() {
        for (auto* c : m_components) delete c;
    }

private:
    std::vector<Component*> m_components; // lista interna de componentes
};