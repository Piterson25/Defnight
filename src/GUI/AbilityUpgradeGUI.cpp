#include "AbilityUpgradeGUI.hpp"

AbilityUpgradeGUI::AbilityUpgradeGUI(sf::VideoMode &t_vm, Player &t_player)
    : vm(t_vm), player(t_player)
{
    this->abilitySprite =
        new gui::Sprite("assets/textures/abilities_icons.png", calcX(80, vm),
                        calcY(160, vm), calcScale(8, vm), false);
    this->abilitySprite->setTextureRect(sf::IntRect(0, 0, 16, 16));
    this->playerCooldown =
        new gui::Text("Cooldown: 0s", calcChar(16, vm), calcX(16, vm),
                      calcY(320, vm), sf::Color(255, 255, 255, 255), false);
    this->abilityUpgradesTexture.loadFromFile(
        "assets/textures/abilities_upgrades.png");
    this->selectsTexture.loadFromFile("assets/textures/select.png");
    this->attributesTexture.loadFromFile(
        "assets/textures/attributes_icons.png");
}

AbilityUpgradeGUI::~AbilityUpgradeGUI()
{
    delete this->abilitySprite;
    delete this->playerCooldown;
    for (auto &pair : abilityUpgrades) {
        delete pair.second.itemSprite;
        delete pair.second.itemFrame;
        delete pair.second.itemButton;
        delete pair.second.itemDesc;
        delete pair.second.itemValue;
        delete pair.second.itemCoin;
        delete pair.second.itemPrice;
    }
}

const uint32_t AbilityUpgradeGUI::getPrice(const std::string &t_name)
{
    return this->abilityUpgrades[t_name].price;
}

void AbilityUpgradeGUI::increasePrice(const std::string &t_name)
{
    this->abilityUpgrades[t_name].price += static_cast<uint32_t>(
        (((1 + sqrtf(5)) / 2.f) - 1) * this->abilityUpgrades[t_name].price);
    this->abilityUpgrades[t_name].itemPrice->setText(
        std::to_string(getPrice(t_name)));
}

void AbilityUpgradeGUI::addAbilityUpgrade(const std::string &t_name, float t_x,
                                          float t_y, uint32_t iconID,
                                          const std::string &desc,
                                          const std::string &value,
                                          uint32_t price)
{
    this->abilityUpgrades.emplace(
        t_name,
        BuyItem{
            new gui::Sprite(this->abilityUpgradesTexture, t_x, t_y,
                            calcScale(4, vm), false),
            new gui::Sprite(this->selectsTexture, t_x - calcX(12, vm),
                            t_y - calcY(12, vm), calcScale(1, vm), false),
            new gui::ButtonSprite(this->selectsTexture, t_x - calcX(12, vm),
                                  t_y - calcY(12, vm), calcScale(1, vm), false),
            new gui::Text(desc, calcChar(16, vm), t_x + calcX(160, vm),
                          t_y - calcY(2, vm), sf::Color(255, 255, 255), true),
            new gui::Text(value, calcChar(16, vm), t_x + calcX(160, vm),
                          t_y + calcY(20, vm), sf::Color(255, 255, 255), true),
            new gui::Sprite(this->attributesTexture, t_x + calcX(126, vm),
                            t_y + calcY(38, vm), calcScale(2, vm), false),
            new gui::Text(std::to_string(price), calcChar(16, vm),
                          t_x + calcX(160, vm), t_y + calcY(48, vm),
                          sf::Color(255, 246, 76), false),
            price,
        });

    this->abilityUpgrades[t_name].itemSprite->setTextureRect(
        sf::IntRect(16 * iconID, 0, 16, 16));

    this->abilityUpgrades[t_name].itemFrame->setTextureRect(
        sf::IntRect(176, 0, 88, 88));

    this->abilityUpgrades[t_name].itemButton->setTextureRect(
        sf::IntRect(0, 0, 88, 88));

    this->abilityUpgrades[t_name].itemCoin->setTextureRect(
        sf::IntRect(0, 0, 16, 16));
}

void AbilityUpgradeGUI::setAbility(const sf::IntRect &intRect)
{
    this->abilitySprite->setTextureRect(intRect);
}

const bool AbilityUpgradeGUI::isPressed(const std::string &t_name,
                                        bool mouseClicked)
{
    return this->abilityUpgrades[t_name].itemButton->isPressed() &&
           !mouseClicked;
}

void AbilityUpgradeGUI::buy(const std::string &t_name,
                            FloatingTextSystem *floatingTextSystem)
{
    player.setGold(player.getGold() - getPrice(t_name));
    floatingTextSystem->addFloatingText(
        "-" + std::to_string(getPrice(t_name)), calcChar(16, vm), calcX(20, vm),
        calcX(96, vm), sf::Color(255, 246, 76), true);
    increasePrice(t_name);
}

void AbilityUpgradeGUI::updateItemFrames()
{
    for (auto &pair : abilityUpgrades) {
        pair.second.itemButton->setTransparent();

        if (player.getGold() >= pair.second.price) {
            pair.second.itemFrame->setTextureRect(sf::IntRect(264, 0, 88, 88));
        }
        else {
            pair.second.itemFrame->setTextureRect(sf::IntRect(176, 0, 88, 88));
        }
    }
}

void AbilityUpgradeGUI::updatePlayerInfo()
{
    float maxTime =
        player.getAbilityMaxTime() * player.getAbilityMaxTimeModifier();
    std::string cooldownText = "Cooldown: ";

    cooldownText += std::format("{:g}", maxTime);

    cooldownText += "s";
    this->playerCooldown->setText(cooldownText);
}

void AbilityUpgradeGUI::update(const std::string &t_name,
                               const sf::Vector2i &mousePos)
{
    this->abilityUpgrades[t_name].itemButton->update(mousePos);
}

void AbilityUpgradeGUI::draw(sf::RenderTarget &target)
{
    this->abilitySprite->draw(target);
    this->playerCooldown->draw(target);
    for (const auto &pair : abilityUpgrades) {
        BuyItem item = pair.second;

        item.itemSprite->draw(target);
        item.itemFrame->draw(target);
        item.itemButton->draw(target);
        item.itemDesc->draw(target);
        item.itemValue->draw(target);
        item.itemCoin->draw(target);
        item.itemPrice->draw(target);
    }
}
