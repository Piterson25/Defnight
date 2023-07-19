#pragma once

#include "Entities/Player/Player.hpp"
#include "Systems/FloatingTextSystem.hpp"

class AbilityUpgradeGUI {
public:
    AbilityUpgradeGUI(sf::VideoMode &t_vm, Player &t_player);
    ~AbilityUpgradeGUI();

    struct BuyItem {
        gui::Sprite *itemSprite;
        gui::Sprite *itemFrame;
        gui::ButtonSprite *itemButton;
        gui::Text *itemDesc;
        gui::Text *itemValue;
        gui::Sprite *itemCoin;
        gui::Text *itemPrice;
        uint32_t price;
    };

    const uint32_t getPrice(const std::string &t_name);

    void increasePrice(const std::string &t_name);

    void addPlayerStat(const std::string &t_name, float t_x, float t_y,
                       const std::string &desc);

    void addAbilityUpgrade(const std::string &t_name, float t_x, float t_y,
                           uint32_t iconID, const std::string &desc,
                           const std::string &value, uint32_t price);

    void setAbility(const sf::IntRect &intRect);
    const bool isPressed(const std::string &t_name, bool mouseClicked);
    const bool hasBoughtUpgrade(const sf::Vector2i &mousePos, bool mouseClicked,
                                const std::string &t_name,
                                FloatingTextSystem *floatingTextSystem,
                                SoundEngine *soundEngine);
    void buy(const std::string &t_name, FloatingTextSystem *floatingTextSystem);
    void updateItemFrames();
    void updatePlayerInfo(const std::string &t_name, const std::string &t_desc);
    void update(const std::string &t_name, const sf::Vector2i &mousePos);
    void draw(sf::RenderTarget &target);

private:
    sf::VideoMode &vm;
    Player &player;
    sf::Texture abilityUpgradesTexture;
    sf::Texture selectsTexture;
    sf::Texture attributesTexture;

    gui::Sprite *abilitySprite;
    std::unordered_map<std::string, std::unique_ptr<gui::Text>> playerStats;
    std::unordered_map<std::string, BuyItem> abilityUpgrades;
};
