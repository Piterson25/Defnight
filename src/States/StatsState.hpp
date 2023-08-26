#pragma once

#include "GUI/GUI.hpp"
#include "Game/PlayerStats.hpp"
#include "State.hpp"

class StatsState : public State {
public:
    StatsState(float gridSize, sf::RenderWindow &window,
               GameSettings &gameSettings, SoundEngine &soundEngine,
               MusicEngine &musicEngine, std::stack<State *> &states);
    ~StatsState();

    void update(float dt) override;
    void draw(sf::RenderTarget *target = NULL) override;

private:
    PlayerStats::PlayerData playerData;

    std::unordered_map<std::string, std::unique_ptr<gui::ButtonText>>
        text_buttons;
    std::unordered_map<std::string, std::unique_ptr<gui::Text>> texts;
    std::unordered_map<std::string, std::unique_ptr<gui::ButtonSprite>>
        sprite_buttons;
    std::unordered_map<std::string, std::unique_ptr<gui::Sprite>> sprites;
};
