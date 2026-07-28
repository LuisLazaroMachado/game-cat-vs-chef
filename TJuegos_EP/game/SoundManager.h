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
    MIX_Track* sfx = nullptr;

    bool init() {
        if (!MIX_Init()) return false;
        mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
        if (!mixer) return false;
        music = MIX_CreateTrack(mixer);
        sfx = MIX_CreateTrack(mixer);
        return music && sfx;
    }
    MIX_Audio* load(const std::string& id, const std::string& path, bool predecode) {
        MIX_Audio* a = MIX_LoadAudio(mixer, path.c_str(), predecode);
        if (!a) { SDL_Log("MIX_LoadAudio('%s'): %s", path.c_str(), SDL_GetError()); return nullptr; }
        clips[id] = a;
        return a;
    }
    void playMusic(const std::string& id, bool loop) {
        if (clips.find(id) == clips.end()) return;
        MIX_SetTrackAudio(music, clips[id]);
        SDL_PropertiesID opt = SDL_CreateProperties();
        SDL_SetNumberProperty(opt, MIX_PROP_PLAY_LOOPS_NUMBER, loop ? -1 : 0);
        MIX_PlayTrack(music, opt);
        SDL_DestroyProperties(opt);
    }
    void stopMusic() { MIX_StopTrack(music, 0); }
    void setMusicVolume(float gain) { MIX_SetTrackGain(music, gain); }
    void setSfxVolume(float gain) { MIX_SetTrackGain(sfx, gain); }

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

    void playSfx(const std::string& id) {
        if (clips.find(id) == clips.end()) return;
        MIX_SetTrackAudio(sfx, clips[id]);
        MIX_PlayTrack(sfx, 0);
    }
    void clear() {
        for (auto& kv : clips) MIX_DestroyAudio(kv.second);
        clips.clear();
        if (mixer) MIX_DestroyMixer(mixer);
        mixer = nullptr;
        MIX_Quit();
    }
};