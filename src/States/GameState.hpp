#pragma once

#include "Entities/Player/Warrior.hpp"
#include "GUI/PlayerGUI.hpp"
#include "Game/PlayerStats.hpp"
#include "Map/TileMap.hpp"
#include "SettingsState.hpp"
#include "State.hpp"
#include "Systems/DropSystem.hpp"
#include "Systems/FloatingTextSystem.hpp"
#include "Systems/MonsterSystem.hpp"
#include "Systems/ParticleSystem.hpp"
#include "Systems/ProjectileSystem.hpp"

class GameState : public State {
public:
    GameState(float gridSize, sf::RenderWindow &window,
              GameSettings &gameSettings, SoundEngine &soundEngine,
              MusicEngine &musicEngine, std::stack<State *> &states,
              const std::string &mapName, const std::string &playerName,
              const std::string &difficultyName);
    ~GameState();

    void initGUI() override;
    void resetGUI() override;

    void update(float dt) override;
    void draw(sf::RenderTarget *target = NULL) override;

private:
    void savePlayerData();

    sf::Clock clock;
    sf::View view;
    sf::View viewHUD;

    std::unordered_map<std::string, std::unique_ptr<gui::ButtonText>>
        text_buttons;
    std::unordered_map<std::string, std::unique_ptr<gui::Text>> texts;
    std::unordered_map<std::string, std::unique_ptr<gui::ButtonSprite>>
        sprite_buttons;
    std::unordered_map<std::string, std::unique_ptr<gui::Sprite>> sprites;

    std::string mapName;
    std::string difficultyName;

    PlayerGUI *playerGUI;
    Player *player;
    MonsterSystem *monsterSystem;
    ProjectileSystem *projectileSystem;
    FloatingTextSystem *floatingTextSystem;
    DropSystem *dropSystem;
    TileMap *tileMap;
    ParticleSystem *particleSystem;

    uint32_t wave;
    uint32_t sumHP;
    uint32_t gems;
    float waveCountdown;
};
