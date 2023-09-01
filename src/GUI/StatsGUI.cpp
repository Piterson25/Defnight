#include "StatsGUI.hpp"

StatsGUI::StatsGUI(sf::VideoMode &t_vm, Player &t_player,
                   std::unordered_map<std::string, std::string> &t_lang)
    : vm(t_vm), player(t_player)
{
    this->attributesTexture.loadFromFile(
        "assets/textures/attributes_icons.png");
    this->upgradesTexture.loadFromFile("assets/textures/upgrades_icons.png");
    this->abilitiesTexture.loadFromFile("assets/textures/abilities_icons.png");

    this->sprites["TOP_GUI"] = std::make_unique<gui::Sprite>(
        "assets/textures/top_gui.png", 0.f, 0.f, calcScale(1, vm), false);

    this->sprite_buttons["MENU"] = std::make_unique<gui::ButtonSprite>(
        gui::RECT_BURGER, calcX(4, vm), calcY(4, vm), calcScale(2, vm), false);

    this->sprites["PROGRESS_BAR"] = std::make_unique<gui::Sprite>(
        "assets/textures/progress_bar.png", calcX(640, vm), calcY(44, vm),
        calcScale(1, vm), true);
    this->sprites["PROGRESS_BAR"]->setColor(sf::Color::Transparent);

    this->sprites["MINIATURE"] =
        std::make_unique<gui::Sprite>(this->upgradesTexture, calcX(156, vm),
                                      calcX(16, vm), calcScale(4, vm), false);
    this->sprites["MINIATURE"]->setTextureRect(sf::IntRect(0, 0, 16, 16));
    this->texts["NAME"] = std::make_unique<gui::Text>(
        t_lang[player.getName()], calcChar(16, vm), calcX(188, vm),
        calcY(98, vm), gui::WHITE, true);

    this->sprites["GOLD"] = std::make_unique<gui::Sprite>(
        attributesTexture, 0.f, calcY(88, vm), calcScale(2, vm), false);
    this->sprites["GOLD"]->setTextureRect(sf::IntRect(0, 0, 16, 16));
    this->texts["GOLD"] = std::make_unique<gui::Text>(
        std::to_string(player.getGold()), calcChar(16, vm), calcX(36, vm),
        calcY(98, vm), gui::GOLD, false);

    this->sprites["ARMOR"] =
        std::make_unique<gui::Sprite>(attributesTexture, calcX(376, vm),
                                      calcY(16, vm), calcScale(2, vm), false);
    this->sprites["ARMOR"]->setTextureRect(sf::IntRect(16, 0, 16, 16));
    this->texts["ARMOR"] = std::make_unique<gui::Text>(
        std::to_string(player.getArmor()), calcChar(16, vm), calcX(392, vm),
        calcY(70, vm), gui::LIGHT_GREY, true);

    this->sprites["REG"] =
        std::make_unique<gui::Sprite>(attributesTexture, calcX(440, vm),
                                      calcY(16, vm), calcScale(2, vm), false);
    this->sprites["REG"]->setTextureRect(sf::IntRect(32, 0, 16, 16));
    this->texts["REG"] = std::make_unique<gui::Text>(
        std::to_string(player.getReg()), calcChar(16, vm), calcX(456, vm),
        calcY(70, vm), gui::RED, true);

    this->sprites["ATTACK"] =
        std::make_unique<gui::Sprite>(attributesTexture, calcX(808, vm),
                                      calcY(16, vm), calcScale(2, vm), false);
    this->sprites["ATTACK"]->setTextureRect(sf::IntRect(80, 0, 16, 16));
    this->texts["ATTACK"] = std::make_unique<gui::Text>(
        std::to_string(player.getAttack()), calcChar(16, vm), calcX(824, vm),
        calcY(70, vm), gui::LIGHT_GREY, true);

    this->sprites["ATTACK_SPEED"] =
        std::make_unique<gui::Sprite>(attributesTexture, calcX(872, vm),
                                      calcY(16, vm), calcScale(2, vm), false);
    this->sprites["ATTACK_SPEED"]->setTextureRect(sf::IntRect(96, 0, 16, 16));
    this->texts["ATTACK_SPEED"] = std::make_unique<gui::Text>(
        std::to_string(player.getAttackSpeed()), calcChar(16, vm),
        calcX(888, vm), calcY(70, vm), gui::LIGHT_GREY, true);

    this->sprites["SPEED"] =
        std::make_unique<gui::Sprite>(attributesTexture, calcX(936, vm),
                                      calcY(16, vm), calcScale(2, vm), false);
    this->sprites["SPEED"]->setTextureRect(sf::IntRect(112, 0, 16, 16));
    this->texts["SPEED"] = std::make_unique<gui::Text>(
        std::to_string(player.getSpeed()), calcChar(16, vm), calcX(952, vm),
        calcY(70, vm), gui::LIGHT_GREY, true);

    this->sprites["CRITICAL"] =
        std::make_unique<gui::Sprite>(attributesTexture, calcX(1000, vm),
                                      calcY(16, vm), calcScale(2, vm), false);
    this->sprites["CRITICAL"]->setTextureRect(sf::IntRect(128, 0, 16, 16));
    this->texts["CRITICAL"] = std::make_unique<gui::Text>(
        std::to_string(player.getCriticalChance()) + "%", calcChar(16, vm),
        calcX(1016, vm), calcY(70, vm), gui::LIGHT_GREY, true);

    this->sprites["XP_BAR"] = std::make_unique<gui::Sprite>(
        "assets/textures/bars.png", calcX(512, vm), calcY(12, vm),
        calcScale(1, vm), false);
    this->sprites["XP_BAR"]->setTextureRect(sf::IntRect(0, 0, 0, 0));
    this->texts["XP"] = std::make_unique<gui::Text>(
        "XP:" + std::to_string(player.getXP()) + "/" +
            std::to_string(player.getMaxXP()),
        calcChar(16, vm), calcX(640, vm), calcY(15, vm), gui::WHITE, true);
    this->texts["LEVEL"] = std::make_unique<gui::Text>(
        "Level " + std::to_string(player.getLevel()), calcChar(16, vm),
        calcX(640, vm), calcY(15, vm), gui::WHITE, true);
    this->levelShown = true;
    this->xpBarPercent = 0.f;

    this->sprites["HP_BAR"] = std::make_unique<gui::Sprite>(
        "assets/textures/bars.png", calcX(512, vm), calcY(52, vm),
        calcScale(1, vm), false);
    this->sprites["HP_BAR"]->setTextureRect(sf::IntRect(0, 20, 256, 20));
    this->texts["HP"] = std::make_unique<gui::Text>(
        "HP:" + std::to_string(player.getHP()) + "/" +
            std::to_string(player.getMaxHP()),
        calcChar(16, vm), calcX(640, vm), calcY(55, vm), gui::WHITE, true);
    this->hpBarPercent = 1.f;

    this->sprites["SPRINT_BAR"] = std::make_unique<gui::Sprite>(
        "assets/textures/bars.png", calcX(512, vm), calcY(92, vm),
        calcScale(1, vm), false);
    this->sprites["SPRINT_BAR"]->setTextureRect(sf::IntRect(0, 40, 256, 20));
    this->texts["SPRINT"] = std::make_unique<gui::Text>(
        std::to_string(player.getSprint()) + "/" +
            std::to_string(player.getMaxSprint()),
        calcChar(16, vm), calcX(640, vm), calcY(95, vm), gui::WHITE, true);

    this->texts["MONSTER_COUNT"] = std::make_unique<gui::Text>(
        t_lang["MONSTER_COUNT"], calcChar(16, vm), calcX(906, vm),
        calcY(98, vm), sf::Color(130, 139, 152), false);
    this->texts["MONSTER_COUNT"]->setPosition(
        sf::Vector2f(calcX(1218, vm) - this->texts["MONSTER_COUNT"]->getWidth(),
                     calcY(98, vm)));

    this->texts["WAVE_NUMBER"] = std::make_unique<gui::Text>(
        t_lang["WAVE"], calcChar(32, vm), calcX(1060, vm), calcY(36, vm),
        gui::FLAMINGO, false);

    this->sprites["SHOP_ICON"] =
        std::make_unique<gui::Sprite>("assets/textures/shop.png", calcX(44, vm),
                                      calcY(16, vm), calcScale(4, vm), false);
    this->sprite_buttons["SHOP"] = std::make_unique<gui::ButtonSprite>(
        gui::RECT_BUTTON, calcX(32, vm), calcY(4, vm), calcScale(1, vm), false);

    this->texts["WAVE_COUNTDOWN"] = std::make_unique<gui::Text>(
        t_lang["NEXT_WAVE"], calcChar(16, vm), calcX(954, vm), calcY(98, vm),
        gui::GOLD, false);
    this->texts["WAVE_COUNTDOWN"]->setPosition(sf::Vector2f(
        calcX(1218, vm) - this->texts["WAVE_COUNTDOWN"]->getWidth(),
        calcY(98, vm)));

    this->sprites["ABILITY_ICON"] = std::make_unique<gui::Sprite>(
        "assets/textures/abilities_icons.png", calcX(268, vm), calcY(16, vm),
        calcScale(4, vm), false);
    this->sprites["ABILITY_ICON"]->setTextureRect(sf::IntRect(0, 0, 16, 16));

    this->sprite_buttons["ABILITY_UPGRADE"] =
        std::make_unique<gui::ButtonSprite>(gui::RECT_BUTTON, calcX(256, vm),
                                            calcY(4, vm), calcScale(1, vm),
                                            false);
    this->sprite_buttons["ABILITY_UPGRADE"]->setColor(gui::GOLD);

    this->abilityIcon.setFillColor(sf::Color(128, 128, 128, 128));
    this->abilityIcon.setSize(sf::Vector2f(calcX(80, vm), calcY(80, vm)));
    this->abilityIcon.setPosition(sf::Vector2f(calcX(260, vm), calcY(8, vm)));
}

StatsGUI::~StatsGUI() = default;

void StatsGUI::updateGold()
{
    this->texts["GOLD"]->setText(std::to_string(player.getGold()));
}

void StatsGUI::updateArmor()
{
    this->texts["ARMOR"]->setText(std::to_string(player.getArmor()));
    this->texts["ARMOR"]->center(calcX(392, vm));

    if (this->player.isAbilityActive() &&
        this->player.getIncreasedArmor() > 0) {
        this->texts["ARMOR"]->setFillColor(gui::GOLD);
    }
    else {
        this->texts["ARMOR"]->setFillColor(gui::LIGHT_GREY);
    }
}

void StatsGUI::updateReg()
{
    this->texts["REG"]->setText(std::to_string(player.getReg()));
    this->texts["REG"]->center(calcX(456, vm));

    if (this->player.isAbilityActive() && this->player.getIncreasedReg() > 0) {
        this->texts["REG"]->setFillColor(gui::GOLD);
    }
    else {
        this->texts["REG"]->setFillColor(gui::RED);
    }
}

void StatsGUI::updateAttack()
{
    this->texts["ATTACK"]->setText(std::to_string(player.getAttack()));
    this->texts["ATTACK"]->center(calcX(824, vm));

    if (this->player.isAbilityActive() &&
        this->player.getIncreasedAttack() > 0) {
        this->texts["ATTACK"]->setFillColor(gui::GOLD);
    }
    else {
        this->texts["ATTACK"]->setFillColor(gui::LIGHT_GREY);
    }
}

void StatsGUI::updateAttackSpeed()
{
    this->texts["ATTACK_SPEED"]->setText(
        std::to_string(player.getAttackSpeed()));
    this->texts["ATTACK_SPEED"]->center(calcX(888, vm));
}

void StatsGUI::updateSpeed()
{
    this->texts["SPEED"]->setText(std::to_string(player.getSpeed()));
    this->texts["SPEED"]->center(calcX(952, vm));
}

void StatsGUI::updateCritical()
{
    this->texts["CRITICAL"]->setText(
        std::to_string(player.getCriticalChance()) + "%");
    this->texts["CRITICAL"]->center(calcX(1016, vm));
}

void StatsGUI::updateXP()
{
    this->xpBarPercent = static_cast<float>(player.getXP()) /
                         static_cast<float>(player.getMaxXP());
    this->texts["XP"]->setText("XP:" + std::to_string(player.getXP()) + "/" +
                               std::to_string(player.getMaxXP()));
    this->texts["XP"]->center(calcX(640, vm));
}

void StatsGUI::updatingXP(float dt)
{
    const int width = this->sprites["XP_BAR"]->getTextureRect().width;
    const int barrier = static_cast<int>(this->xpBarPercent * 256.f);

    if (width <= barrier && player.isLeveling()) {
        const int distance = static_cast<int>(width + 1000.f * dt);
        if (distance > barrier) {
            this->sprites["XP_BAR"]->setTextureRect(
                sf::IntRect(0, 0, barrier, 20));
        }
        else if (distance > 256) {
            this->sprites["XP_BAR"]->setTextureRect(sf::IntRect(0, 0, 256, 20));
        }
        else {
            this->sprites["XP_BAR"]->setTextureRect(
                sf::IntRect(0, 0, distance, 20));
        }
    }
    else {
        player.setLeveling(false);
    }
}

void StatsGUI::updateLevel()
{
    this->sprites["XP_BAR"]->setTextureRect(sf::IntRect(0, 0, 0, 20));
    this->texts["LEVEL"]->setText("Level " + std::to_string(player.getLevel()));
    this->texts["LEVEL"]->center(calcX(640, this->vm));
}

void StatsGUI::updateHP()
{
    if (player.isDead()) {
        this->hpBarPercent = 0.f;
        this->sprites["HP_BAR"]->setTextureRect(sf::IntRect(0, 20, 0, 20));
    }
    else {
        this->hpBarPercent =
            static_cast<float>(player.getHP()) / player.getMaxHP();
    }

    this->texts["HP"]->setText("HP:" + std::to_string(player.getHP()) + "/" +
                               std::to_string(player.getMaxHP()));
    this->texts["HP"]->center(calcX(640, vm));
}

void StatsGUI::updatingHP(float dt)
{
    if (player.isHPRegenerating(dt) && !player.isDead()) {
        this->texts["HP"]->setText("HP:" + std::to_string(player.getHP()) +
                                   "/" + std::to_string(player.getMaxHP()));
        this->texts["HP"]->center(calcX(640, this->vm));
        this->hpBarPercent =
            static_cast<float>(player.getHP()) / player.getMaxHP();
        player.setRegenerating(true);
    }
    else if (player.isDead()) {
        this->hpBarPercent = 0.f;
        this->sprites["HP_BAR"]->setTextureRect(sf::IntRect(0, 20, 0, 20));
    }

    const int width = this->sprites["HP_BAR"]->getTextureRect().width;
    const int barrier = static_cast<int>(this->hpBarPercent * 256.f);

    if (width > barrier) {
        const int distance = static_cast<int>(width - 1000.f * dt);
        if (distance < barrier) {
            this->sprites["HP_BAR"]->setTextureRect(
                sf::IntRect(0, 20, barrier, 20));
            this->sprites["PROGRESS_BAR"]->setColor(sf::Color::White);
        }
        else if (distance < 0) {
            this->sprites["HP_BAR"]->setTextureRect(sf::IntRect(0, 20, 0, 20));
            this->sprites["PROGRESS_BAR"]->setColor(sf::Color::Transparent);
        }
        else {
            this->sprites["HP_BAR"]->setTextureRect(
                sf::IntRect(0, 20, distance, 20));
            this->sprites["PROGRESS_BAR"]->setColor(sf::Color::White);
        }
    }
    else if (width < barrier && player.isRegenerating()) {
        const int distance = static_cast<int>(width + 1000.f * dt);
        if (distance > barrier) {
            this->sprites["HP_BAR"]->setTextureRect(
                sf::IntRect(0, 20, barrier, 20));
            this->sprites["PROGRESS_BAR"]->setColor(sf::Color::White);
        }
        else if (distance > 256) {
            this->sprites["HP_BAR"]->setTextureRect(
                sf::IntRect(0, 20, 256, 20));
            this->sprites["PROGRESS_BAR"]->setColor(sf::Color::Transparent);
        }
        else {
            this->sprites["HP_BAR"]->setTextureRect(
                sf::IntRect(0, 20, distance, 20));
            this->sprites["PROGRESS_BAR"]->setColor(sf::Color::White);
        }
    }
    else {
        player.setRegenerating(false);
        this->sprites["PROGRESS_BAR"]->setColor(sf::Color::Transparent);
    }
}

void StatsGUI::updateSprint()
{
    this->texts["SPRINT"]->setText(
        std::to_string(static_cast<uint32_t>(player.getSprint())) + "/" +
        std::to_string(player.getMaxSprint()));
    this->texts["SPRINT"]->center(calcX(640, vm));
}

void StatsGUI::updatingSprint(float dt)
{
    const int bar_width = this->sprites["SPRINT_BAR"]->getTextureRect().width;
    const int barrier_width = static_cast<int>(
        player.getSprint() / static_cast<float>(player.getMaxSprint()) * 256.f);

    if (bar_width > barrier_width) {
        const int new_width = static_cast<int>(bar_width - 1000.f * dt);
        if (new_width < barrier_width) {
            this->sprites["SPRINT_BAR"]->setTextureRect(
                sf::IntRect(0, 40, barrier_width, 20));
        }
        else if (new_width < 0) {
            this->sprites["SPRINT_BAR"]->setTextureRect(
                sf::IntRect(0, 40, 0, 20));
        }
        else {
            this->sprites["SPRINT_BAR"]->setTextureRect(
                sf::IntRect(0, 40, new_width, 20));
        }
    }
    else if (bar_width < barrier_width) {
        const int new_width = static_cast<int>(bar_width + 1000.f * dt);
        if (new_width > barrier_width) {
            this->sprites["SPRINT_BAR"]->setTextureRect(
                sf::IntRect(0, 40, barrier_width, 20));
        }
        else if (new_width > 256) {
            this->sprites["SPRINT_BAR"]->setTextureRect(
                sf::IntRect(0, 40, 256, 20));
        }
        else {
            this->sprites["SPRINT_BAR"]->setTextureRect(
                sf::IntRect(0, 40, new_width, 20));
        }
    }

    this->texts["SPRINT"]->setText(
        std::to_string(static_cast<uint32_t>(player.getSprint())) + "/" +
        std::to_string(player.getMaxSprint()));
    this->texts["SPRINT"]->center(calcX(640, this->vm));
}

void StatsGUI::updateMonsterCount(const std::string &count)
{
    this->texts["MONSTER_COUNT"]->setText(count);
}

void StatsGUI::updateWaveNumber(const std::string &text)
{
    this->texts["WAVE_NUMBER"]->setText(text);
}

void StatsGUI::upgradePlayer(const std::string &t_name,
                             const sf::IntRect &intRect)
{
    this->texts["NAME"]->setText(t_name);
    this->texts["NAME"]->center(calcX(188, this->vm));
    this->sprites["MINIATURE"]->setTextureRect(intRect);
    this->sprites["ABILITY_ICON"]->setTextureRect(sf::IntRect(
        intRect.left, intRect.top - 16, intRect.width, intRect.height));
}

void StatsGUI::setAbilityIcon()
{
    this->sprite_buttons["ABILITY_UPGRADE"]->setColor(gui::YELLOW);
    this->abilityIcon.setSize(sf::Vector2f(calcX(80, vm), calcY(80, vm)));
    this->abilityIcon.setPosition(sf::Vector2f(calcX(260, vm), calcY(8, vm)));
}

const bool StatsGUI::updateAbilityIcon(float value)
{
    this->abilityIcon.setSize(
        sf::Vector2f(calcX(80, vm), calcX(80, vm) - value));
    this->abilityIcon.setPosition(
        sf::Vector2f(calcX(260, vm), calcX(8, vm) + value));
    if (this->abilityIcon.getSize().y <= 0.f) {
        this->sprite_buttons["ABILITY_UPGRADE"]->setColor(gui::GOLD);
        this->player.setAbilityActive(false);
        return true;
    }
    return false;
}

void StatsGUI::setWaveCountdownText(const std::string &text)
{
    this->texts["WAVE_COUNTDOWN"]->setText(text);
}

bool StatsGUI::hasClickedMenu(const sf::Vector2i &mousePos)
{
    return this->sprite_buttons["MENU"]->isPressed(mousePos);
}

bool StatsGUI::hasClickedShop(const sf::Vector2i &mousePos)
{
    return sprite_buttons["SHOP"]->isPressed(mousePos);
}

bool StatsGUI::hasClickedAbilityUpgrade(const sf::Vector2i &mousePos)
{
    return sprite_buttons["ABILITY_UPGRADE"]->isPressed(mousePos);
}

void StatsGUI::update(const sf::Vector2f &mousePosView)
{
    this->levelShown = true;
    if ((mousePosView.y >= calcY(12, vm) && mousePosView.y <= calcY(34, vm)) &&
        (mousePosView.x >= calcX(512, vm) &&
         mousePosView.x <= calcX(768, vm))) {
        this->levelShown = false;
    }
}

void StatsGUI::drawAbility(sf::RenderTarget &target)
{
    this->sprites["ABILITY_ICON"]->draw(target);
    if (player.getAbilityCooldown() < player.getAbilityMaxTime()) {
        target.draw(this->abilityIcon);
    }
    this->sprite_buttons["ABILITY_UPGRADE"]->draw(target);
}

void StatsGUI::drawWaveCountdown(sf::RenderTarget &target)
{
    this->texts["WAVE_COUNTDOWN"]->draw(target);
}

void StatsGUI::drawMonsterCount(sf::RenderTarget &target)
{
    this->texts["MONSTER_COUNT"]->draw(target);
}

void StatsGUI::drawMenu(sf::RenderTarget &target)
{
    this->sprite_buttons["MENU"]->draw(target);
}

void StatsGUI::draw(sf::RenderTarget &target)
{
    this->sprites["TOP_GUI"]->draw(target);
    this->sprites["PROGRESS_BAR"]->draw(target);
    this->sprites["MINIATURE"]->draw(target);
    this->texts["NAME"]->draw(target);
    this->sprites["GOLD"]->draw(target);
    this->texts["GOLD"]->draw(target);
    this->sprites["ATTACK"]->draw(target);
    this->texts["ATTACK"]->draw(target);
    this->sprites["ATTACK_SPEED"]->draw(target);
    this->texts["ATTACK_SPEED"]->draw(target);
    this->sprites["SPEED"]->draw(target);
    this->texts["SPEED"]->draw(target);
    this->sprites["CRITICAL"]->draw(target);
    this->texts["CRITICAL"]->draw(target);
    this->sprites["ARMOR"]->draw(target);
    this->texts["ARMOR"]->draw(target);
    this->sprites["REG"]->draw(target);
    this->texts["REG"]->draw(target);
    this->sprites["XP_BAR"]->draw(target);
    if (this->levelShown) {
        this->texts["LEVEL"]->draw(target);
    }
    else {
        this->texts["XP"]->draw(target);
    }
    this->sprites["HP_BAR"]->draw(target);
    this->texts["HP"]->draw(target);
    this->sprites["SPRINT_BAR"]->draw(target);
    this->texts["SPRINT"]->draw(target);

    this->sprites["SHOP_ICON"]->draw(target);
    this->sprite_buttons["SHOP"]->draw(target);
    this->texts["WAVE_NUMBER"]->draw(target);
}
