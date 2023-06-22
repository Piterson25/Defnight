#pragma once

#include "Entities/Drops/Coin.hpp"
#include "Entities/Drops/Heart.hpp"
#include "GUI/PlayerGUI.hpp"

class PlayerGUI;

class DropSystem {
public:
    DropSystem(sf::VideoMode &vm, float difficulty_mod);
    ~DropSystem();

    void addDrop(const std::string &name, float posX, float posY,
                 uint32_t worth);

    void update(Player &player, PlayerGUI &playerGUI,
                FloatingTextSystem &floatingTextSystem,
                SoundEngine &soundEngine, float dt);
    void draw(sf::RenderTarget &target);

private:
    std::list<std::unique_ptr<Drop>> drops;
    float difficulty_mod;
    sf::VideoMode vm;
};
