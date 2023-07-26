#pragma once

#include "AbilityUpgradeGUI.hpp"

class UpgradeGUI {
public:
    UpgradeGUI(sf::VideoMode &t_vm, Player &t_player);
    ~UpgradeGUI();

    struct Upgrade {
        gui::Sprite *upgradeSprite;
        gui::Sprite *upgradeFrame;
        gui::ButtonSprite *upgradeButton;
        gui::Text *upgradeName;
        gui::Sprite *upgradeAbility;
        gui::Sprite *upgradeAttribute;
        gui::Text *upgradeAttributeValue;
        uint32_t value;
    };

    const bool isPressed(const std::string &t_name, bool mouseClicked);

    void changeUpgrade(const std::string &t_name, float t_x, float t_y,
                       const std::string &desc, uint32_t upgradeIconID,
                       uint32_t abilityIconID, uint32_t attributeIconID,
                       uint32_t attributeValue);
    void deleteUpgrade(const std::string &t_name);

    const bool hasClickedUpgrade(const sf::Vector2i &mousePos,
                                 bool mouseClicked, const std::string &t_name,
                                 SoundEngine *soundEngine);

    void update(const std::string &t_name, const sf::Vector2i &mousePos);
    void draw(sf::RenderTarget &target);

private:
    sf::VideoMode &vm;
    Player &player;

    sf::Texture selectsTexture;
    sf::Texture abilitiesTexture;
    sf::Texture attributesTexture;
    sf::Texture upgradesTexture;

    std::unordered_map<std::string, Upgrade> upgrades;
};
