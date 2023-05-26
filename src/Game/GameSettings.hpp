#pragma once

#include "Utils/header.h"

class GameSettings {
public:
    GameSettings();
    ~GameSettings();

    void save();
    void load();

    bool fullscreen;
    uint32_t fpsLimit;
    bool fpsCounterOn;
    float musicVolume;
    float soundsVolume;

    std::string language;
    sf::VideoMode resolution;
    std::vector<sf::VideoMode> videoModes;
    std::unordered_map<std::string, std::string> lang;
    std::unordered_map<std::string, int> keybinds;
    std::unordered_map<std::string, int> supportedKeys;
};
