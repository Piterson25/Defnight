#include "AbilityComponent.hpp"

AbilityComponent::AbilityComponent(sf::VideoMode &t_vm) : vm(t_vm)
{
    this->abilityActive = false;
    this->abilityCooldown = 0.f;
    this->abilityTime = 0.f;
    this->abilityMaxTime = 0.f;
    this->abilityMaxTimeModifier = 1.f;
    this->attackModifier = 1.f;
    this->armorModifier = 1.f;
    this->speedModifier = 1.f;
}

AbilityComponent::~AbilityComponent()
{
}

const bool AbilityComponent::isAbilityActive() const
{
    return this->abilityActive;
}

const float AbilityComponent::getAbilityCooldown() const
{
    return this->abilityCooldown;
}

const float AbilityComponent::getAbilityTime() const
{
    return this->abilityTime;
}

const float AbilityComponent::getAbilityMaxTime() const
{
    return this->abilityMaxTime;
}

const float AbilityComponent::getAbilityMaxTimeModifier() const
{
    return this->abilityMaxTimeModifier;
}

const float AbilityComponent::getAttackModifier() const
{
    return this->attackModifier;
}

const float AbilityComponent::getArmorModifier() const
{
    return this->armorModifier;
}

const float AbilityComponent::getSpeedModifier() const
{
    return this->speedModifier;
}

void AbilityComponent::setAbilityStats(float t_abilityMaxTime,
                                       float t_abilityCooldown,
                                       float t_abilityTime)
{
    this->abilityMaxTime = t_abilityMaxTime;
    this->abilityCooldown = t_abilityCooldown;
    this->abilityTime = t_abilityTime;
}

void AbilityComponent::setAbilityActive(bool t_abilityActive)
{
    this->abilityActive = t_abilityActive;
}

void AbilityComponent::setAbilityCooldown(float t_abilityCooldown)
{
    this->abilityCooldown = t_abilityCooldown;
}

void AbilityComponent::setAbilityTime(float t_abilityTime)
{
    this->abilityTime = t_abilityTime;
}

void AbilityComponent::setAbilityMaxTime(float t_abilityMaxTime)
{
    this->abilityMaxTime = t_abilityMaxTime;
}

void AbilityComponent::setAbilityMaxTimeModifier(float t_abilityMaxTimeModifier)
{
    this->abilityMaxTimeModifier = t_abilityMaxTimeModifier;
}

void AbilityComponent::setAttackModifier(float t_attackModifier)
{
    this->attackModifier = t_attackModifier;
}

void AbilityComponent::setArmorModifier(float t_armorModifier)
{
    this->armorModifier = t_armorModifier;
}

void AbilityComponent::setSpeedModifier(float t_speedModifier)
{
    this->speedModifier = t_speedModifier;
}

void AbilityComponent::abilityCounter(float dt)
{
    const float maxTime = this->getAbilityMaxTime();
    if (maxTime > 0.f) {
        if (this->abilityCooldown < maxTime) {
            this->abilityCooldown += dt;
        }

        if (this->abilityCooldown >= this->abilityTime && this->abilityActive) {
            this->abilityActive = false;
        }
        else if (this->abilityCooldown > maxTime) {
            this->abilityCooldown = maxTime;
        }
    }
}
