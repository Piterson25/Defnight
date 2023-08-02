#include "AbilityUpgradeGUI.hpp"

AbilityUpgradeGUI::AbilityUpgradeGUI(sf::VideoMode &t_vm, Player &t_player)
    : vm(t_vm), player(t_player)
{
    this->playerStats["COOLDOWN"] = std::make_unique<gui::Text>(
        "Cooldown: 0s", calcChar(16, vm), calcX(32, vm), calcY(180, vm),
        sf::Color(255, 255, 255, 255), false);
    this->abilityUpgradesTexture.loadFromFile(
        "assets/textures/abilities_upgrades.png");
    this->attributesTexture.loadFromFile(
        "assets/textures/attributes_icons.png");
}

AbilityUpgradeGUI::~AbilityUpgradeGUI()
{
    this->abilityUpgrades.clear();
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

void AbilityUpgradeGUI::addPlayerStat(const std::string &t_name, float t_x,
                                      float t_y, const std::string &desc)
{
    this->playerStats[t_name] = std::make_unique<gui::Text>(
        desc, calcChar(16, vm), t_x, t_y, gui::WHITE, false);
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
            std::make_unique<gui::Sprite>(this->abilityUpgradesTexture, t_x,
                                          t_y, calcScale(4, vm), false),
            std::make_unique<gui::ButtonSprite>(
                gui::RECT_BUTTON, t_x - calcX(12, vm), t_y - calcY(12, vm),
                calcScale(1, vm), false),
            std::make_unique<gui::Sprite>("assets/textures/lock.png", t_x, t_y,
                                          calcScale(4, vm), false),
            std::make_unique<gui::Text>(desc, calcChar(16, vm),
                                        t_x + calcX(166, vm),
                                        t_y - calcY(2, vm), gui::WHITE, true),
            std::make_unique<gui::Text>(value, calcChar(16, vm),
                                        t_x + calcX(166, vm),
                                        t_y + calcY(20, vm), gui::WHITE, true),
            std::make_unique<gui::Sprite>(
                this->attributesTexture, t_x + calcX(132, vm),
                t_y + calcY(38, vm), calcScale(2, vm), false),
            std::make_unique<gui::Text>(std::to_string(price), calcChar(16, vm),
                                        t_x + calcX(166, vm),
                                        t_y + calcY(48, vm), gui::GOLD, false),
            price,
            false,
        });

    this->abilityUpgrades[t_name].itemSprite->setTextureRect(
        sf::IntRect(16 * iconID, 0, 16, 16));

    this->abilityUpgrades[t_name].itemButton->setColor(gui::DARK_RED);

    this->abilityUpgrades[t_name].itemCoin->setTextureRect(
        sf::IntRect(0, 0, 16, 16));
}

const bool AbilityUpgradeGUI::isPressed(const std::string &t_name,
                                        bool mouseClicked)
{
    return this->abilityUpgrades[t_name].itemButton->isPressed() &&
           !mouseClicked;
}

const bool AbilityUpgradeGUI::hasBoughtUpgrade(
    const sf::Vector2i &mousePos, bool mouseClicked, const std::string &t_name,
    FloatingTextSystem *floatingTextSystem, SoundEngine *soundEngine)
{
    if (this->abilityUpgrades.find(t_name) == this->abilityUpgrades.end() ||
        this->abilityUpgrades[t_name].locked ||
        this->abilityUpgrades[t_name].itemButton->getColor() == gui::DARK_RED) {
        return false;
    }

    if (!player.isAbilityActive() && player.getGold() >= getPrice(t_name)) {
        update(t_name, mousePos);
        if (isPressed(t_name, mouseClicked)) {
            buy(t_name, floatingTextSystem);
            soundEngine->addSound("buy");
            return true;
        }
    }

    return false;
}

void AbilityUpgradeGUI::lockUpgrade(const std::string t_name)
{
    this->abilityUpgrades[t_name].locked = true;
    this->abilityUpgrades[t_name].itemButton->setColor(gui::DARK_RED);
}

void AbilityUpgradeGUI::unlockUpgrade(const std::string t_name)
{
    this->abilityUpgrades[t_name].locked = false;
}

void AbilityUpgradeGUI::disableUpgrade(const std::string t_name)
{
    this->abilityUpgrades[t_name].itemButton->setColor(gui::DARK_RED);
}

void AbilityUpgradeGUI::buy(const std::string &t_name,
                            FloatingTextSystem *floatingTextSystem)
{
    player.setGold(player.getGold() - getPrice(t_name));
    floatingTextSystem->addFloatingText(
        gui::GOLD, "-" + std::to_string(getPrice(t_name)), calcChar(16, vm),
        calcX(20, vm), calcX(96, vm), true);
    increasePrice(t_name);
}

void AbilityUpgradeGUI::updateItemFrames()
{
    for (auto &pair : abilityUpgrades) {
        if (!pair.second.locked && !player.isAbilityActive() &&
            player.getGold() >= pair.second.price) {
            pair.second.itemButton->setColor(gui::GREEN);
        }
        else {
            pair.second.itemButton->setColor(gui::DARK_RED);
        }
    }
}

void AbilityUpgradeGUI::updatePlayerInfo(const std::string &t_name,
                                         const std::string &t_desc)
{
    if (this->playerStats.find(t_name) == this->playerStats.end()) {
        return;
    }

    std::string text = t_desc + ": ";

    if (t_name == "COOLDOWN") {
        const float maxTime = player.getAbilityTotalMaxTime();

        text += std::format("{:g}", maxTime);

        text += "s";
        playerStats[t_name]->setText(text);
    }
    else if (t_name == "PROJ_ATTACK") {
        playerStats[t_name]->setText(
            text + std::to_string(player.getProjectileAttack()));
    }
    else if (t_name == "PIERCING") {
        playerStats[t_name]->setText(
            text + std::to_string(player.getProjectilePiercing()));
    }
    else if (t_name == "AREA") {
        playerStats[t_name]->setText(
            text + std::to_string(player.getProjectileArea()));
    }
    else if (t_name == "ARMOR") {
        playerStats[t_name]->setText(
            text + std::to_string(player.getIncreasedArmor()));
    }
    else if (t_name == "ATTACK") {
        playerStats[t_name]->setText(
            text + std::to_string(player.getIncreasedAttack()));
    }
    else if (t_name == "REG") {
        playerStats[t_name]->setText(text +
                                     std::to_string(player.getIncreasedReg()));
    }
}

void AbilityUpgradeGUI::update(const std::string &t_name,
                               const sf::Vector2i &mousePos)
{
    this->abilityUpgrades[t_name].itemButton->update(mousePos);
}

void AbilityUpgradeGUI::draw(sf::RenderTarget &target)
{
    for (const auto &stats : playerStats) {
        stats.second->draw(target);
    }

    for (const auto &pair : abilityUpgrades) {
        pair.second.itemSprite->draw(target);
        if (pair.second.locked) {
            pair.second.itemLock->draw(target);
        }
        pair.second.itemButton->draw(target);
        pair.second.itemDesc->draw(target);
        pair.second.itemValue->draw(target);
        pair.second.itemCoin->draw(target);
        pair.second.itemPrice->draw(target);
    }
}
