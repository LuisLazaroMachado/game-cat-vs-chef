#pragma once
// ===========================================================================
//  SoundManager.h
// ===========================================================================
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <unordered_map>
#include <string>
struct SoundManager {
    MIX_Mixer* mixer = nullptr;
    std::unordered_map<std::string, MIX_Audio*> clips;
    MIX_Track* music = nullptr;
    bool init() {
        if (!MIX_Init()) { SDL_Log("MIX_Init failed: %s", SDL_GetError()); return false; }
        mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
        if (!mixer) { SDL_Log("MIX_CreateMixerDevice failed: %s", SDL_GetError()); return false; }
        music = MIX_CreateTrack(mixer);
        if (!music) { SDL_Log("MIX_CreateTrack (music) failed: %s", SDL_GetError()); return false; }
        return true;
    }
    MIX_Audio* load(const std::string& id, const std::string& path, bool predecode) {
        MIX_Audio* a = MIX_LoadAudio(mixer, path.c_str(), predecode);
        if (!a) { SDL_Log("MIX_LoadAudio('%s'): %s", path.c_str(), SDL_GetError()); return nullptr; }
        clips[id] = a;
        return a;
    }
    void playMusic(const std::string& id, bool loop) {
        auto it = clips.find(id);
        if (it == clips.end()) { SDL_Log("playMusic: clip '%s' no esta cargado", id.c_str()); return; }
        if (!MIX_SetTrackAudio(music, it->second)) {
            SDL_Log("MIX_SetTrackAudio('%s') fallo: %s", id.c_str(), SDL_GetError());
            return;
        }
        SDL_PropertiesID opt = SDL_CreateProperties();
        SDL_SetNumberProperty(opt, MIX_PROP_PLAY_LOOPS_NUMBER, loop ? -1 : 0);
        if (!MIX_PlayTrack(music, opt)) {
            SDL_Log("MIX_PlayTrack('%s') fallo: %s", id.c_str(), SDL_GetError());
        }
        SDL_DestroyProperties(opt);
    }
    void stopMusic() { MIX_StopTrack(music, 0); }
    void setMusicVolume(float gain) { MIX_SetTrackGain(music, gain); }
    void setSfxVolume(float gain) { sfxGain = gain; }
    float sfxGain = 1.0f;
    void loadGameSounds() {
        load("victoria", "assets/victoria.mp3", true);
        load("muerte", "assets/muerte.mp3", true);
        load("dano", "assets/dano.mp3", true);
        load("salto", "assets/salto.mp3", true);
        load("nivel_bgm", "assets/nivel_bgm.mp3", false);
        load("final_boss", "assets/final_boss.mp3", false);
        setMusicVolume(0.5f);
        setSfxVolume(1.0f);
    }
    std::string currentMusic = "";
    void updateLevelMusic(int gameStateIsPlayingOrMenu, int currentLevel, bool isPlayingState) {
        std::string wanted = (isPlayingState && currentLevel == 3) ? "final_boss" : "nivel_bgm";
        if (gameStateIsPlayingOrMenu) {
            if (currentMusic != wanted) {
                playMusic(wanted, true);
                currentMusic = wanted;
            }
        }
        else {
            currentMusic = "";
        }
    }
    // Fire-and-forget: permite sonidos simultaneos/superpuestos sin cortarse entre si.
    void playSfx(const std::string& id) {
        auto it = clips.find(id);
        if (it == clips.end()) { SDL_Log("playSfx: clip '%s' no esta cargado", id.c_str()); return; }
        if (!MIX_PlayAudio(mixer, it->second)) {
            SDL_Log("MIX_PlayAudio('%s') fallo: %s", id.c_str(), SDL_GetError());
        }
    }
    void clear() {
        for (auto& kv : clips) MIX_DestroyAudio(kv.second);
        clips.clear();
        if (mixer) MIX_DestroyMixer(mixer);
        mixer = nullptr;
        MIX_Quit();
    }
};