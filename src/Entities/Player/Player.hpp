#pragma once

#include "Audio/SoundEngine.hpp"
#include "Entities/Entity.hpp"

class Player : public Entity {
public:
    Player(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
           float t_y);
    virtual ~Player();

    const uint32_t getGold() const;
    const uint32_t getArmor() const;
    const uint32_t getReg() const;
    const uint32_t getLevel() const;
    const uint32_t getMaxXP() const;
    const uint32_t getLastMaxXP() const;
    const float getSprint() const;
    const uint32_t getMaxSprint() const;
    const uint32_t getCriticalChance() const;
    const uint32_t getKills() const;
    const bool getIncreasedArmor() const;
    const bool getRegenerating() const;
    const bool getLeveling() const;
    const bool getSprinting() const;
    const bool getSpawned() const;
    const bool getAbilityActive() const;
    const float getAbilityCooldown() const;
    const float getAbilityTime() const;
    const float getAbilityMaxTime() const;
    const bool getPlayedSound() const;

    void setGold(uint32_t t_gold);
    void setArmor(uint32_t t_armor);
    void setReg(uint32_t t_reg);
    void setSprint(float t_sprint);
    void setMaxSprint(uint32_t t_maxSprint);
    void setCriticalChance(uint32_t t_criticalChance);
    void setKills(uint32_t t_kills);
    void setRegenerating(bool t_regenerating);
    void setLeveling(bool t_leveling);
    void setSprinting(bool t_sprinting);
    void setAbilityActive(bool t_abilityActive);
    void setAbilityCooldown(float t_abilityCooldown);
    void setAbilityTime(float t_abilityTime);
    void setAbilityMaxTime(float t_abilityMaxTime);
    void setPlayedSound(bool t_playedSound);

    void controls(const std::unordered_map<std::string, int> &keybinds,
                  float dt);
    void whooshSound(SoundEngine &soundEngine);
    const bool addXP(uint32_t monsterXP);
    const bool regeneration(float dt);
    const bool checkIfAbility();
    void abilityCounter(float dt);
    void endAbility();
    void doAbility(SoundEngine &soundEngine);
    void spawn(float dt);
    void updateSprint(float dt);
    void update(float dt);
    void draw(sf::RenderTarget &target);
    void drawShadow(sf::RenderTarget &target);

protected:
    sf::Sprite shadow;
    sf::Texture shadow_texture;
    sf::Sprite ability;
    sf::Texture ability_texture;

    std::list<sf::RectangleShape> particles;

    uint32_t gold;
    uint32_t armor;
    uint32_t reg;
    uint32_t level;
    uint32_t maxXP;
    uint32_t lastMaxXP;
    float sprint;
    uint32_t maxSprint;
    uint32_t criticalChance;
    uint32_t kills;
    bool increasedArmor;
    bool isRegenerating;
    float regCooldown;
    bool isLeveling;
    bool isSprinting;
    bool spawned;
    float spawnCountdown;

    bool abilityActive;
    float abilityCooldown;
    float abilityTime;
    float abilityMaxTime;

    bool playedSound;
};
