#pragma once

// ===========================================================================
//  Scene.h
//  Contenedor de todos los GameObjects del juego.
//  createGameObject(), update(), render().
// ===========================================================================

#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include "GameObject.h"
#include "Camera.h"

class Scene {
public:
    SDL_Renderer* renderer = nullptr; // renderer compartido con todos los componentes
    Camera* mainCamera = nullptr; // cámara activa de la escena

    // Constructor: guarda el renderer
    Scene(SDL_Renderer* r) : renderer(r) {}

    // Crea un GameObject, lo agrega a la escena y lo devuelve
    // Uso: GameObject* player = scene.createGameObject("Player")
    GameObject* createGameObject(const std::string& name) {
        GameObject* go = new GameObject(name);
        m_objects.push_back(go);
        return go;
    }

    // Registra la cámara principal de la escena
    void setMainCamera(Camera* cam) { mainCamera = cam; }

    // Llama update() en todos los GameObjects activos
    void update(float dt) {
        for (auto* go : m_objects)
            if (go->active) go->update(dt);
    }

    // Llama render() en todos los GameObjects activos
    void render() {
        for (auto* go : m_objects)
            if (go->active) go->render();
    }

    // Libera todos los GameObjects de memoria
    ~Scene() {
        for (auto* go : m_objects) delete go;
    }

private:
    std::vector<GameObject*> m_objects; // todos los objetos de la escena
};