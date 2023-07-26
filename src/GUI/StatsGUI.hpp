#pragma once

#include "ShopGUI.hpp"

class StatsGUI {
public:
    StatsGUI(sf::VideoMode &t_vm, Player &t_player,
             std::unordered_map<std::string, std::string> &t_lang);
    ~StatsGUI();

    void updateGold();
    void updateArmor();
    void updateReg();
    void updateAttack();
    void updateAttackSpeed();
    void updateSpeed();
    void updateCritical();

    void updateXP();
    void updatingXP(float dt);
    void updateLevel();
    void updateHP();
    void updatingHP(float dt);
    void updateSprint();
    void updatingSprint(float dt);

    void updateMonsterCount(const std::string &count);
    void updateWaveNumber(const std::string &text);

    void upgradePlayer(const std::string &t_name, const sf::IntRect &intRect);
    void setAbilityIcon();
    void updateAbilityIcon(float value);
    void setWaveCountdownText(const std::string &text);
    const bool hasClickedShop(const sf::Vector2i &mousePos, bool mouseClicked);
    const bool hasClickedAbilityUpgrade(const sf::Vector2i &mousePos,
                                        bool mouseClicked);
    void update(const sf::Vector2f &mousePosView);
    void drawAbility(sf::RenderTarget &target);
    void drawWaveCountdown(sf::RenderTarget &target);
    void drawMonsterCount(sf::RenderTarget &target);
    void draw(sf::RenderTarget &target);

private:
    sf::VideoMode &vm;
    Player &player;

    sf::Texture attributesTexture;
    sf::Texture upgradesTexture;
    sf::Texture abilitiesTexture;
    sf::Texture selectTexture;

    bool levelShown;
    float hpBarPercent;
    float xpBarPercent;
    sf::RectangleShape abilityIcon;

    std::unordered_map<std::string, std::unique_ptr<gui::Text>> texts;
    std::unordered_map<std::string, std::unique_ptr<gui::Sprite>> sprites;
    std::unordered_map<std::string, std::unique_ptr<gui::ButtonSprite>>
        sprite_buttons;
};
