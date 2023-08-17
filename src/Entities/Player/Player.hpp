#pragma once

#include "Audio/SoundEngine.hpp"
#include "Components/AbilityComponent.hpp"
#include "Entities/Entity.hpp"

class Player : public Entity {
public:
    Player(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
           float t_y);
    virtual ~Player();

    const uint32_t getWave() const;
    const uint32_t getGold() const;
    const uint32_t getArmor() const;
    const uint32_t getReg() const;
    const uint32_t getLevel() const;
    const uint32_t getMaxXP() const;
    const uint32_t getPendingXP() const;
    const float getSprint() const;
    const uint32_t getMaxSprint() const;
    const uint32_t getCriticalChance() const;
    const uint32_t getProjectileAttack() const;
    const uint32_t getProjectilePiercing() const;
    const uint32_t getProjectileArea() const;
    const uint32_t getKills() const;
    const bool isUpgraded() const;
    const uint32_t getIncreasedArmor() const;
    const uint32_t getIncreasedReg() const;
    const uint32_t getIncreasedAttack() const;
    const uint32_t getIncreasedGold() const;
    const uint32_t getIncreasedTargets() const;
    const uint32_t getGoldReward() const;
    const uint32_t getAttackLimit() const;
    const float getTimeSlowdown() const;
    const bool isRegenerating() const;
    const bool isLeveling() const;
    const bool isSprinting() const;
    const bool hasSpawned() const;
    const bool isSoundPlayed() const;
    const bool isAbilityActive() const;
    const float getAbilityCooldown() const;
    const float getAbilityTime() const;
    const float getAbilityTotalMaxTime() const;
    const float getAbilityMaxTime() const;
    const float getAbilityMaxTimeModifier() const;
    const uint32_t getAttackIncrease() const;

    void setWave(uint32_t t_wave);
    void setGold(uint32_t t_gold);
    void setArmor(uint32_t t_armor);
    void setReg(uint32_t t_reg);
    void setSprint(float t_sprint);
    void setMaxSprint(uint32_t t_maxSprint);
    void setCriticalChance(uint32_t t_criticalChance);
    void setProjectileAttack(uint32_t t_projectileAttack);
    void setProjectilePiercing(uint32_t t_projectilePiercing);
    void setProjectileArea(uint32_t t_projectileArea);
    void setIncreasedArmor(uint32_t t_increasedArmor);
    void setIncreasedReg(uint32_t t_increasedReg);
    void setIncreasedAttack(uint32_t t_increasedAttack);
    void setIncreasedGold(uint32_t t_increasedGold);
    void setIncreasedTargets(uint32_t t_increasedTargets);
    void setAttackLimit(uint32_t t_attackLimit);
    void setTimeSlowdown(float t_timeSlowdown);
    void setKills(uint32_t t_kills);
    void setUpgraded(bool t_upgraded);
    void setRegenerating(bool t_regenerating);
    void setLeveling(bool t_leveling);
    void setPendingXP(uint32_t t_pendingXP);
    void setSprinting(bool t_sprinting);
    void setAbilityActive(bool t_abilityActive);
    void setAbilityCooldown(bool t_abilityCooldown);
    void setAbilityMaxTime(float t_abilityMaxTime);
    void setAbilityMaxTimeModifier(float t_abilityMaxTimeModifier);
    void setAttackIncrease(uint32_t t_attackIncrease);
    void setPlayedSound(bool t_soundPlayed);

    void controls(const std::unordered_map<std::string, int> &keybinds,
                  float dt);
    void whooshSound(SoundEngine &soundEngine);
    const bool levelUp();
    const bool isHPRegenerating(float dt);
    const bool isAbilityActivated();
    void abilityCounter(float dt);
    virtual void setAbilityTexture() = 0;
    virtual void endAbility() = 0;
    virtual void doAbility(SoundEngine &soundEngine) = 0;
    void spawn(float dt);
    void upgrade(const std::string &t_name, sf::IntRect &intRect);
    virtual void upgradeAttributes(const std::string &t_name,
                                   sf::IntRect &intRect) = 0;
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

    uint32_t wave;
    uint32_t gold;
    uint32_t armor;
    uint32_t reg;
    uint32_t level;
    uint32_t maxXP;
    uint32_t pendingXP;
    float sprint;
    uint32_t maxSprint;
    uint32_t criticalChance;
    uint32_t projectileAttack;
    uint32_t projectilePiercing;
    uint32_t projectileArea;
    uint32_t kills;
    bool upgraded;
    uint32_t increasedArmor;
    uint32_t increasedReg;
    uint32_t increasedAttack;
    uint32_t increasedGold;
    uint32_t goldReward;
    uint32_t attackLimit;
    uint32_t increasedTargets;
    float timeSlowdown;
    bool regenerating;
    float regCooldown;
    bool leveling;
    bool sprinting;
    bool spawned;
    float spawnCountdown;

    std::unique_ptr<AbilityComponent> abilityComponent;

    bool soundPlayed;
};
