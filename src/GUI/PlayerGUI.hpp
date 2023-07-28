#pragma once

#include "StatsGUI.hpp"

class PlayerGUI {
public:
    PlayerGUI(sf::VideoMode &vm, Player &player, float soundVolume,
              const std::string &heroName, const std::string &difficulty_name,
              std::unordered_map<std::string, std::string> &lang);
    ~PlayerGUI();

    void levelUpPlayer(uint32_t optionID, uint32_t optionValue);
    void upgradePlayer(const std::string &name);
    void update_level(SoundEngine &soundEngine);
    void updateXP();
    void updateHP();
    void updateSprint();
    void updatingHP(SoundEngine &soundEngine, float dt);
    void update_Gold();
    void update_ability(float dt);
    void setAbilityIcon();
    void setIsEscape(bool escape);
    void updatePaused(bool &paused);
    void updateArmor();
    void updateAttack();
    void updateReg();
    void updateBossHP(float dt);
    const bool hasClickedShopBuy(const sf::Vector2i &mousePos,
                                 bool mouseClicked, SoundEngine &soundEngine,
                                 FloatingTextSystem &floatingTextSystem);
    const bool hasClickedAbilityBuy(const sf::Vector2i &mousePos,
                                    bool mouseClicked, SoundEngine &soundEngine,
                                    FloatingTextSystem &floatingTextSystem);

    const bool isEscape() const;
    const bool isLeveling() const;
    const bool isUpgrading() const;
    const bool isShopping() const;
    const bool isBuyingAbility() const;

    void updatingSprint(float dt);
    void updateIsShopping();
    void updateIsBuyingAbility();
    void updateKills();
    void updateMonsterCountWave(const std::string &language, uint32_t wave,
                                bool bossWave, const size_t &monsterCount,
                                SoundEngine &soundEngine);
    void updateMonsterCount(const size_t &monsterCount);
    const uint8_t updateEscapeButton(const sf::Vector2i &mousePos,
                                     const bool &mouseClicked);
    const bool hasClickedButtons(const sf::Vector2i &mousePos,
                                 bool mouseClicked, SoundEngine &soundEngine);
    const bool hasClickedLevelUpButtons(const sf::Vector2i &mousePos,
                                        bool mouseClicked,
                                        SoundEngine &soundEngine);
    const bool hasClickedUpgradeButtons(const sf::Vector2i &mousePos,
                                        bool mouseClicked,
                                        SoundEngine &soundEngine);
    const uint8_t updateDeathScreenButtons(const sf::Vector2i &mousePos,
                                           bool mouseClicked);
    void updatePlayerAttributes();
    void update(sf::Vector2f &mousePosView, float waveCountdown, float bossHP,
                float dt);
    void draw(sf::RenderTarget &target);

private:
    sf::VideoMode &vm;
    Player &player;
    StatsGUI *statsGUI;
    ShopGUI *shopGUI;
    UpgradeGUI *upgradeGUI;
    AbilityUpgradeGUI *abilityUpgradeGUI;

    enum class SideGUI { SHOP, ABILITY_UPGRADE, UPGRADE, NONE };

    SideGUI sideGUI;

    sf::Texture attributesTexture;

    struct Option {
        std::unique_ptr<gui::Sprite> optionSprite;
        std::unique_ptr<gui::ButtonSprite> optionButton;
        std::unique_ptr<gui::Text> optionDesc;
        std::unique_ptr<gui::Text> optionValue;
        uint32_t value;
        uint32_t id;
    };

    void updateOption(Option &option, std::vector<short> &id_vector, float pos);

    Option option1;
    Option option2;

    bool bossWave;
    float bossCooldown;

    bool leveling;
    bool upgrading;
    bool escape;
    float boss_bar_percent;

    float titleCooldown;
    float waveCountdown;

    sf::RectangleShape death_background;
    sf::RectangleShape escape_background;

    std::unordered_map<std::string, std::unique_ptr<gui::ButtonText>>
        text_buttons;
    std::unordered_map<std::string, std::unique_ptr<gui::Text>> texts;
    std::unordered_map<std::string, std::unique_ptr<gui::ButtonSprite>>
        sprite_buttons;
    std::unordered_map<std::string, std::unique_ptr<gui::Sprite>> sprites;

    std::unordered_map<std::string, std::string> &lang;
};
