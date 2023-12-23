#pragma once

#include "Game/PlayerStats.hpp"
#include "GameState.hpp"
#include "SettingsState.hpp"
#include "StatsState.hpp"

#define VERSION "v0.2.5"

class MainMenuState : public State {
public:
    MainMenuState(float gridSize, sf::RenderWindow &window,
                  GameSettings &gameSettings, SoundEngine &soundEngine,
                  MusicEngine &musicEngine, std::stack<State *> &states);
    ~MainMenuState();

    void initGUI();
    void resetGUI();

    void update(float dt) override;
    void draw(sf::RenderTarget *target = NULL) override;

private:
    void fadingEffect(float dt);
    void setPlayerRank();

    struct RankText {
        std::unique_ptr<gui::Text> rank;
        std::unique_ptr<gui::Text> xp;
    };

    sf::Texture framesTexture;
    std::vector<RankText> ranksTexts;
    PlayerStats::Rank playerRank;

    std::unordered_map<std::string, std::unique_ptr<gui::ButtonText>>
        text_buttons;
    std::unordered_map<std::string, std::unique_ptr<gui::Text>> texts;
    std::unordered_map<std::string, std::unique_ptr<gui::ButtonSprite>>
        sprite_buttons;
    std::unordered_map<std::string, std::unique_ptr<gui::Sprite>> sprites;

    uint16_t page;

    bool loadedPlayerData;
    PlayerStats::PlayerData playerData;

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

    std::vector<std::unique_ptr<gui::Sprite>> abilties;

    std::string map_name;
    std::string hero_name;
    std::string difficulty_name;
};
