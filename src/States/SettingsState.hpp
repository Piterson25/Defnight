#pragma once

#include "GUI/GUI.hpp"
#include "State.hpp"

class SettingsState : public State {
public:
    SettingsState(float gridSize, sf::RenderWindow &window,
                  GameSettings &gameSettings, SoundEngine &soundEngine,
                  MusicEngine &musicEngine, std::stack<State *> &states);
    ~SettingsState();

    struct Setting {
        std::unique_ptr<gui::ButtonSprite> leftArrow;
        std::unique_ptr<gui::ButtonSprite> rightArrow;
        std::unique_ptr<gui::Text> desc;
        std::unique_ptr<gui::Text> change;

        void draw(sf::RenderTarget &target)
        {
            desc->draw(target);
            leftArrow->draw(target);
            change->draw(target);
            rightArrow->draw(target);
        }
    };

    struct Keybind {
        std::unique_ptr<gui::Text> desc;
        std::unique_ptr<gui::Text> key;

        void draw(sf::RenderTarget &target)
        {
            desc->draw(target);
            key->draw(target);
        }
    };

    void addSetting(const std::string &t_name, float t_x, float t_y,
                    const std::string &desc, const std::string &change);

    void addKeybind(const std::string &t_name, float t_x, float t_y,
                    const std::string &desc, const std::string &key);

    void initGUI();
    void resetGUI();

    void update(float dt) override;
    void draw(sf::RenderTarget *target = NULL) override;

private:
    uint32_t page;
    std::unordered_map<std::string, Setting> settings;
    std::unordered_map<std::string, Keybind> keybindsTexts;

    std::vector<sf::VideoMode> videoModes;

    sf::VideoMode mode;
    int id;
    int fps_id;
    bool fullscreen;
    uint32_t fpsLimit;
    std::vector<uint32_t> fpsLimits;
    bool fpsCounterOn;
    float musicVolume;
    int musicVolume_id;
    std::vector<uint32_t> musicVolumes;
    float soundsVolume;
    int soundsVolume_id;
    std::vector<uint32_t> soundsVolumes;
    std::string language;

    std::unordered_map<std::string, std::unique_ptr<gui::ButtonText>>
        text_buttons;
    std::unordered_map<std::string, std::unique_ptr<gui::Text>> texts;
    std::unordered_map<std::string, std::unique_ptr<gui::ButtonSprite>>
        sprite_buttons;
    std::unordered_map<std::string, std::unique_ptr<gui::Sprite>> sprites;
};
