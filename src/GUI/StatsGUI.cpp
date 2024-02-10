#include "StatsGUI.hpp"

StatsGUI::StatsGUI(sf::VideoMode &t_vm, Player &t_player,
                   std::unordered_map<std::string, std::string> &t_lang)
    : vm(t_vm), player(t_player)
{
    this->framesTexture.loadFromFile("assets/textures/frames.png");
    this->attributesTexture.loadFromFile(
        "assets/textures/attributes_icons.png");
    this->barsTexture.loadFromFile("assets/textures/bars.png");
    this->upgradesTexture.loadFromFile("assets/textures/upgrades_icons.png");
    this->abilitiesTexture.loadFromFile("assets/textures/abilities_icons.png");

    this->sprites["TOP_GUI"] = std::make_unique<gui::Sprite>(
        "assets/textures/top_gui.png", 0.f, 0.f, calcScale(1, vm), false);

    this->sprite_buttons["MENU"] = std::make_unique<gui::ButtonSprite>(
        gui::RECT_BURGER, calcX(4, vm), calcY(4, vm), calcScale(2, vm),
        gui::GREY, gui::WHITE, false);

    this->sprites["SHOP_ICON"] =
        std::make_unique<gui::Sprite>("assets/textures/shop.png", calcX(44, vm),
                                      calcY(16, vm), calcScale(4, vm), false);
    this->sprite_buttons["SHOP"] = std::make_unique<gui::ButtonSprite>(
        gui::RECT_BUTTON, calcX(32, vm), calcY(4, vm), calcScale(1, vm),
        gui::GREY, gui::WHITE, false);

    this->sprites["GOLD"] = std::make_unique<gui::Sprite>(
        attributesTexture, 0.f, calcY(88, vm), calcScale(2, vm), false,
        sf::IntRect(0, 0, 16, 16));
    this->texts["GOLD"] = std::make_unique<gui::Text>(
        std::to_string(player.getGold()), calcChar(16, vm), calcX(36, vm),
        calcY(98, vm), gui::GOLD, false);

    this->sprites["PLAYER_FRAME"] = std::make_unique<gui::Sprite>(
        framesTexture, calcX(188, vm), calcY(4, vm), calcScale(1, vm), true,
        sf::IntRect(0, 0, 88, 88));
    this->sprites["PLAYER_ICON"] = std::make_unique<gui::Sprite>(
        upgradesTexture, calcX(188, vm), calcY(16, vm), calcScale(4, vm), true,
        sf::IntRect(0, 0, 16, 16));
    this->texts["PLAYER_NAME"] = std::make_unique<gui::Text>(
        t_lang[player.getName()], calcChar(16, vm), calcX(188, vm),
        calcY(98, vm), gui::WHITE, true);

    this->sprites["ABILITY_ICON"] = std::make_unique<gui::Sprite>(
        abilitiesTexture, calcX(268, vm), calcY(16, vm), calcScale(4, vm),
        false, sf::IntRect(0, 0, 16, 16));
    this->sprite_buttons["ABILITY"] = std::make_unique<gui::ButtonSprite>(
        gui::RECT_BUTTON, calcX(256, vm), calcY(4, vm), calcScale(1, vm),
        gui::GREY, gui::WHITE, false);
    this->sprite_buttons["ABILITY"]->setColor(gui::GOLD);
    this->abilityCooldown.setFillColor(sf::Color(128, 128, 128, 128));
    this->abilityCooldown.setSize(sf::Vector2f(calcX(80, vm), calcY(80, vm)));
    this->abilityCooldown.setPosition(
        sf::Vector2f(calcX(260, vm), calcY(8, vm)));

    this->sprites["ARMOR"] = std::make_unique<gui::Sprite>(
        attributesTexture, calcX(376, vm), calcY(16, vm), calcScale(2, vm),
        false, sf::IntRect(16, 0, 16, 16));
    this->texts["ARMOR"] = std::make_unique<gui::Text>(
        std::to_string(player.getArmor()), calcChar(16, vm), calcX(392, vm),
        calcY(70, vm), gui::LIGHT_GREY, true);

    this->sprites["REG"] = std::make_unique<gui::Sprite>(
        attributesTexture, calcX(440, vm), calcY(16, vm), calcScale(2, vm),
        false, sf::IntRect(32, 0, 16, 16));
    this->texts["REG"] = std::make_unique<gui::Text>(
        std::to_string(player.getReg()), calcChar(16, vm), calcX(456, vm),
        calcY(70, vm), gui::RED, true);

    XPBar = Bar{
        std::make_unique<gui::Sprite>(framesTexture, calcX(640, vm),
                                      calcY(4, vm), calcScale(1, vm), true,
                                      sf::IntRect(88, 0, 272, 36)),
        std::make_unique<gui::Sprite>(barsTexture, calcX(512, vm),
                                      calcY(12, vm), calcScale(1, vm), false,
                                      sf::IntRect(0, 0, 0, 0)),
        std::make_unique<gui::Text>(
            "Level " + std::to_string(player.getLevel()), calcChar(16, vm),
            calcX(640, vm), calcY(15, vm), gui::WHITE, true),
        std::make_unique<gui::Text>("XP:" + std::to_string(player.getXP()) +
                                        "/" + std::to_string(player.getMaxXP()),
                                    calcChar(16, vm), calcX(640, vm),
                                    calcY(15, vm), gui::WHITE, true),
        0.f,
        false};

    HPBar = Bar{
        std::make_unique<gui::Sprite>(framesTexture, calcX(640, vm),
                                      calcY(44, vm), calcScale(1, vm), true,
                                      sf::IntRect(88, 0, 272, 36)),
        std::make_unique<gui::Sprite>(barsTexture, calcX(512, vm),
                                      calcY(52, vm), calcScale(1, vm), false,
                                      sf::IntRect(0, 20, 256, 20)),
        std::make_unique<gui::Text>("HP:" + std::to_string(player.getHP()) +
                                        "/" + std::to_string(player.getMaxHP()),
                                    calcChar(16, vm), calcX(640, vm),
                                    calcY(55, vm), gui::WHITE, true),
        std::make_unique<gui::Text>("Full", calcChar(16, vm), calcX(640, vm),
                                    calcY(55, vm), gui::WHITE, true),
        1.f,
        false};

    SprintBar = Bar{
        std::make_unique<gui::Sprite>(framesTexture, calcX(640, vm),
                                      calcY(84, vm), calcScale(1, vm), true,
                                      sf::IntRect(88, 0, 272, 36)),
        std::make_unique<gui::Sprite>(barsTexture, calcX(512, vm),
                                      calcY(92, vm), calcScale(1, vm), false,
                                      sf::IntRect(0, 40, 256, 20)),
        std::make_unique<gui::Text>(std::to_string(player.getSprint()) + "/" +
                                        std::to_string(player.getMaxSprint()),
                                    calcChar(16, vm), calcX(640, vm),
                                    calcY(95, vm), gui::WHITE, true),
        std::make_unique<gui::Text>("Full", calcChar(16, vm), calcX(640, vm),
                                    calcY(95, vm), gui::WHITE, true),
        1.f,
        false};

    this->sprites["ATTACK"] = std::make_unique<gui::Sprite>(
        attributesTexture, calcX(808, vm), calcY(16, vm), calcScale(2, vm),
        false, sf::IntRect(80, 0, 16, 16));
    this->texts["ATTACK"] = std::make_unique<gui::Text>(
        std::to_string(player.getAttack()), calcChar(16, vm), calcX(824, vm),
        calcY(70, vm), gui::LIGHT_GREY, true);

    this->sprites["ATTACK_SPEED"] = std::make_unique<gui::Sprite>(
        attributesTexture, calcX(872, vm), calcY(16, vm), calcScale(2, vm),
        false, sf::IntRect(96, 0, 16, 16));
    this->texts["ATTACK_SPEED"] = std::make_unique<gui::Text>(
        std::to_string(player.getAttackSpeed()), calcChar(16, vm),
        calcX(888, vm), calcY(70, vm), gui::LIGHT_GREY, true);

    this->sprites["SPEED"] = std::make_unique<gui::Sprite>(
        attributesTexture, calcX(936, vm), calcY(16, vm), calcScale(2, vm),
        false, sf::IntRect(112, 0, 16, 16));
    this->texts["SPEED"] = std::make_unique<gui::Text>(
        std::to_string(player.getSpeed()), calcChar(16, vm), calcX(952, vm),
        calcY(70, vm), gui::LIGHT_GREY, true);

    this->sprites["CRITICAL"] = std::make_unique<gui::Sprite>(
        attributesTexture, calcX(1000, vm), calcY(16, vm), calcScale(2, vm),
        false, sf::IntRect(128, 0, 16, 16));
    this->texts["CRITICAL"] = std::make_unique<gui::Text>(
        std::to_string(player.getCriticalChance()) + "%", calcChar(16, vm),
        calcX(1016, vm), calcY(70, vm), gui::LIGHT_GREY, true);

    this->texts["MONSTER_COUNT"] = std::make_unique<gui::Text>(
        t_lang["MONSTER_COUNT"], calcChar(16, vm), calcX(906, vm),
        calcY(98, vm), sf::Color(130, 139, 152), false);
    this->texts["MONSTER_COUNT"]->setPosition(
        sf::Vector2f(calcX(1218, vm) - this->texts["MONSTER_COUNT"]->getWidth(),
                     calcY(98, vm)));

    this->texts["WAVE_NUMBER"] = std::make_unique<gui::Text>(
        t_lang["WAVE"], calcChar(32, vm), calcX(1060, vm), calcY(36, vm),
        gui::FLAMINGO, false);

    this->texts["WAVE_COUNTDOWN"] = std::make_unique<gui::Text>(
        t_lang["NEXT_WAVE"], calcChar(16, vm), calcX(954, vm), calcY(98, vm),
        gui::GOLD, false);
    this->texts["WAVE_COUNTDOWN"]->setPosition(sf::Vector2f(
        calcX(1218, vm) - this->texts["WAVE_COUNTDOWN"]->getWidth(),
        calcY(98, vm)));
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
    XPBar.percent = static_cast<float>(player.getXP()) /
                    static_cast<float>(player.getMaxXP());
    XPBar.hoverText->setText("XP:" + std::to_string(player.getXP()) + "/" +
                             std::to_string(player.getMaxXP()));
    XPBar.hoverText->center(calcX(640, vm));
}

void StatsGUI::updatingXP(float dt)
{
    const int width = XPBar.bar->getTextureRect().width;
    const int barrier = static_cast<int>(XPBar.percent * 256.f);

    if (width <= barrier && player.isLeveling()) {
        const int distance = static_cast<int>(width + 1000.f * dt);
        if (distance > barrier) {
            XPBar.bar->setTextureRect(sf::IntRect(0, 0, barrier, 20));
        }
        else if (distance > 256) {
            XPBar.bar->setTextureRect(sf::IntRect(0, 0, 256, 20));
        }
        else {
            XPBar.bar->setTextureRect(sf::IntRect(0, 0, distance, 20));
        }
    }
    else {
        player.setLeveling(false);
    }
}

void StatsGUI::updateLevel()
{
    XPBar.bar->setTextureRect(sf::IntRect(0, 0, 0, 20));
    XPBar.text->setText("Level " + std::to_string(player.getLevel()));
    XPBar.text->center(calcX(640, this->vm));
}

void StatsGUI::updateHP()
{
    if (player.isDead()) {
        HPBar.percent = 0.f;
        HPBar.bar->setTextureRect(sf::IntRect(0, 20, 0, 20));
    }
    else {
        HPBar.percent = static_cast<float>(player.getHP()) / player.getMaxHP();
    }

    HPBar.text->setText("HP:" + std::to_string(player.getHP()) + "/" +
                        std::to_string(player.getMaxHP()));
    HPBar.text->center(calcX(640, vm));
}

void StatsGUI::updatingHP(float dt)
{
    if (player.isHPRegenerating(dt) && !player.isDead()) {
        HPBar.text->setText("HP:" + std::to_string(player.getHP()) + "/" +
                            std::to_string(player.getMaxHP()));
        HPBar.text->center(calcX(640, this->vm));
        HPBar.percent = static_cast<float>(player.getHP()) / player.getMaxHP();
        player.setRegenerating(true);
    }
    else if (player.isDead()) {
        HPBar.percent = 0.f;
        HPBar.bar->setTextureRect(sf::IntRect(0, 20, 0, 20));
    }

    const int width = HPBar.bar->getTextureRect().width;
    const int barrier = static_cast<int>(HPBar.percent * 256.f);

    if (width > barrier) {
        const int distance = static_cast<int>(width - 1000.f * dt);
        if (distance < barrier) {
            HPBar.bar->setTextureRect(sf::IntRect(0, 20, barrier, 20));
        }
        else if (distance < 0) {
            HPBar.bar->setTextureRect(sf::IntRect(0, 20, 0, 20));
        }
        else {
            HPBar.bar->setTextureRect(sf::IntRect(0, 20, distance, 20));
        }
    }
    else if (width < barrier && player.isRegenerating()) {
        const int distance = static_cast<int>(width + 1000.f * dt);
        if (distance > barrier) {
            HPBar.bar->setTextureRect(sf::IntRect(0, 20, barrier, 20));
        }
        else if (distance > 256) {
            HPBar.bar->setTextureRect(sf::IntRect(0, 20, 256, 20));
        }
        else {
            HPBar.bar->setTextureRect(sf::IntRect(0, 20, distance, 20));
        }
    }
    else {
        player.setRegenerating(false);
    }
}

void StatsGUI::updateSprint()
{
    SprintBar.text->setText(
        std::to_string(static_cast<uint32_t>(player.getSprint())) + "/" +
        std::to_string(player.getMaxSprint()));
    SprintBar.text->center(calcX(640, vm));
}

void StatsGUI::updatingSprint(float dt)
{
    const int bar_width = SprintBar.bar->getTextureRect().width;
    const int barrier_width = static_cast<int>(
        player.getSprint() / static_cast<float>(player.getMaxSprint()) * 256.f);

    if (bar_width > barrier_width) {
        const int new_width = static_cast<int>(bar_width - 1000.f * dt);
        if (new_width < barrier_width) {
            SprintBar.bar->setTextureRect(
                sf::IntRect(0, 40, barrier_width, 20));
        }
        else if (new_width < 0) {
            SprintBar.bar->setTextureRect(sf::IntRect(0, 40, 0, 20));
        }
        else {
            SprintBar.bar->setTextureRect(sf::IntRect(0, 40, new_width, 20));
        }
    }
    else if (bar_width < barrier_width) {
        const int new_width = static_cast<int>(bar_width + 1000.f * dt);
        if (new_width > barrier_width) {
            SprintBar.bar->setTextureRect(
                sf::IntRect(0, 40, barrier_width, 20));
        }
        else if (new_width > 256) {
            SprintBar.bar->setTextureRect(sf::IntRect(0, 40, 256, 20));
        }
        else {
            SprintBar.bar->setTextureRect(sf::IntRect(0, 40, new_width, 20));
        }
    }

    SprintBar.text->setText(
        std::to_string(static_cast<uint32_t>(player.getSprint())) + "/" +
        std::to_string(player.getMaxSprint()));
    SprintBar.text->center(calcX(640, this->vm));
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
    this->texts["PLAYER_NAME"]->setText(t_name);
    this->texts["PLAYER_NAME"]->center(calcX(188, this->vm));
    this->sprites["PLAYER_ICON"]->setTextureRect(intRect);
    this->sprites["ABILITY_ICON"]->setTextureRect(sf::IntRect(
        intRect.left, intRect.top - 16, intRect.width, intRect.height));
}

void StatsGUI::setAbilityIcon()
{
    this->sprite_buttons["ABILITY"]->setColor(gui::YELLOW);
    this->abilityCooldown.setSize(sf::Vector2f(calcX(80, vm), calcY(80, vm)));
    this->abilityCooldown.setPosition(
        sf::Vector2f(calcX(260, vm), calcY(8, vm)));
}

const bool StatsGUI::updateAbilityIcon(float value)
{
    this->abilityCooldown.setSize(
        sf::Vector2f(calcX(80, vm), calcX(80, vm) - value));
    this->abilityCooldown.setPosition(
        sf::Vector2f(calcX(260, vm), calcX(8, vm) + value));
    if (this->abilityCooldown.getSize().y <= 0.f) {
        this->sprite_buttons["ABILITY"]->setColor(gui::GOLD);
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
    return sprite_buttons["ABILITY"]->isPressed(mousePos);
}

void StatsGUI::update(const sf::Vector2f &mousePosView)
{
    XPBar.update(mousePosView);
    HPBar.update(mousePosView);
    SprintBar.update(mousePosView);
}

void StatsGUI::drawAbility(sf::RenderTarget &target)
{
    this->sprites["ABILITY_ICON"]->draw(target);
    if (player.getAbilityCooldown() < player.getAbilityMaxTime()) {
        target.draw(this->abilityCooldown);
    }
    this->sprite_buttons["ABILITY"]->draw(target);
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
    this->sprites["SHOP_ICON"]->draw(target);
    this->sprite_buttons["SHOP"]->draw(target);
    this->sprites["GOLD"]->draw(target);
    this->texts["GOLD"]->draw(target);
    this->sprites["PLAYER_FRAME"]->draw(target);
    this->sprites["PLAYER_ICON"]->draw(target);
    this->texts["PLAYER_NAME"]->draw(target);
    this->sprites["ARMOR"]->draw(target);
    this->texts["ARMOR"]->draw(target);
    this->sprites["REG"]->draw(target);
    this->texts["REG"]->draw(target);

    XPBar.draw(target);
    HPBar.draw(target);
    SprintBar.draw(target);

    this->sprites["ATTACK"]->draw(target);
    this->texts["ATTACK"]->draw(target);
    this->sprites["ATTACK_SPEED"]->draw(target);
    this->texts["ATTACK_SPEED"]->draw(target);
    this->sprites["SPEED"]->draw(target);
    this->texts["SPEED"]->draw(target);
    this->sprites["CRITICAL"]->draw(target);
    this->texts["CRITICAL"]->draw(target);
    this->texts["WAVE_NUMBER"]->draw(target);
}
