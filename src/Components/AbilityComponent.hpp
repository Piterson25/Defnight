#pragma once

#include "Utils/Functions.hpp"

class AbilityComponent {
public:
    AbilityComponent(sf::VideoMode &t_vm);
    ~AbilityComponent();

    const bool isAbilityActive() const;
    const float getAbilityCooldown() const;
    const float getAbilityTime() const;
    const float getAbilityMaxTime() const;
    const float getAbilityMaxTimeModifier() const;
    const float getAttackModifier() const;
    const float getArmorModifier() const;
    const float getSpeedModifier() const;

    void setAbilityStats(float t_abilityMaxTime, float t_abilityCooldown,
                         float t_abilityTime);
    void setAbilityActive(bool t_abilityActive);
    void setAbilityCooldown(float t_abilityCooldown);
    void setAbilityTime(float t_abilityTime);
    void setAbilityMaxTime(float t_abilityMaxTime);
    void setAbilityMaxTimeModifier(float t_abilityMaxTimeModifier);
    void setAttackModifier(float t_attackModifier);
    void setArmorModifier(float t_armorModifier);
    void setSpeedModifier(float t_speedModifier);

    void abilityCounter(float dt);
    void endAbility();

private:
    sf::VideoMode &vm;

    bool abilityActive;
    float abilityCooldown;
    float abilityTime;
    float abilityMaxTime;
    float abilityMaxTimeModifier;

    float attackModifier;
    float armorModifier;
    float speedModifier;
};
