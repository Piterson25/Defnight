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
    this->abilityUpgrades[t_name].priceText->setText(
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
                                          uint32_t price, uint32_t boughtNumber,
                                          uint32_t maxNumber)
{
    float offsetY = 0.f;

    if (maxNumber > 0) {
        offsetY = 12.f;
    }

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
            std::make_unique<gui::Text>(
                desc, calcChar(16, vm), t_x + calcX(166, vm),
                t_y - calcY(2, vm) - calcY(offsetY, vm), gui::WHITE, true),
            std::make_unique<gui::Text>(
                value, calcChar(16, vm), t_x + calcX(166, vm),
                t_y + calcY(20, vm) - calcY(offsetY, vm), gui::WHITE, true),
            std::make_unique<gui::Sprite>(
                this->attributesTexture, t_x + calcX(132, vm),
                t_y + calcY(38, vm) - calcY(offsetY, vm), calcScale(2, vm),
                false),
            std::make_unique<gui::Text>(
                std::to_string(price), calcChar(16, vm), t_x + calcX(166, vm),
                t_y + calcY(48, vm) - calcY(offsetY, vm), gui::GOLD, false),
            std::vector<std::unique_ptr<gui::Sprite>>(),
            boughtNumber,
            maxNumber,
            price,
            false,
        });

    this->abilityUpgrades[t_name].sprite->setTextureRect(
        sf::IntRect(16 * iconID, 0, 16, 16));

    this->abilityUpgrades[t_name].button->setColor(gui::DARK_RED);

    this->abilityUpgrades[t_name].coinSprite->setTextureRect(
        sf::IntRect(0, 0, 16, 16));

    if (maxNumber > 0) {
        sf::Texture texture;
        texture.loadFromFile("assets/textures/progress_segment.png");
        const sf::Vector2f position =
            this->abilityUpgrades[t_name].sprite->getPosition();
        for (uint32_t i = 0; i < maxNumber; i++) {
            this->abilityUpgrades[t_name].segments.push_back(
                std::make_unique<gui::Sprite>(
                    texture,
                    position.x + calcX(static_cast<float>(i) * 12.f + 108, vm),
                    position.y + calcY(74, vm) - calcY(offsetY, vm),
                    calcScale(2, vm), false));
            this->abilityUpgrades[t_name].segments[i]->setTextureRect(
                sf::IntRect(4, 0, 4, 8));

            if (i < boughtNumber) {
                this->abilityUpgrades[t_name].segments[i]->setTextureRect(
                    sf::IntRect(0, 0, 4, 8));
            }
        }
    }
}

void AbilityUpgradeGUI::updateSegments(const std::string &t_name)
{
    this->abilityUpgrades[t_name]
        .segments[this->abilityUpgrades[t_name].boughtNumber]
        ->setTextureRect(sf::IntRect(0, 0, 4, 8));
    this->abilityUpgrades[t_name].boughtNumber++;

    if (this->abilityUpgrades[t_name].boughtNumber ==
        this->abilityUpgrades[t_name].maxNumber) {
        lockUpgrade(t_name);
    }
}

const bool AbilityUpgradeGUI::hasBoughtUpgrade(
    const sf::Vector2i &mousePos, const std::string &t_name,
    FloatingTextSystem *floatingTextSystem, SoundEngine *soundEngine)
{
    if (this->abilityUpgrades.find(t_name) == this->abilityUpgrades.end() ||
        this->abilityUpgrades[t_name].locked ||
        this->abilityUpgrades[t_name].button->getColor() == gui::DARK_RED) {
        return false;
    }

    if (player.getGold() >= getPrice(t_name)) {
        if (this->abilityUpgrades[t_name].button->isPressed(mousePos)) {
            buy(t_name, floatingTextSystem);
            player.setBoughtItems(player.getBoughtItems() + 1);
            soundEngine->addSound("buy");
            return true;
        }
    }

    return false;
}

void AbilityUpgradeGUI::lockUpgrade(const std::string &t_name)
{
    this->abilityUpgrades[t_name].locked = true;
    this->abilityUpgrades[t_name].button->setColor(gui::DARK_RED);
}

void AbilityUpgradeGUI::unlockUpgrade(const std::string &t_name)
{
    this->abilityUpgrades[t_name].locked = false;
}

void AbilityUpgradeGUI::disableUpgrade(const std::string &t_name)
{
    this->abilityUpgrades[t_name].button->setColor(gui::DARK_RED);
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
    for (const auto &pair : abilityUpgrades) {
        if (!pair.second.locked && !player.isAbilityActive() &&
            player.getGold() >= pair.second.price) {
            pair.second.button->setColor(gui::DARK_GREEN);
        }
        else {
            pair.second.button->setColor(gui::DARK_RED);
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
    else if (t_name == "SLOWDOWN") {
        const float slowdown = player.getTimeSlowdown() * 100.f;

        text += std::format("{:g}", slowdown);

        text += "%";
        playerStats[t_name]->setText(text);
    }
    else if (t_name == "GOLD") {
        playerStats[t_name]->setText(text +
                                     std::to_string(player.getIncreasedGold()));
    }
    else if (t_name == "TARGETS") {
        playerStats[t_name]->setText(text +
                                     std::to_string(player.getAttackLimit()));
    }
}

void AbilityUpgradeGUI::draw(sf::RenderTarget &target)
{
    for (const auto &stats : playerStats) {
        stats.second->draw(target);
    }

    for (const auto &pair : abilityUpgrades) {
        pair.second.sprite->draw(target);
        if (pair.second.locked) {
            pair.second.lockSprite->draw(target);
        }
        pair.second.button->draw(target);
        pair.second.desc->draw(target);
        pair.second.effect->draw(target);
        pair.second.coinSprite->draw(target);
        pair.second.priceText->draw(target);
        for (const auto &segment : pair.second.segments) {
            segment->draw(target);
        }
    }
}
