#pragma once

#include "Entities/player/Warrior.hpp"
#include "GUI/PlayerGUI.hpp"
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
              const std::string &map_name, const std::string &hero_name,
              const std::string &difficulty_name);
    ~GameState();

    void initGUI();
    void resetGUI();

    void update(float dt);
    void draw(sf::RenderTarget *target = NULL);

private:
    sf::View view;
    sf::View viewHUD;

    std::unordered_map<std::string, std::unique_ptr<gui::ButtonText>> text_buttons;
    std::unordered_map<std::string, std::unique_ptr<gui::Text>> texts;
    std::unordered_map<std::string, std::unique_ptr<gui::ButtonSprite>> sprite_buttons;
    std::unordered_map<std::string, std::unique_ptr<gui::Sprite>> sprites;

    sf::Sprite background;
    sf::Texture background_texture;

    PlayerGUI *playerGUI;
    Player *player;
    MonsterSystem *monsterSystem;
    ProjectileSystem *projectileSystem;
    FloatingTextSystem *floatingTextSystem;
    DropSystem *dropSystem;
    sf::Texture tiles_texture;
    sf::VertexArray vertexArray;
    TileMap *tileMap;
    ParticleSystem *particleSystem;

    uint32_t wave;
    uint32_t sumHP;
    uint32_t gems;
    float waveCountdown;
};
