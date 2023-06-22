#pragma once

#include "Entities/Player/Player.hpp"
#include "Systems/FloatingTextSystem.hpp"

class PlayerGUI {
public:
    PlayerGUI(sf::VideoMode &vm, Player &player, float soundVolume,
              const std::string &heroName, const std::string &difficulty_name,
              std::unordered_map<std::string, std::string> &lang);
    ~PlayerGUI();

    void update_options(uint32_t &option_id, uint32_t &option_val,
                        std::vector<short> &id_vector, gui::Text *text,
                        gui::Text *value, gui::Sprite *sprite, float pos);
    void levelUpPlayer(uint32_t option_id, uint32_t option_val);
    void upgradePlayer(const std::string &name);
    void update_level(SoundEngine &soundEngine);
    void update_XP();
    void updating_XP(float dt);
    void update_HP();
    void updating_HP(SoundEngine &soundEngine, float dt);
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

    void updateSprint(float dt);
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
    void update(sf::Vector2f &mousePosView, float waveCountdown, float bossHP,
                float dt);
    void draw(sf::RenderTarget &target);

private:
    sf::VideoMode &vm;
    Player &player;

    sf::Texture attributes_texture;
    sf::Texture upgrades_texture;
    sf::Texture abilities_texture;
    sf::Texture select_texture;

    bool bossWave;
    float bossCooldown;

    bool upgraded;
    bool levelShown;
    bool leveling;
    bool upgrading;
    bool shopping;
    bool buyingAbility;
    bool escape;
    float hp_bar_percent;
    float xp_bar_percent;
    float boss_bar_percent;
    uint32_t option1_id;
    uint32_t option2_id;
    uint32_t option1_val;
    uint32_t option2_val;

    uint32_t item1Price;
    uint32_t item2Price;
    uint32_t item3Price;
    uint32_t item4Price;

    float titleCooldown;
    float waveCountdown;

    sf::RectangleShape ability_icon;

    sf::RectangleShape death_background;
    sf::RectangleShape escape_background;

    std::unordered_map<std::string, gui::ButtonText *> text_buttons;
    std::unordered_map<std::string, gui::Text *> texts;
    std::unordered_map<std::string, gui::ButtonSprite *> sprite_buttons;
    std::unordered_map<std::string, gui::Sprite *> sprites;

    std::unordered_map<std::string, std::string> &lang;
};
