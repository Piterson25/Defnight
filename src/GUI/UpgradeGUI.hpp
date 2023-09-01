#pragma once

#include "AbilityUpgradeGUI.hpp"

class UpgradeGUI {
public:
    UpgradeGUI(sf::VideoMode &t_vm, Player &t_player);
    ~UpgradeGUI();

    struct Upgrade {
        std::unique_ptr<gui::Sprite> sprite;
        std::unique_ptr<gui::ButtonSprite> button;
        std::unique_ptr<gui::Text> name;
        std::unique_ptr<gui::Sprite> ability;
        std::unique_ptr<gui::Sprite> attribute;
        std::unique_ptr<gui::Text> attributeEffect;
        uint32_t effect;
    };

    void changeUpgrade(const std::string &t_name, float t_x, float t_y,
                       const std::string &desc, uint32_t abilityIconID,
                       uint32_t attributeIconID, uint32_t attributeValue,
                       const std::string &attributeText);
    void deleteUpgrade(const std::string &t_name);

    const bool hasClickedUpgrade(const sf::Vector2i &mousePos,
                                 const std::string &t_name,
                                 SoundEngine *soundEngine);
    void draw(sf::RenderTarget &target);

private:
    sf::VideoMode &vm;
    Player &player;

    sf::Texture abilitiesTexture;
    sf::Texture attributesTexture;
    sf::Texture upgradesTexture;

    std::unordered_map<std::string, Upgrade> upgrades;
};
