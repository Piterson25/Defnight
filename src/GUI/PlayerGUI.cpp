#include "PlayerGUI.hpp"

PlayerGUI::PlayerGUI(sf::VideoMode &vm, Player &player,
                     FloatingTextSystem &floatingTextSystem,
                     const std::string &difficultyName,
                     std::unordered_map<std::string, std::string> &lang)
    : vm(vm), lang(lang), player(player), floatingTextSystem(floatingTextSystem)
{
    this->attributesTexture.loadFromFile(
        "assets/textures/attributes_icons.png");

    this->statsGUI = new StatsGUI(this->vm, this->player, this->lang);

    this->titleCooldown = 0.f;
    this->texts["WAVE_NEW_MOBS"] = std::make_unique<gui::Text>(
        "", calcChar(16, vm), calcX(640, vm), calcY(562, vm),
        sf::Color(228, 92, 95), true);
    this->texts["BIG_WAVE_NUMBER"] = std::make_unique<gui::Text>(
        this->lang["WAVE"], calcChar(64, vm), calcX(640, vm), calcY(256, vm),
        sf::Color(255, 255, 255), true);
    this->texts["MOBS_TO_KILL"] = std::make_unique<gui::Text>(
        this->lang["MONSTER"], calcChar(32, vm), calcX(640, vm), calcY(512, vm),
        sf::Color(192, 192, 192), true);
    this->waveCountdown = 0.f;

    this->leveling = false;

    this->texts["LEVEL_UP"] = std::make_unique<gui::Text>(
        "LEVEL UP!", calcChar(32, vm), calcX(640, vm), calcY(256, vm),
        sf::Color(255, 246, 76), true);
    this->sprites["LEVEL_UP"] = std::make_unique<gui::Sprite>(
        "assets/textures/bottom_gui.png", calcX(640, vm), calcY(460, vm),
        calcScale(1, vm), true);

    this->option1 = Option{
        std::make_unique<gui::Sprite>(this->attributesTexture, calcX(516, vm),
                                      calcY(524, vm), calcScale(4, vm), false),
        std::make_unique<gui::ButtonSprite>(gui::RECT_BUTTON, calcX(504, vm),
                                            calcY(512, vm), calcScale(1, vm),
                                            false),
        std::make_unique<gui::Text>("", calcChar(16, vm), calcX(548, vm),
                                    calcY(608, vm), gui::WHITE, false),
        std::make_unique<gui::Text>("", calcChar(16, vm), calcX(548, vm),
                                    calcY(630, vm), gui::WHITE, false),
        0,
        0};

    this->option2 = Option{
        std::make_unique<gui::Sprite>(this->attributesTexture, calcX(700, vm),
                                      calcY(524, vm), calcScale(4, vm), false),
        std::make_unique<gui::ButtonSprite>(gui::RECT_BUTTON, calcX(688, vm),
                                            calcY(512, vm), calcScale(1, vm),
                                            false),
        std::make_unique<gui::Text>("", calcChar(16, vm), calcX(732, vm),
                                    calcY(608, vm), sf::Color(255, 255, 255),
                                    false),
        std::make_unique<gui::Text>("", calcChar(16, vm), calcX(732, vm),
                                    calcY(630, vm), sf::Color(255, 255, 255),
                                    false),
        0,
        0};

    this->sprites["UPGRADES"] = std::make_unique<gui::Sprite>(
        "assets/textures/side_gui.png", calcX(1280, vm), calcY(128, vm),
        calcScale(1, vm), false);
    this->sprites["UPGRADES"]->flipHorizontal();

    this->upgradeGUI = new UpgradeGUI(vm, this->player);
    this->upgrading = false;
    this->upgradeGUI->changeUpgrade("UPGRADE1", calcX(1024, vm), calcY(222, vm),
                                    this->lang["NINJA"], 0, 7, 1, "1");
    this->upgradeGUI->changeUpgrade("UPGRADE2", calcX(1024, vm), calcY(392, vm),
                                    this->lang["KNIGHT"], 3, 5, 1, "1");
    this->upgradeGUI->changeUpgrade("UPGRADE3", calcX(1024, vm), calcY(562, vm),
                                    this->lang["SCOUT"], 6, 2, 1, "1");

    this->death_background.setFillColor(sf::Color(182, 60, 53, 192));
    this->death_background.setSize(
        sf::Vector2f(calcX(1280, vm), calcY(592, vm)));
    this->death_background.setPosition(sf::Vector2f(0, calcY(128, vm)));

    this->texts["YOU_DIED"] = std::make_unique<gui::Text>(
        this->lang["YOU_DIED"], calcChar(72, vm), calcX(640, vm),
        calcY(224, vm), sf::Color(255, 255, 255), true);
    this->text_buttons["RESUME"] = std::make_unique<gui::ButtonText>(
        this->lang["RESUME"], calcChar(32, vm), calcX(640, vm), calcY(296, vm),
        sf::Color(255, 255, 255), sf::Color(192, 192, 192), true);
    this->text_buttons["SETTINGS"] = std::make_unique<gui::ButtonText>(
        this->lang["SETTINGS"], calcChar(32, vm), calcX(640, vm),
        calcY(392, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192),
        true);
    this->text_buttons["MAIN_MENU"] = std::make_unique<gui::ButtonText>(
        this->lang["MAIN_MENU"], calcChar(32, vm), calcX(640, vm),
        calcY(488, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192),
        true);
    this->text_buttons["QUIT"] = std::make_unique<gui::ButtonText>(
        this->lang["QUIT"], calcChar(32, vm), calcX(640, vm), calcY(584, vm),
        sf::Color(255, 255, 255), sf::Color(192, 192, 192), true);

    this->texts["KILLS"] = std::make_unique<gui::Text>(
        this->lang["KILLS"] + std::to_string(player.getKills()),
        calcChar(16, vm), calcX(640, vm), calcY(186, vm),
        sf::Color(192, 192, 192), true);

    this->texts["DIFFICULTY"] = std::make_unique<gui::Text>(
        this->lang["DIFFICULTY_LEVEL"] + " " + this->lang[difficultyName],
        calcChar(16, vm), calcX(640, vm), calcY(186, vm), gui::RED, true);

    this->escape_background.setFillColor(sf::Color(0, 0, 0, 192));
    this->escape_background.setSize(
        sf::Vector2f(calcX(1280, vm), calcY(720, vm)));
    this->escape = false;

    this->sprites["SIDE_GUI"] =
        std::make_unique<gui::Sprite>("assets/textures/side_gui.png", 0.f,
                                      calcY(128, vm), calcScale(1, vm), false);

    this->abilityUpgradeGUI = new AbilityUpgradeGUI(vm, this->player);
    this->abilityUpgradeGUI->addAbilityUpgrade("LOWER_COOLDOWN", calcX(44, vm),
                                               calcY(324, vm), 0, "Cooldown",
                                               "-10%", 20, 0, 5);

    this->shopGUI = new ShopGUI(vm, this->player);
    this->shopGUI->addShopItem("FULL_HP", calcX(44, vm), calcY(188, vm), 9,
                               "Full HP", "+Full", 30, 0, 0);
    this->shopGUI->addShopItem("MAX_HP", calcX(44, vm), calcY(324, vm), 3,
                               this->lang["MAX_HP"], "+2", 20, 0, 0);
    this->shopGUI->addShopItem("ATTACK", calcX(44, vm), calcY(460, vm), 5,
                               this->lang["ATTACK"], "+1", 15, 0, 0);
    this->shopGUI->addShopItem("ARMOR", calcX(44, vm), calcY(596, vm), 1,
                               this->lang["ARMOR"], "+1", 20, player.getArmor(),
                               10);

    this->sideGUI = SideGUI::NONE;

    this->bossWave = false;
    this->bossCooldown = 0.f;

    this->texts["BOSS"] = std::make_unique<gui::Text>(
        "Minotaur", calcChar(16, vm), calcX(640, vm), calcY(136, vm),
        sf::Color(113, 43, 59), true);
    this->sprites["BOSS_BAR"] = std::make_unique<gui::Sprite>(
        "assets/textures/bars.png", calcX(640, vm), calcY(158, vm),
        calcScale(1, vm), true);
    this->sprites["BOSS_BAR"]->setTextureRect(sf::IntRect(0, 60, 512, 20));
    this->sprites["BOSS_BAR_EMPTY"] = std::make_unique<gui::Sprite>(
        "assets/textures/bars.png", calcX(640, vm), calcY(158, vm),
        calcScale(1, vm), true);
    this->sprites["BOSS_BAR_EMPTY"]->setTextureRect(
        sf::IntRect(0, 80, 512, 20));

    this->boss_bar_percent = 1.f;
}

PlayerGUI::~PlayerGUI()
{
    delete this->statsGUI;
    delete this->shopGUI;
    delete this->upgradeGUI;
    delete this->abilityUpgradeGUI;
}

void PlayerGUI::updateOption(Option &option, std::vector<short> &id_vector,
                             float pos)
{
    const short id = static_cast<short>(Random::Float() * id_vector.size());

    option.id = id_vector[id];
    switch (option.id) {
        case 1:
            option.value = 1;
            option.optionDesc->setText(this->lang["ARMOR"]);
            break;
        case 2:
            option.value = 1;
            option.optionDesc->setText(this->lang["REG"]);
            break;
        case 3:
            option.value = static_cast<uint32_t>(Random::Float() * 2.f) + 2;
            option.optionDesc->setText(this->lang["MAX_HP"]);
            break;
        case 4:
            option.value = 50;
            option.optionDesc->setText(this->lang["SPRINT"]);
            break;
        case 5:
            option.value = 1;
            option.optionDesc->setText(this->lang["ATTACK"]);
            break;
        case 6:
            option.value = 1;
            option.optionDesc->setText(this->lang["ATTACK_SPEED"]);
            break;
        case 7:
            option.value = 1;
            option.optionDesc->setText(this->lang["SPEED"]);
            break;
        case 8:
            option.value = 10;
            option.optionDesc->setText(this->lang["CRITICAL"]);
            break;
    }
    option.optionValue->setText("+" + std::to_string(option.value));
    if (option.id == 8) {
        option.optionValue->setText(option.optionValue->getText() + "%");
    }
    option.optionValue->center(pos);
    id_vector.erase(id_vector.begin() + id);
    option.optionSprite->setTextureRect(sf::IntRect(option.id * 16, 0, 16, 16));
    option.optionDesc->center(pos);
}

void PlayerGUI::levelUpPlayer(uint32_t optionID, uint32_t optionValue)
{
    const sf::VideoMode vm = this->vm;

    switch (optionID) {
        case 1:
            player.setArmor(player.getArmor() + optionValue);
            shopGUI->updateSegments("ARMOR");
            statsGUI->updateArmor();
            break;
        case 2:
            player.setReg(player.getReg() + optionValue);
            statsGUI->updateReg();
            break;
        case 3:
            player.setMaxHP(player.getMaxHP() + optionValue);
            this->updateHP();
            player.setRegenerating(true);
            break;
        case 4:
            player.setMaxSprint(player.getMaxSprint() + optionValue);
            statsGUI->updateSprint();
            break;
        case 5:
            player.setAttack(player.getAttack() + optionValue);
            statsGUI->updateAttack();
            break;
        case 6:
            player.setAttackSpeed(player.getAttackSpeed() + optionValue);
            statsGUI->updateAttackSpeed();
            break;
        case 7:
            player.setSpeed(player.getSpeed() + optionValue);
            statsGUI->updateSpeed();
            break;
        case 8:
            player.setCriticalChance(player.getCriticalChance() + optionValue);
            statsGUI->updateCritical();
            break;
    }
}

void PlayerGUI::upgradePlayer(const std::string &name)
{
    if (player.isAbilityActive()) {
        player.endAbility();
    }

    sf::IntRect intRect = sf::IntRect(0, 0, 16, 16);
    player.upgrade(name, intRect);
    statsGUI->upgradePlayer(this->lang[name], intRect);
    this->abilityUpgradeGUI->updatePlayerInfo("COOLDOWN", "Cooldown");
    this->abilityUpgradeGUI->updatePlayerInfo("PROJ_ATTACK",
                                              this->lang["ATTACK"]);
    this->abilityUpgradeGUI->updatePlayerInfo("PIERCING",
                                              this->lang["PIERCING"]);
    this->abilityUpgradeGUI->updatePlayerInfo("AREA", this->lang["AREA"]);
    this->abilityUpgradeGUI->updatePlayerInfo("ARMOR", this->lang["ARMOR"]);
    this->abilityUpgradeGUI->updatePlayerInfo("ATTACK", this->lang["ATTACK"]);
    this->abilityUpgradeGUI->updatePlayerInfo("REG", this->lang["REG"]);
    this->abilityUpgradeGUI->updatePlayerInfo("SLOWDOWN",
                                              this->lang["SLOWDOWN"]);
    this->abilityUpgradeGUI->updatePlayerInfo("GOLD", this->lang["GOLD"]);
    this->abilityUpgradeGUI->updatePlayerInfo("TARGETS", this->lang["TARGETS"]);
    updatePlayerAttributes();
    this->update_Gold();
}

void PlayerGUI::update_level(SoundEngine &soundEngine)
{
    soundEngine.addSound("levelup");
    this->leveling = true;
    this->sideGUI = SideGUI::NONE;
    this->statsGUI->updateLevel();
    if (player.getLevel() == 5 || player.getLevel() == 10) {
        this->upgrading = true;
    }

    std::vector<short> id = {3, 4, 5};
    if (player.getArmor() < 10) {
        id.push_back(1);
    }
    if (player.getReg() < 10) {
        id.push_back(2);
    }
    if (player.getAttackSpeed() < 10) {
        id.push_back(6);
    }
    if (player.getSpeed() < 10) {
        id.push_back(7);
    }
    if (player.getCriticalChance() < 100) {
        id.push_back(8);
    }

    updateOption(this->option1, id, calcX(548, vm));
    updateOption(this->option2, id, calcX(732, vm));

    if (player.getLevel() == 10) {
        if (player.getName() == "NINJA") {
            this->upgradeGUI->changeUpgrade("UPGRADE1", calcX(1024, vm),
                                            calcY(222, vm),
                                            this->lang["SENSEI"], 1, 6, 1, "1");
            this->upgradeGUI->changeUpgrade(
                "UPGRADE2", calcX(1024, vm), calcY(392, vm),
                this->lang["BOMBER"], 2, 8, 1, "10%");
        }
        else if (player.getName() == "KNIGHT") {
            this->upgradeGUI->changeUpgrade(
                "UPGRADE1", calcX(1024, vm), calcY(210, vm),
                this->lang["CRUSADER"], 4, 3, 2, "2");
            this->upgradeGUI->changeUpgrade(
                "UPGRADE2", calcX(1024, vm), calcY(380, vm),
                this->lang["PALADIN"], 5, 6, 1, "1");
        }
        else if (player.getName() == "SCOUT") {
            this->upgradeGUI->changeUpgrade(
                "UPGRADE1", calcX(1024, vm), calcY(210, vm),
                this->lang["ASSASSIN"], 7, 6, 1, "1");
            this->upgradeGUI->changeUpgrade(
                "UPGRADE2", calcX(1024, vm), calcY(380, vm),
                this->lang["KILLER"], 8, 8, 1, "10%");
        }
    }
}

void PlayerGUI::updateXP()
{
    this->statsGUI->updateXP();
}

void PlayerGUI::updateHP()
{
    this->statsGUI->updateHP();

    if (this->player.isDead()) {
        this->sideGUI = SideGUI::NONE;
    }
}

void PlayerGUI::updateSprint()
{
    this->statsGUI->updateSprint();
}

void PlayerGUI::updatingHP(SoundEngine &soundEngine, float dt)
{
    this->statsGUI->updatingHP(dt);

    if (this->isShopping() && player.getHP() / player.getMaxHP() == 1) {
        this->shopGUI->disableItem("FULL_HP");
    }
    else if (player.isDead()) {
        soundEngine.addSound("gameover");
    }
}

void PlayerGUI::update_Gold()
{
    statsGUI->updateGold();
    shopGUI->updateItemFrames();
    abilityUpgradeGUI->updateItemFrames();
}

void PlayerGUI::update_ability(float dt)
{
    if (player.getAbilityCooldown() > 0.f) {
        const float value = player.getAbilityCooldown() /
                            (player.getAbilityTotalMaxTime()) * calcX(80, vm);
        if (this->statsGUI->updateAbilityIcon(value)) {
            abilityUpgradeGUI->updateItemFrames();
        }
    }
}

void PlayerGUI::setAbilityIcon()
{
    statsGUI->setAbilityIcon();
    abilityUpgradeGUI->updateItemFrames();
}

void PlayerGUI::updatingSprint(float dt)
{
    this->statsGUI->updatingSprint(dt);
}

void PlayerGUI::updateIsShopping()
{
    if (this->isShopping()) {
        this->sideGUI = SideGUI::NONE;
    }
    else {
        this->sideGUI = SideGUI::SHOP;
    }
}

void PlayerGUI::updateIsBuyingAbility()
{
    if (this->isBuyingAbility()) {
        this->sideGUI = SideGUI::NONE;
    }
    else {
        this->sideGUI = SideGUI::ABILITY_UPGRADE;
    }
}

void PlayerGUI::updateKills()
{
    this->texts["KILLS"]->setText(this->lang["KILLS"] +
                                  std::to_string(player.getKills()));
}

void PlayerGUI::updateMonsterCountWave(const std::string &language,
                                       uint32_t wave, bool bossWave,
                                       const size_t &monsterCount,
                                       SoundEngine &soundEngine)
{
    this->bossWave = bossWave;

    this->statsGUI->updateWaveNumber(this->lang["WAVE"] + std::to_string(wave));

    this->texts["BIG_WAVE_NUMBER"]->setText(this->lang["WAVE"] +
                                            std::to_string(wave));
    this->texts["BIG_WAVE_NUMBER"]->center(calcX(640, this->vm));

    switch (wave) {
        case 1:
            this->texts["WAVE_NEW_MOBS"]->setText(this->lang["NEW_TYPE"] +
                                                  this->lang["GOBLIN"]);
            break;
        case 3:
            this->texts["WAVE_NEW_MOBS"]->setText(this->lang["NEW_TYPE"] +
                                                  this->lang["SPIDER"]);
            break;
        case 5:
            this->texts["WAVE_NEW_MOBS"]->setText(this->lang["NEW_TYPE"] +
                                                  this->lang["ORC"]);
            break;
        case 7:
            this->texts["WAVE_NEW_MOBS"]->setText(this->lang["NEW_TYPE"] +
                                                  this->lang["CYCLOPE"]);
            break;
        case 10:
            this->texts["WAVE_NEW_MOBS"]->setText(this->lang["NEW_TYPE"] +
                                                  this->lang["MINOTAUR"]);
            break;
        default:
            this->texts["WAVE_NEW_MOBS"]->setText("");
            break;
    }

    this->texts["WAVE_NEW_MOBS"]->center(calcX(640, this->vm));

    updateMonsterCount(monsterCount);

    if (language == "polish") {
        if (monsterCount > 10) {
            if (monsterCount % 10 >= 2 && monsterCount % 10 <= 4) {
                this->texts["MOBS_TO_KILL"]->setText(
                    std::to_string(monsterCount) + " potwory");
            }
            else {
                this->texts["MOBS_TO_KILL"]->setText(
                    std::to_string(monsterCount) + " potworow");
            }
        }
        else {
            if (monsterCount == 1) {
                this->texts["MOBS_TO_KILL"]->setText(
                    std::to_string(monsterCount) + " potwor");
            }
            else if (monsterCount < 5) {
                this->texts["MOBS_TO_KILL"]->setText(
                    std::to_string(monsterCount) + " potwory");
            }
            else {
                this->texts["MOBS_TO_KILL"]->setText(
                    std::to_string(monsterCount) + " potworow");
            }
        }
    }
    else {
        if (monsterCount == 1) {
            this->texts["MOBS_TO_KILL"]->setText(std::to_string(monsterCount) +
                                                 " monster");
        }
        else {
            this->texts["MOBS_TO_KILL"]->setText(std::to_string(monsterCount) +
                                                 " monsters");
        }
    }
    this->texts["MOBS_TO_KILL"]->center(calcX(640, this->vm));

    soundEngine.addSound("new_wave");
}

void PlayerGUI::updateMonsterCount(const size_t &monsterCount)
{
    statsGUI->updateMonsterCount(this->lang["MONSTER_COUNT"] +
                                 std::to_string(monsterCount));
}

const bool PlayerGUI::hasClickedMenu(const sf::Vector2i &mousePos,
                                     bool mouseClicked, bool &paused)
{
    if (!player.isDead() && statsGUI->hasClickedMenu(mousePos, mouseClicked)) {
        this->updatePaused(paused);
        return true;
    }

    return false;
}

void PlayerGUI::setIsEscape(bool escape)
{
    this->escape = escape;
}

void PlayerGUI::updatePaused(bool &paused)
{
    if (paused) {
        this->setIsEscape(false);
        paused = false;
    }
    else {
        this->setIsEscape(true);
        paused = true;
    }
}

void PlayerGUI::updateArmor()
{
    this->statsGUI->updateArmor();
}

void PlayerGUI::updateAttack()
{
    this->statsGUI->updateAttack();
}

void PlayerGUI::updateReg()
{
    this->statsGUI->updateReg();
}

void PlayerGUI::updateBossHP(float dt)
{
    if (this->bossWave) {
        const int width = this->sprites["BOSS_BAR"]->getTextureRect().width;
        const int barrier = static_cast<int>(this->boss_bar_percent * 512.f);

        if (width > barrier) {
            const int distance = static_cast<int>(width - 1000.f * dt);
            if (distance < barrier) {
                this->sprites["BOSS_BAR"]->setTextureRect(
                    sf::IntRect(0, 60, barrier, 20));
            }
            else if (distance < 0) {
                this->sprites["BOSS_BAR"]->setTextureRect(
                    sf::IntRect(0, 60, 0, 20));
            }
            else {
                this->sprites["BOSS_BAR"]->setTextureRect(
                    sf::IntRect(0, 60, distance, 20));
            }
        }
    }
}

const bool PlayerGUI::hasClickedShopBuy(const sf::Vector2i &mousePos,
                                        bool mouseClicked,
                                        SoundEngine &soundEngine)
{
    if (statsGUI->hasClickedShop(mousePos, mouseClicked)) {
        updateIsShopping();
        return true;
    }

    if (this->isShopping()) {
        if (player.getHP() < player.getMaxHP() &&
            shopGUI->hasBoughtItem(mousePos, mouseClicked, "FULL_HP",
                                   &this->floatingTextSystem, &soundEngine)) {
            player.setHP(player.getMaxHP());
            this->updateHP();
            player.setRegenerating(true);
            this->update_Gold();
            return true;
        }
        else if (shopGUI->hasBoughtItem(mousePos, mouseClicked, "MAX_HP",
                                        &this->floatingTextSystem,
                                        &soundEngine)) {
            player.setMaxHP(player.getMaxHP() + 2);
            this->updateHP();
            player.setRegenerating(true);
            this->update_Gold();
            return true;
        }
        else if (shopGUI->hasBoughtItem(mousePos, mouseClicked, "ATTACK",
                                        &this->floatingTextSystem,
                                        &soundEngine)) {
            player.setAttack(player.getAttack() + 1);
            this->update_Gold();
            return true;
        }
        else if (shopGUI->hasBoughtItem(mousePos, mouseClicked, "ARMOR",
                                        &this->floatingTextSystem,
                                        &soundEngine)) {
            player.setArmor(player.getArmor() + 1);
            shopGUI->updateSegments("ARMOR");
            this->update_Gold();
            return true;
        }
    }

    return false;
}

const bool PlayerGUI::hasClickedAbilityBuy(const sf::Vector2i &mousePos,
                                           bool mouseClicked,
                                           SoundEngine &soundEngine)
{
    if (this->statsGUI->hasClickedAbilityUpgrade(mousePos, mouseClicked)) {
        updateIsBuyingAbility();
        return true;
    }

    if (this->isBuyingAbility()) {
        if (abilityUpgradeGUI->hasBoughtUpgrade(
                mousePos, mouseClicked, "LOWER_COOLDOWN",
                &this->floatingTextSystem, &soundEngine)) {
            player.setAbilityMaxTimeModifier(
                player.getAbilityMaxTimeModifier() - 0.1f);
            this->update_Gold();
            this->abilityUpgradeGUI->updatePlayerInfo("COOLDOWN", "Cooldown");
            this->abilityUpgradeGUI->updateSegments("LOWER_COOLDOWN");
            return true;
        }
        else if (abilityUpgradeGUI->hasBoughtUpgrade(
                     mousePos, mouseClicked, "PROJ_ATTACK",
                     &this->floatingTextSystem, &soundEngine)) {
            player.setProjectileAttack(player.getProjectileAttack() + 1);
            this->update_Gold();
            this->abilityUpgradeGUI->updatePlayerInfo("PROJ_ATTACK",
                                                      this->lang["ATTACK"]);
            return true;
        }
        else if (abilityUpgradeGUI->hasBoughtUpgrade(
                     mousePos, mouseClicked, "PIERCING",
                     &this->floatingTextSystem, &soundEngine)) {
            player.setProjectilePiercing(player.getProjectilePiercing() + 1);
            this->update_Gold();
            this->abilityUpgradeGUI->updatePlayerInfo("PIERCING",
                                                      this->lang["PIERCING"]);
            this->abilityUpgradeGUI->updateSegments("PIERCING");
            return true;
        }
        else if (abilityUpgradeGUI->hasBoughtUpgrade(
                     mousePos, mouseClicked, "AREA", &this->floatingTextSystem,
                     &soundEngine)) {
            player.setProjectileArea(player.getProjectileArea() + 1);
            this->update_Gold();
            this->abilityUpgradeGUI->updatePlayerInfo("AREA",
                                                      this->lang["AREA"]);
            this->abilityUpgradeGUI->updateSegments("AREA");
            return true;
        }
        else if (abilityUpgradeGUI->hasBoughtUpgrade(
                     mousePos, mouseClicked, "ARMOR", &this->floatingTextSystem,
                     &soundEngine)) {
            player.setIncreasedArmor(player.getIncreasedArmor() + 1);
            this->update_Gold();
            this->abilityUpgradeGUI->updatePlayerInfo("ARMOR",
                                                      this->lang["ARMOR"]);
            this->abilityUpgradeGUI->updateSegments("ARMOR");
            return true;
        }
        else if (abilityUpgradeGUI->hasBoughtUpgrade(
                     mousePos, mouseClicked, "ATTACK",
                     &this->floatingTextSystem, &soundEngine)) {
            player.setIncreasedAttack(player.getIncreasedAttack() + 1);
            this->update_Gold();
            this->abilityUpgradeGUI->updatePlayerInfo("ATTACK",
                                                      this->lang["ATTACK"]);
            return true;
        }
        else if (abilityUpgradeGUI->hasBoughtUpgrade(
                     mousePos, mouseClicked, "REG", &this->floatingTextSystem,
                     &soundEngine)) {
            player.setIncreasedReg(player.getIncreasedReg() + 1);
            this->update_Gold();
            this->abilityUpgradeGUI->updatePlayerInfo("REG", this->lang["REG"]);
            this->abilityUpgradeGUI->updateSegments("REG");
            return true;
        }
        else if (abilityUpgradeGUI->hasBoughtUpgrade(
                     mousePos, mouseClicked, "SLOWDOWN",
                     &this->floatingTextSystem, &soundEngine)) {
            player.setTimeSlowdown(player.getTimeSlowdown() + 0.1f);
            this->update_Gold();
            this->abilityUpgradeGUI->updatePlayerInfo("SLOWDOWN",
                                                      this->lang["SLOWDOWN"]);
            this->abilityUpgradeGUI->updateSegments("SLOWDOWN");
            return true;
        }
        else if (abilityUpgradeGUI->hasBoughtUpgrade(
                     mousePos, mouseClicked, "GOLD", &this->floatingTextSystem,
                     &soundEngine)) {
            player.setIncreasedGold(player.getIncreasedGold() + 1);
            this->update_Gold();
            this->abilityUpgradeGUI->updatePlayerInfo("GOLD",
                                                      this->lang["GOLD"]);
            this->abilityUpgradeGUI->updateSegments("GOLD");
            return true;
        }
        else if (abilityUpgradeGUI->hasBoughtUpgrade(
                     mousePos, mouseClicked, "TARGETS",
                     &this->floatingTextSystem, &soundEngine)) {
            player.setAttackLimit(player.getAttackLimit() + 1);
            this->update_Gold();
            this->abilityUpgradeGUI->updatePlayerInfo("TARGETS",
                                                      this->lang["TARGETS"]);
            this->abilityUpgradeGUI->updateSegments("TARGETS");
            return true;
        }
    }

    return false;
}

const bool PlayerGUI::isEscape() const
{
    return this->escape;
}

const bool PlayerGUI::isLeveling() const
{
    return this->leveling;
}

const bool PlayerGUI::isUpgrading() const
{
    return this->upgrading;
}

const bool PlayerGUI::isShopping() const
{
    return this->sideGUI == SideGUI::SHOP;
}

const bool PlayerGUI::isBuyingAbility() const
{
    return this->sideGUI == SideGUI::ABILITY_UPGRADE;
}

const uint8_t PlayerGUI::updateEscapeButton(const sf::Vector2i &mousePos,
                                            const bool &mouseClicked)
{
    this->text_buttons["RESUME"]->update(mousePos);
    if (this->text_buttons["RESUME"]->isPressed() && !mouseClicked) {
        this->escape = false;
        return 3;
    }

    this->text_buttons["MAIN_MENU"]->update(mousePos);
    if (this->text_buttons["MAIN_MENU"]->isPressed() && !mouseClicked) {
        return 1;
    }

    this->text_buttons["QUIT"]->update(mousePos);
    if (this->text_buttons["QUIT"]->isPressed() && !mouseClicked) {
        return 2;
    }

    this->text_buttons["SETTINGS"]->update(mousePos);
    if (this->text_buttons["SETTINGS"]->isPressed() && !mouseClicked) {
        return 4;
    }

    return 0;
}

const bool PlayerGUI::hasClickedButtons(const sf::Vector2i &mousePos,
                                        bool mouseClicked,
                                        SoundEngine &soundEngine)
{
    if (this->leveling && this->upgrading) {
        this->hasClickedLevelUpButtons(mousePos, mouseClicked, soundEngine);
        this->hasClickedUpgradeButtons(mousePos, mouseClicked, soundEngine);
    }
    else if (this->leveling) {
        return this->hasClickedLevelUpButtons(mousePos, mouseClicked,
                                              soundEngine);
    }
    else if (this->upgrading) {
        return this->hasClickedUpgradeButtons(mousePos, mouseClicked,
                                              soundEngine);
    }
    return false;
}

const bool PlayerGUI::hasClickedLevelUpButtons(const sf::Vector2i &mousePos,
                                               bool mouseClicked,
                                               SoundEngine &soundEngine)
{
    this->option1.optionButton->update(mousePos);
    if (this->option1.optionButton->isPressed() && !mouseClicked) {
        this->levelUpPlayer(this->option1.id, this->option1.value);
        this->leveling = false;
        soundEngine.addSound("option");
        return true;
    }

    this->option2.optionButton->update(mousePos);
    if (this->option2.optionButton->isPressed() && !mouseClicked) {
        this->levelUpPlayer(this->option2.id, this->option2.value);
        this->leveling = false;
        soundEngine.addSound("option");
        return true;
    }

    return false;
}

const bool PlayerGUI::hasClickedUpgradeButtons(const sf::Vector2i &mousePos,
                                               bool mouseClicked,
                                               SoundEngine &soundEngine)
{
    if (this->upgradeGUI->hasClickedUpgrade(mousePos, mouseClicked, "UPGRADE1",
                                            &soundEngine)) {
        if (player.getLevel() == 5) {
            this->abilityUpgradeGUI->addPlayerStat("PROJ_ATTACK", calcX(32, vm),
                                                   calcY(212, vm),
                                                   this->lang["ATTACK"]);
            this->abilityUpgradeGUI->addAbilityUpgrade(
                "PROJ_ATTACK", calcX(44, vm), calcY(460, vm), 1,
                this->lang["ATTACK"], "+1", 30, 0, 0);
            this->upgradePlayer("NINJA");
        }
        else if (player.getLevel() == 10) {
            if (player.getName() == "NINJA") {
                this->abilityUpgradeGUI->addPlayerStat(
                    "PIERCING", calcX(32, vm), calcY(250, vm),
                    this->lang["PIERCING"]);
                this->abilityUpgradeGUI->addAbilityUpgrade(
                    "PIERCING", calcX(44, vm), calcY(596, vm), 2,
                    this->lang["PIERCING"], "+1", 50, 1, 5);
                this->upgradePlayer("SENSEI");
            }
            else if (player.getName() == "KNIGHT") {
                this->abilityUpgradeGUI->addPlayerStat("ATTACK", calcX(32, vm),
                                                       calcY(250, vm),
                                                       this->lang["ATTACK"]);
                this->abilityUpgradeGUI->addAbilityUpgrade(
                    "ATTACK", calcX(44, vm), calcY(596, vm), 1,
                    this->lang["ATTACK"], "+1", 50, 0, 0);
                this->upgradePlayer("CRUSADER");
            }
            else if (player.getLevel() == 10) {
                if (player.getName() == "SCOUT") {
                    this->abilityUpgradeGUI->addPlayerStat(
                        "GOLD", calcX(32, vm), calcY(250, vm),
                        this->lang["GOLD"]);
                    this->abilityUpgradeGUI->addAbilityUpgrade(
                        "GOLD", calcX(44, vm), calcY(596, vm), 7,
                        this->lang["GOLD"], "+1", 100, 1, 10);
                    this->upgradePlayer("ASSASSIN");
                }
            }
        }

        this->upgrading = false;
        return true;
    }
    else if (this->upgradeGUI->hasClickedUpgrade(mousePos, mouseClicked,
                                                 "UPGRADE2", &soundEngine)) {
        if (player.getLevel() == 5) {
            this->abilityUpgradeGUI->addPlayerStat(
                "ARMOR", calcX(32, vm), calcY(212, vm), this->lang["ARMOR"]);
            this->abilityUpgradeGUI->addAbilityUpgrade(
                "ARMOR", calcX(44, vm), calcY(460, vm), 4, this->lang["ARMOR"],
                "+1", 30, 5, 10);
            this->upgradePlayer("KNIGHT");
        }
        else if (player.getLevel() == 10) {
            if (player.getName() == "NINJA") {
                this->abilityUpgradeGUI->addPlayerStat(
                    "AREA", calcX(32, vm), calcY(250, vm), this->lang["AREA"]);
                this->abilityUpgradeGUI->addAbilityUpgrade(
                    "AREA", calcX(44, vm), calcY(596, vm), 3,
                    this->lang["AREA"], "+1", 100, 2, 5);
                this->upgradePlayer("BOMBER");
            }
            else if (player.getName() == "KNIGHT") {
                this->abilityUpgradeGUI->addPlayerStat(
                    "REG", calcX(32, vm), calcY(250, vm), this->lang["REG"]);
                this->abilityUpgradeGUI->addAbilityUpgrade(
                    "REG", calcX(44, vm), calcY(596, vm), 5, this->lang["REG"],
                    "+1", 50, 5, 10);
                this->upgradePlayer("PALADIN");
            }
            else if (player.getName() == "SCOUT") {
                this->abilityUpgradeGUI->addPlayerStat("TARGETS", calcX(32, vm),
                                                       calcY(250, vm),
                                                       this->lang["TARGETS"]);
                this->abilityUpgradeGUI->addAbilityUpgrade(
                    "TARGETS", calcX(44, vm), calcY(596, vm), 8,
                    this->lang["TARGETS"], "+1", 50, 1, 5);
                this->upgradePlayer("KILLER");
            }
        }

        this->upgrading = false;
        return true;
    }
    else if (this->upgradeGUI->hasClickedUpgrade(mousePos, mouseClicked,
                                                 "UPGRADE3", &soundEngine)) {
        if (player.getLevel() == 5) {
            this->abilityUpgradeGUI->addPlayerStat("SLOWDOWN", calcX(32, vm),
                                                   calcY(212, vm),
                                                   this->lang["SLOWDOWN"]);
            this->abilityUpgradeGUI->addAbilityUpgrade(
                "SLOWDOWN", calcX(44, vm), calcY(460, vm), 6,
                this->lang["SLOWDOWN"], "+10%", 30, 5, 10);
            this->upgradePlayer("SCOUT");
        }

        this->upgrading = false;
        return true;
    }

    return false;
}

const uint8_t PlayerGUI::updateDeathScreenButtons(const sf::Vector2i &mousePos,
                                                  bool mouseClicked)
{
    this->text_buttons["MAIN_MENU"]->update(mousePos);
    if (this->text_buttons["MAIN_MENU"]->isPressed() && !mouseClicked) {
        return 1;
    }

    this->text_buttons["QUIT"]->update(mousePos);
    if (this->text_buttons["QUIT"]->isPressed() && !mouseClicked) {
        return 2;
    }
    return 0;
}

void PlayerGUI::updatePlayerAttributes()
{
    this->statsGUI->updateArmor();
    this->statsGUI->updateReg();
    this->statsGUI->updateAttack();
    this->statsGUI->updateAttackSpeed();
    this->statsGUI->updateSpeed();
    this->statsGUI->updateCritical();
    this->updateHP();
    if (player.getHP() < player.getMaxHP()) {
        player.setRegenerating(true);
    }
}

void PlayerGUI::update(sf::Vector2f &mousePosView, float waveCountdown,
                       float bossHP, float dt)
{
    this->statsGUI->updatingXP(dt);

    this->waveCountdown = waveCountdown;

    if (this->bossWave) {
        this->boss_bar_percent = bossHP;

        if (this->boss_bar_percent != 0.f && this->bossCooldown < 255.f) {
            this->bossCooldown += dt * 255.f;
            if (this->bossCooldown > 255.f) {
                this->bossCooldown = 255.f;
            }

            this->texts["BOSS"]->setFillColor(sf::Color(
                113, 43, 59, static_cast<sf::Uint8>(this->bossCooldown)));
            this->sprites["BOSS_BAR_EMPTY"]->setColor(sf::Color(
                255, 255, 255, static_cast<sf::Uint8>(this->bossCooldown)));
            this->sprites["BOSS_BAR"]->setTextureRect(sf::IntRect(
                0, 60, static_cast<int>(this->bossCooldown / 255.f * 512.f),
                20));

            this->sprites["BOSS_BAR"]->setColor(sf::Color(
                255, 255, 255, static_cast<sf::Uint8>(this->bossCooldown)));
        }
        else if (this->boss_bar_percent == 0.f && this->bossCooldown > 0.f) {
            this->bossCooldown -= dt * 255.f;
            if (this->bossCooldown < 0.f) {
                this->bossCooldown = 0.f;
            }

            this->texts["BOSS"]->setFillColor(sf::Color(
                113, 43, 59, static_cast<sf::Uint8>(this->bossCooldown)));
            this->sprites["BOSS_BAR_EMPTY"]->setColor(sf::Color(
                255, 255, 255, static_cast<sf::Uint8>(this->bossCooldown)));
            this->sprites["BOSS_BAR"]->setColor(sf::Color(
                255, 255, 255, static_cast<sf::Uint8>(this->bossCooldown)));
        }
    }

    this->statsGUI->update(mousePosView);

    if (this->waveCountdown < 10.f) {
        this->statsGUI->setWaveCountdownText(
            this->lang["NEXT_WAVE"] +
            std::to_string(static_cast<int>(11.f - this->waveCountdown)));
        if (this->waveCountdown > 8.f && this->waveCountdown < 9.f &&
            this->titleCooldown < 255.f) {
            this->titleCooldown += dt * 1000.f;
            if (this->titleCooldown > 255.f) {
                this->titleCooldown = 255.f;
            }
            this->texts["BIG_WAVE_NUMBER"]->setFillColor(
                sf::Color(255, 255, 255, sf::Uint8(this->titleCooldown)));
            this->texts["WAVE_NEW_MOBS"]->setFillColor(
                sf::Color(228, 92, 95, sf::Uint8(this->titleCooldown)));
            this->texts["MOBS_TO_KILL"]->setFillColor(
                sf::Color(192, 192, 192, sf::Uint8(this->titleCooldown)));
        }
        else if (this->waveCountdown > 9.f && this->titleCooldown > 0.f) {
            this->titleCooldown -= dt * 1000.f;
            if (this->titleCooldown < 0.f) {
                this->titleCooldown = 0.f;
            }
            this->texts["BIG_WAVE_NUMBER"]->setFillColor(
                sf::Color(255, 255, 255, sf::Uint8(this->titleCooldown)));
            this->texts["WAVE_NEW_MOBS"]->setFillColor(
                sf::Color(228, 92, 95, sf::Uint8(this->titleCooldown)));
            this->texts["MOBS_TO_KILL"]->setFillColor(
                sf::Color(192, 192, 192, sf::Uint8(this->titleCooldown)));
        }

        this->statsGUI->setWaveCountdownText(
            this->lang["NEXT_WAVE"] +
            std::to_string(static_cast<int>(10.f - this->waveCountdown)));
    }
}

void PlayerGUI::draw(sf::RenderTarget &target)
{
    this->statsGUI->draw(target);

    if (isShopping()) {
        this->sprites["SIDE_GUI"]->draw(target);
        this->shopGUI->draw(target);
    }

    if (player.isUpgraded()) {
        statsGUI->drawAbility(target);
        if (isBuyingAbility() && !isShopping()) {
            this->sprites["SIDE_GUI"]->draw(target);
            this->abilityUpgradeGUI->draw(target);
        }
    }

    if (this->waveCountdown < 10.f) {
        this->statsGUI->drawWaveCountdown(target);
        if (this->waveCountdown > 8.f) {
            this->texts["BIG_WAVE_NUMBER"]->draw(target);
            this->texts["WAVE_NEW_MOBS"]->draw(target);
            this->texts["MOBS_TO_KILL"]->draw(target);
        }
    }
    else if (this->waveCountdown >= 10.f) {
        this->statsGUI->drawMonsterCount(target);
        if (this->leveling) {
            this->texts["LEVEL_UP"]->draw(target);
            this->sprites["LEVEL_UP"]->draw(target);
            this->option1.optionSprite->draw(target);
            this->option1.optionButton->draw(target);
            this->option1.optionDesc->draw(target);
            this->option1.optionValue->draw(target);
            this->option2.optionSprite->draw(target);
            this->option2.optionButton->draw(target);
            this->option2.optionDesc->draw(target);
            this->option2.optionValue->draw(target);
        }

        if (this->upgrading) {
            this->sprites["UPGRADES"]->draw(target);
            this->upgradeGUI->draw(target);
        }

        if (this->bossWave) {
            this->texts["BOSS"]->draw(target);
            this->sprites["BOSS_BAR_EMPTY"]->draw(target);
            this->sprites["BOSS_BAR"]->draw(target);
        }
    }

    if (player.isDead()) {
        target.draw(this->death_background);
        this->texts["YOU_DIED"]->draw(target);
        this->texts["KILLS"]->draw(target);
        this->text_buttons["MAIN_MENU"]->draw(target);
        this->text_buttons["QUIT"]->draw(target);
    }
    else if (this->escape) {
        target.draw(this->escape_background);
        this->texts["DIFFICULTY"]->draw(target);
        this->text_buttons["RESUME"]->draw(target);
        this->text_buttons["MAIN_MENU"]->draw(target);
        this->text_buttons["QUIT"]->draw(target);
        this->text_buttons["SETTINGS"]->draw(target);
    }

    this->statsGUI->drawMenu(target);
}
