#pragma once

#include "GUI/GUI.hpp"
#include "GameState.hpp"
#include "SettingsState.hpp"

class MainMenuState : public State {
public:
    MainMenuState(float gridSize, sf::RenderWindow &window,
                  GameSettings &gameSettings, SoundEngine &soundEngine,
                  MusicEngine &musicEngine, std::stack<State *> &states);
    ~MainMenuState();

    void initGUI();
    void resetGUI();

    void update(float dt);
    void draw(sf::RenderTarget *target = NULL);

private:
    void fadingEffect(float dt);

    std::unordered_map<std::string, gui::ButtonText *> text_buttons;
    std::unordered_map<std::string, gui::Text *> texts;
    std::unordered_map<std::string, gui::ButtonSprite *> sprite_buttons;
    std::unordered_map<std::string, gui::Sprite *> sprites;

    uint16_t page;

    sf::View mapView;
    sf::Sprite map;
    sf::Texture map_texture;
    sf::RectangleShape dimMap;
    sf::RectangleShape quitBackground;
    sf::Vector2f mapVelocity;
    float mapRotate;

    sf::RectangleShape dimBackground;
    float introCooldown;
    float dimAlpha;
    bool fading;
    bool quitwindow;
    bool appearing;

    bool choosing_hero;
    sf::Texture attributes_texture;
    std::vector<sf::Sprite> attribute_vec;

    sf::Texture abilities_texture;
    std::vector<sf::Sprite> abilities_vec;

    std::string map_name;
    std::string hero_name;
    std::string difficulty_name;
};