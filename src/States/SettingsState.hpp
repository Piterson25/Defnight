#pragma once

#include "GUI/GUI.hpp"
#include "State.hpp"

class SettingsState : public State {
public:
    SettingsState(float gridSize, sf::RenderWindow &window,
                  GameSettings &gameSettings, SoundEngine &soundEngine,
                  MusicEngine &musicEngine, std::stack<State *> &states);
    ~SettingsState();

    void initGUI();
    void resetGUI();

    void update(float dt) override;
    void draw(sf::RenderTarget *target = NULL) override;

private:
    std::vector<sf::VideoMode> videoModes;

    sf::VideoMode mode;
    size_t id;
    size_t fps_id;
    bool fullscreen;
    uint32_t fpsLimit;
    std::vector<uint32_t> fpsLimits;
    bool fpsCounterOn;
    float musicVolume;
    size_t musicVolume_id;
    std::vector<uint32_t> musicVolumes;
    float soundsVolume;
    size_t soundsVolume_id;
    std::vector<uint32_t> soundsVolumes;
    std::string language;

    std::unordered_map<std::string, std::unique_ptr<gui::ButtonText>>
        text_buttons;
    std::unordered_map<std::string, std::unique_ptr<gui::Text>> texts;
    std::unordered_map<std::string, std::unique_ptr<gui::ButtonSprite>>
        sprite_buttons;
    std::unordered_map<std::string, std::unique_ptr<gui::Sprite>> sprites;
};
