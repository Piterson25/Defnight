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
    const bool updateAbilityIcon(float value);
    void setWaveCountdownText(const std::string &text);
    bool hasClickedMenu(const sf::Vector2i &mousePos);
    bool hasClickedShop(const sf::Vector2i &mousePos);
    bool hasClickedAbilityUpgrade(const sf::Vector2i &mousePos);
    void update(const sf::Vector2f &mousePosView);
    void drawAbility(sf::RenderTarget &target);
    void drawWaveCountdown(sf::RenderTarget &target);
    void drawMonsterCount(sf::RenderTarget &target);
    void drawMenu(sf::RenderTarget &target);
    void draw(sf::RenderTarget &target);

private:
    struct Bar {
        std::unique_ptr<gui::Sprite> frame;
        std::unique_ptr<gui::Sprite> bar;
        std::unique_ptr<gui::Text> text;
        std::unique_ptr<gui::Text> hoverText;
        float percent;
        bool isHovering;

        void update(const sf::Vector2f &mousePosView)
        {
            if ((frame->getGlobalBounds().contains(mousePosView))) {
                isHovering = true;
            }
            else {
                isHovering = false;
            }
        }

        void draw(sf::RenderTarget &target)
        {
            frame->draw(target);
            bar->draw(target);
            if (!isHovering) {
                text->draw(target);
            }
            else {
                hoverText->draw(target);
            }
        }
    };

    sf::VideoMode &vm;
    Player &player;

    sf::Texture framesTexture;
    sf::Texture attributesTexture;
    sf::Texture barsTexture;
    sf::Texture upgradesTexture;
    sf::Texture abilitiesTexture;

    sf::RectangleShape abilityCooldown;
    Bar XPBar;
    Bar HPBar;
    Bar SprintBar;

    std::unordered_map<std::string, std::unique_ptr<gui::Text>> texts;
    std::unordered_map<std::string, std::unique_ptr<gui::Sprite>> sprites;
    std::unordered_map<std::string, std::unique_ptr<gui::ButtonSprite>>
        sprite_buttons;
};
