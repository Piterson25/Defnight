#ifndef PLAYERGUI_HPP
#define PLAYERGUI_HPP

#include "FloatingTextSystem.hpp"
#include "GUI.hpp"
#include "MonsterSystem.hpp"
#include "Player.hpp"
#include "SoundEngine.hpp"

class Player;
class FloatingTextSystem;
class MonsterSystem;
class SoundEngine;

class PlayerGUI {
public:
    PlayerGUI(sf::Font *font, Player *player, sf::VideoMode &vm, const float &soundVolume,
              const std::string &heroName, const std::string &difficulty_name,
              std::unordered_map<std::string, std::string> &lang);
    virtual ~PlayerGUI();

    void update_options(uint32_t &option_id, uint32_t &option_val, std::vector<short> &id_vector,
                        gui::Text *text, gui::Text *value, gui::Sprite *sprite, const float &pos);
    void levelUpPlayer(uint32_t &option_id, uint32_t &option_val);
    void upgradePlayer(const std::string &name);
    void update_level(SoundEngine *soundEngine);
    void update_XP();
    void updating_XP(const float &dt);
    void update_HP();
    void updating_HP(SoundEngine *soundEngine, const float &dt);
    void update_Gold();
    void update_ability(const float &dt);
    void setAbilityIcon();
    void setIsEscape(const bool &escape);
    void updatePaused(bool &paused);
    void updateArmor();
    void updateAttack();
    void updateReg();
    void updateBossHP(const float &dt);
    const bool updateShop(const sf::Vector2i &mousePos, const bool &mouseClicked,
                          SoundEngine *soundEngine, FloatingTextSystem *floatingTextSystem);
    const bool updateBuyingAbility(const sf::Vector2i &mousePos, const bool &mouseClicked,
                                   SoundEngine *soundEngine,
                                   FloatingTextSystem *floatingTextSystem);

    const bool getIsEscape() const;
    const bool getIsLeveling() const;
    const bool getIsUpgrading() const;
    const bool getIsShopping() const;
    const bool getIsBuyingAbility() const;

    void updateSprint(const float &dt);
    void updateIsShopping();
    void updateIsBuyingAbility();
    void updateKills();
    void updateMonsterCountWave(const std::string &language, const uint32_t &wave,
                                const bool &bossWave, const size_t &monsterCount,
                                SoundEngine *soundEngine);
    void updateMonsterCount(const size_t &monsterCount);
    const uint8_t updateEscapeButton(const sf::Vector2i &mousePos, const bool &mouseClicked);
    const bool updateButtons(const sf::Vector2i &mousePos, const bool &mouseClicked,
                             SoundEngine *soundEngine);
    const bool updateLevelUpButtons(const sf::Vector2i &mousePos, const bool &mouseClicked,
                                    SoundEngine *soundEngine);
    const bool updateUpgradeButtons(const sf::Vector2i &mousePos, const bool &mouseClicked,
                                    SoundEngine *soundEngine);
    const uint8_t updateDeathScreenButtons(const sf::Vector2i &mousePos, const bool &mouseClicked);
    void update(sf::Vector2f &mousePosView, const float &waveCountdown,
                MonsterSystem *monsterSystem, const float &dt);
    void draw(sf::RenderTarget &target);

private:
    sf::Font font;

    Player *player;
    sf::VideoMode &vm;

    sf::Texture attributes_texture;
    sf::Texture upgrades_texture;
    sf::Texture abilities_texture;
    sf::Texture select_texture;

    bool bossWave;
    float bossCooldown;

    bool isLevelshown;
    bool isLeveling;
    bool isUpgrading;
    bool isShopping;
    bool isBuyingAbility;
    bool isEscape;
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

    std::unordered_map<std::string, std::string> lang;
    std::unordered_map<std::string, gui::ButtonText *> text_buttons;
    std::unordered_map<std::string, gui::Text *> texts;
    std::unordered_map<std::string, gui::ButtonSprite *> sprite_buttons;
    std::unordered_map<std::string, gui::Sprite *> sprites;
};

#endif