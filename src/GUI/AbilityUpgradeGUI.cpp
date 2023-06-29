#include "AbilityUpgradeGUI.hpp"

AbilityUpgradeGUI::AbilityUpgradeGUI(sf::VideoMode &t_vm, Player &t_player)
    : vm(t_vm), player(t_player)
{
    this->abilitySprite =
        new gui::Sprite("assets/textures/abilities_icons.png", calcX(80, vm),
                        calcY(160, vm), calcScale(8, vm), false);
    this->abilitySprite->setTextureRect(sf::IntRect(0, 0, 16, 16));
}

AbilityUpgradeGUI::~AbilityUpgradeGUI()
{
    delete this->abilitySprite;
}

void AbilityUpgradeGUI::setAbility(const sf::IntRect &intRect)
{
    this->abilitySprite->setTextureRect(intRect);
}

void AbilityUpgradeGUI::draw(sf::RenderTarget &target)
{
    this->abilitySprite->draw(target);
}
