#pragma once

// ===========================================================================
//  AssetManager.h
//  Caché de texturas.ncarga una vez, reutiliza siempre.
// ===========================================================================

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <unordered_map>
#include <string>

class AssetManager {
public:
    // Carga la textura si no existe, o devuelve la ya cargada (caché)
    SDL_Texture* load(SDL_Renderer* r, const std::string& path) {
        auto it = m_textures.find(path);
        if (it != m_textures.end()) return it->second; // ya estaba en caché

        SDL_Texture* tex = IMG_LoadTexture(r, path.c_str());
        if (!tex)
            SDL_Log("AssetManager: no se pudo cargar '%s': %s",
                path.c_str(), SDL_GetError());
        else
            m_textures[path] = tex; // guardar en caché
        return tex;
    }

    // Devuelve una textura ya cargada sin ir al disco
    SDL_Texture* get(const std::string& path) {
        auto it = m_textures.find(path);
        return (it != m_textures.end()) ? it->second : nullptr;
    }

    // Libera toda la memoria — llamar antes de SDL_DestroyRenderer
    void clear() {
        for (auto& entry : m_textures)
            SDL_DestroyTexture(entry.second);
        m_textures.clear();
    }

private:
    std::unordered_map<std::string, SDL_Texture*> m_textures;
};

// Instancia global accesible desde cualquier archivo del proyecto
extern AssetManager assets;