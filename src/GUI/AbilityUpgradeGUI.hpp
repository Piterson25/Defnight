#pragma once

#include "Entities/Player/Player.hpp"
#include "Systems/FloatingTextSystem.hpp"

struct BuyItem {
    std::unique_ptr<gui::Sprite> sprite;
    std::unique_ptr<gui::ButtonSprite> button;
    std::unique_ptr<gui::Sprite> lockSprite;
    std::unique_ptr<gui::Text> desc;
    std::unique_ptr<gui::Text> effect;
    std::unique_ptr<gui::Sprite> coinSprite;
    std::unique_ptr<gui::Text> priceText;
    std::vector<std::unique_ptr<gui::Sprite>> segments;
    uint32_t boughtNumber;
    uint32_t maxNumber;
    uint32_t price;
    bool locked;
};

class AbilityUpgradeGUI {
public:
    AbilityUpgradeGUI(sf::VideoMode &t_vm, Player &t_player);
    ~AbilityUpgradeGUI();

    const uint32_t getPrice(const std::string &t_name);

    void increasePrice(const std::string &t_name);

    void addPlayerStat(const std::string &t_name, float t_x, float t_y,
                       const std::string &desc);

    void addAbilityUpgrade(const std::string &t_name, float t_x, float t_y,
                           uint32_t iconID, const std::string &desc,
                           const std::string &value, uint32_t price,
                           uint32_t boughtNumber, uint32_t maxNumber);
    void updateSegments(const std::string &t_name);

    const bool isPressed(const std::string &t_name, bool mouseClicked);
    const bool hasBoughtUpgrade(const sf::Vector2i &mousePos, bool mouseClicked,
                                const std::string &t_name,
                                FloatingTextSystem *floatingTextSystem,
                                SoundEngine *soundEngine);
    void lockUpgrade(const std::string &t_name);
    void unlockUpgrade(const std::string &t_name);
    void disableUpgrade(const std::string &t_name);
    void buy(const std::string &t_name, FloatingTextSystem *floatingTextSystem);
    void updateItemFrames();
    void updatePlayerInfo(const std::string &t_name, const std::string &t_desc);
    void update(const std::string &t_name, const sf::Vector2i &mousePos);
    void draw(sf::RenderTarget &target);

private:
    sf::VideoMode &vm;
    Player &player;
    sf::Texture abilityUpgradesTexture;
    sf::Texture attributesTexture;

    std::unordered_map<std::string, std::unique_ptr<gui::Text>> playerStats;
    std::unordered_map<std::string, BuyItem> abilityUpgrades;
};
