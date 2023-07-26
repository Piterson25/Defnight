#include "PlayerGUI.hpp"

PlayerGUI::PlayerGUI(sf::VideoMode &vm, Player &player, float soundVolume,
                     const std::string &heroName,
                     const std::string &difficulty_name,
                     std::unordered_map<std::string, std::string> &lang)
    : vm(vm), lang(lang), player(player)
{
    this->attributes_texture.loadFromFile(
        "assets/textures/attributes_icons.png");
    this->upgrades_texture.loadFromFile("assets/textures/upgrades_icons.png");
    this->abilities_texture.loadFromFile("assets/textures/abilities_icons.png");
    this->select_texture.loadFromFile("assets/textures/select.png");

    this->statsGUI = new StatsGUI(this->vm, this->player, this->lang);

    this->titleCooldown = 0.f;
    this->texts["WAVE_NEW_MOBS"] =
        new gui::Text("", calcChar(16, vm), calcX(640, vm), calcY(562, vm),
                      sf::Color(228, 92, 95), true);
    this->texts["BIG_WAVE_NUMBER"] =
        new gui::Text(this->lang["WAVE"], calcChar(64, vm), calcX(640, vm),
                      calcY(256, vm), sf::Color(255, 255, 255), true);
    this->texts["MOBS_TO_KILL"] =
        new gui::Text(this->lang["MONSTER"], calcChar(32, vm), calcX(640, vm),
                      calcY(512, vm), sf::Color(192, 192, 192), true);
    this->waveCountdown = 0.f;

    this->leveling = false;

    this->texts["LEVEL_UP"] =
        new gui::Text("LEVEL UP!", calcChar(32, vm), calcX(640, vm),
                      calcY(256, vm), sf::Color(255, 246, 76), true);
    this->sprites["LEVEL_UP"] =
        new gui::Sprite("assets/textures/bottom_gui.png", calcX(640, vm),
                        calcY(476, vm), calcScale(1, vm), true);

    this->sprites["OPTION1_FRAME"] =
        new gui::Sprite(this->select_texture, calcX(504, vm), calcY(512, vm),
                        calcScale(1, vm), false);
    this->sprites["OPTION1_FRAME"]->setTextureRect(sf::IntRect(88, 0, 88, 88));
    this->sprites["OPTION2_FRAME"] =
        new gui::Sprite(this->select_texture, calcX(688, vm), calcY(512, vm),
                        calcScale(1, vm), false);
    this->sprites["OPTION2_FRAME"]->setTextureRect(sf::IntRect(88, 0, 88, 88));
    this->sprite_buttons["OPTION1"] =
        new gui::ButtonSprite(this->select_texture, calcX(504, vm),
                              calcY(512, vm), calcScale(1, vm), false);
    this->sprite_buttons["OPTION1"]->setTextureRect(sf::IntRect(0, 0, 88, 88));
    this->sprite_buttons["OPTION2"] =
        new gui::ButtonSprite(this->select_texture, calcX(688, vm),
                              calcY(512, vm), calcScale(1, vm), false);
    this->sprite_buttons["OPTION2"]->setTextureRect(sf::IntRect(0, 0, 88, 88));
    this->sprites["OPTION1"] =
        new gui::Sprite(this->attributes_texture, calcX(516, vm),
                        calcY(524, vm), calcScale(4, vm), false);
    this->sprites["OPTION2"] =
        new gui::Sprite(this->attributes_texture, calcX(700, vm),
                        calcY(524, vm), calcScale(4, vm), false);
    this->texts["OPTION1"] =
        new gui::Text("", calcChar(16, vm), calcX(548, vm), calcY(612, vm),
                      sf::Color(255, 255, 255), false);
    this->texts["OPTION2"] =
        new gui::Text("", calcChar(16, vm), calcX(732, vm), calcY(612, vm),
                      sf::Color(255, 255, 255), false);
    this->texts["OPTION1_VALUE"] =
        new gui::Text("", calcChar(16, vm), calcX(548, vm), calcY(636, vm),
                      sf::Color(255, 255, 255), false);
    this->texts["OPTION2_VALUE"] =
        new gui::Text("", calcChar(16, vm), calcX(732, vm), calcY(636, vm),
                      sf::Color(255, 255, 255), false);

    this->option1_id = 0;
    this->option1_val = 0;
    this->option2_id = 0;
    this->option2_val = 0;

    this->sprites["UPGRADES"] =
        new gui::Sprite("assets/textures/side_gui.png", calcX(1280, vm),
                        calcY(128, vm), calcScale(1, vm), false);
    this->sprites["UPGRADES"]->flipHorizontal();

    this->upgradeGUI = new UpgradeGUI(vm, this->player);
    this->upgrading = false;
    this->upgradeGUI->changeUpgrade("UPGRADE1", calcX(1036, vm), calcY(222, vm),
                                    this->lang["NINJA"], 1, 0, 7, 1);
    this->upgradeGUI->changeUpgrade("UPGRADE2", calcX(1036, vm), calcY(392, vm),
                                    this->lang["KNIGHT"], 2, 1, 5, 1);
    this->upgradeGUI->changeUpgrade("UPGRADE3", calcX(1036, vm), calcY(562, vm),
                                    this->lang["SCOUT"], 3, 2, 2, 1);

    this->death_background.setFillColor(sf::Color(182, 60, 53, 192));
    this->death_background.setSize(
        sf::Vector2f(calcX(1280, vm), calcY(592, vm)));
    this->death_background.setPosition(sf::Vector2f(0, calcY(128, vm)));

    this->texts["YOU_DIED"] =
        new gui::Text(this->lang["YOU_DIED"], calcChar(72, vm), calcX(640, vm),
                      calcY(224, vm), sf::Color(255, 255, 255), true);
    this->text_buttons["RESUME"] = new gui::ButtonText(
        this->lang["RESUME"], calcChar(32, vm), calcX(640, vm), calcY(296, vm),
        sf::Color(255, 255, 255), sf::Color(192, 192, 192), true);
    this->text_buttons["MAIN_MENU"] = new gui::ButtonText(
        this->lang["MAIN_MENU"], calcChar(32, vm), calcX(640, vm),
        calcY(392, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192),
        true);
    this->text_buttons["SETTINGS"] = new gui::ButtonText(
        this->lang["SETTINGS"], calcChar(32, vm), calcX(640, vm),
        calcY(488, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192),
        true);
    this->text_buttons["QUIT"] = new gui::ButtonText(
        this->lang["QUIT"], calcChar(32, vm), calcX(640, vm), calcY(584, vm),
        sf::Color(255, 255, 255), sf::Color(192, 192, 192), true);

    this->texts["KILLS"] =
        new gui::Text(this->lang["KILLS"] + std::to_string(player.getKills()),
                      calcChar(16, vm), calcX(640, vm), calcY(160, vm),
                      sf::Color(192, 192, 192), true);

    if (difficulty_name == "easy") {
        this->texts["DIFFICULTY"] = new gui::Text(
            this->lang["DIFFICULTY_LEVEL"] + " " + this->lang["EASY"],
            calcChar(16, vm), calcX(640, vm), calcY(144, vm), gui::RED, true);
    }
    else if (difficulty_name == "normal") {
        this->texts["DIFFICULTY"] = new gui::Text(
            this->lang["DIFFICULTY_LEVEL"] + " " + this->lang["NORMAL"],
            calcChar(16, vm), calcX(640, vm), calcY(144, vm), gui::RED, true);
    }
    else if (difficulty_name == "hard") {
        this->texts["DIFFICULTY"] = new gui::Text(
            this->lang["DIFFICULTY_LEVEL"] + " " + this->lang["HARD"],
            calcChar(16, vm), calcX(640, vm), calcY(144, vm), gui::RED, true);
    }

    this->escape_background.setFillColor(sf::Color(0, 0, 0, 192));
    this->escape_background.setSize(
        sf::Vector2f(calcX(1280, vm), calcY(720, vm)));
    this->escape = false;

    this->sprites["SIDE_GUI"] =
        new gui::Sprite("assets/textures/side_gui.png", 0, calcY(128, vm),
                        calcScale(1, vm), false);

    this->abilityUpgradeGUI = new AbilityUpgradeGUI(vm, this->player);
    this->abilityUpgradeGUI->addAbilityUpgrade("LOWER_COOLDOWN", calcX(44, vm),
                                               calcY(320, vm), 0, "Cooldown",
                                               "-10%", 20);

    this->shopGUI = new ShopGUI(vm, this->player);
    this->shopGUI->addShopItem("FULL_HP", calcX(44, vm), calcY(192, vm), 9,
                               "Full HP", "+Full", 30);
    this->shopGUI->addShopItem("MAX_HP", calcX(44, vm), calcY(320, vm), 3,
                               this->lang["MAX_HP"], "+2", 20);
    this->shopGUI->addShopItem("ATTACK", calcX(44, vm), calcY(448, vm), 5,
                               this->lang["ATTACK"], "+1", 15);
    this->shopGUI->addShopItem("ARMOR", calcX(44, vm), calcY(576, vm), 1,
                               this->lang["ARMOR"], "+1", 10);

    this->sideGUI = SideGUI::NONE;

    this->bossWave = false;
    this->bossCooldown = 0.f;

    this->texts["BOSS"] =
        new gui::Text("Minotaur", calcChar(16, vm), calcX(640, vm),
                      calcY(136, vm), sf::Color(113, 43, 59), true);
    this->sprites["BOSS_BAR"] =
        new gui::Sprite("assets/textures/bars.png", calcX(376, vm),
                        calcY(158, vm), calcScale(1, vm), false);
    this->sprites["BOSS_BAR"]->setTextureRect(sf::IntRect(0, 60, 528, 20));
    this->sprites["BOSS_BAR_EMPTY"] =
        new gui::Sprite("assets/textures/bars.png", calcX(376, vm),
                        calcY(158, vm), calcScale(1, vm), false);
    this->sprites["BOSS_BAR_EMPTY"]->setTextureRect(
        sf::IntRect(0, 80, 528, 20));

    this->boss_bar_percent = 1.f;
}

PlayerGUI::~PlayerGUI()
{
    delete this->statsGUI;
    delete this->shopGUI;
    delete this->upgradeGUI;
    delete this->abilityUpgradeGUI;
}

void PlayerGUI::update_options(uint32_t &option_id, uint32_t &option_val,
                               std::vector<short> &id_vector, gui::Text *text,
                               gui::Text *value, gui::Sprite *sprite, float pos)
{
    const short id = static_cast<short>(Random::Float() * id_vector.size());

    option_id = id_vector[id];
    switch (option_id) {
        case 1:
            option_val = 1;
            text->setText(this->lang["ARMOR"]);
            break;
        case 2:
            option_val = 1;
            text->setText(this->lang["REG"]);
            break;
        case 3:
            option_val = static_cast<uint32_t>(Random::Float() * 2.f) + 2;
            text->setText(this->lang["MAX_HP"]);
            break;
        case 4:
            option_val = 50;
            text->setText(this->lang["SPRINT"]);
            break;
        case 5:
            option_val = 1;
            text->setText(this->lang["ATTACK"]);
            break;
        case 6:
            option_val = 1;
            text->setText(this->lang["ATTACK_SPEED"]);
            break;
        case 7:
            option_val = 1;
            text->setText(this->lang["SPEED"]);
            break;
        case 8:
            option_val = 10;
            text->setText(this->lang["CRITICAL"]);
            break;
    }
    value->setText("+" + std::to_string(option_val));
    if (option_id == 8) {
        value->setText(value->getText() + "%");
    }
    value->center(pos);
    id_vector.erase(id_vector.begin() + id);
    sprite->setTextureRect(sf::IntRect(option_id * 16, 0, 16, 16));
    text->center(pos);
}

void PlayerGUI::levelUpPlayer(uint32_t option_id, uint32_t option_val)
{
    const sf::VideoMode vm = this->vm;

    switch (option_id) {
        case 1:
            player.setArmor(player.getArmor() + option_val);
            statsGUI->updateArmor();
            break;
        case 2:
            player.setReg(player.getReg() + option_val);
            statsGUI->updateReg();
            break;
        case 3:
            player.setMaxHP(player.getMaxHP() + option_val);
            this->updateHP();
            player.setRegenerating(true);
            break;
        case 4:
            player.setMaxSprint(player.getMaxSprint() + option_val);
            statsGUI->updateSprint();
            break;
        case 5:
            player.setAttack(player.getAttack() + option_val);
            statsGUI->updateAttack();
            break;
        case 6:
            player.setAttackSpeed(player.getAttackSpeed() + option_val);
            statsGUI->updateAttackSpeed();
            break;
        case 7:
            player.setSpeed(player.getSpeed() + option_val);
            statsGUI->updateSpeed();
            break;
        case 8:
            player.setCriticalChance(player.getCriticalChance() + option_val);
            statsGUI->updateCritical();
            break;
    }
}

void PlayerGUI::upgradePlayer(const std::string &name)
{
    sf::IntRect intRect = sf::IntRect(0, 0, 16, 16);
    player.upgrade(name, intRect);
    statsGUI->upgradePlayer(this->lang[name], intRect);
    this->abilityUpgradeGUI->updatePlayerInfo("COOLDOWN", "Cooldown");
    this->abilityUpgradeGUI->updatePlayerInfo("ATTACK", this->lang["ATTACK"]);
    this->abilityUpgradeGUI->updatePlayerInfo("PIERCING",
                                              this->lang["PIERCING"]);
    this->abilityUpgradeGUI->updatePlayerInfo("AREA", this->lang["AREA"]);
    this->abilityUpgradeGUI->updatePlayerInfo("ARMOR", this->lang["ARMOR"]);
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
    if (player.getArmor() + player.getIncreasedArmor() < 20) {
        id.push_back(1);
    }
    else {
        shopGUI->deleteItem("ARMOR");
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

    update_options(this->option1_id, this->option1_val, id,
                   this->texts["OPTION1"], this->texts["OPTION1_VALUE"],
                   this->sprites["OPTION1"], calcX(548, vm));
    update_options(this->option2_id, this->option2_val, id,
                   this->texts["OPTION2"], this->texts["OPTION2_VALUE"],
                   this->sprites["OPTION2"], calcX(732, vm));

    if (player.getLevel() == 10) {
        if (player.getName() == "ninja") {
            this->upgradeGUI->changeUpgrade("UPGRADE1", calcX(1036, vm),
                                            calcY(222, vm),
                                            this->lang["SENSEI"], 4, 3, 6, 1);
            this->upgradeGUI->changeUpgrade("UPGRADE2", calcX(1036, vm),
                                            calcY(392, vm),
                                            this->lang["BOMBER"], 5, 4, 8, 1);
        }
        else if (player.getName() == "knight") {
            this->upgradeGUI->changeUpgrade("UPGRADE1", calcX(1036, vm),
                                            calcY(210, vm),
                                            this->lang["CRUSADER"], 6, 5, 3, 2);
            this->upgradeGUI->changeUpgrade("UPGRADE2", calcX(1036, vm),
                                            calcY(380, vm),
                                            this->lang["PALADIN"], 7, 6, 6, 1);
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
        this->statsGUI->updateAbilityIcon(value);
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
        const int barrier = static_cast<int>(this->boss_bar_percent * 528.f);

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
                                        SoundEngine &soundEngine,
                                        FloatingTextSystem &floatingTextSystem)
{
    if (statsGUI->hasClickedShop(mousePos, mouseClicked)) {
        updateIsShopping();
        return true;
    }

    if (this->isShopping()) {
        if (shopGUI->hasBoughtItem(mousePos, mouseClicked, "FULL_HP",
                                   &floatingTextSystem, &soundEngine)) {
            player.setHP(player.getMaxHP());
            this->updateHP();
            player.setRegenerating(true);
            this->update_Gold();
            return true;
        }
        else if (shopGUI->hasBoughtItem(mousePos, mouseClicked, "MAX_HP",
                                        &floatingTextSystem, &soundEngine)) {
            player.setMaxHP(player.getMaxHP() + 2);
            this->updateHP();
            player.setRegenerating(true);
            this->update_Gold();
            return true;
        }
        else if (shopGUI->hasBoughtItem(mousePos, mouseClicked, "ATTACK",
                                        &floatingTextSystem, &soundEngine)) {
            player.setAttack(player.getAttack() + 1);
            this->update_Gold();
            return true;
        }
        else if (shopGUI->hasBoughtItem(mousePos, mouseClicked, "ARMOR",
                                        &floatingTextSystem, &soundEngine)) {
            player.setArmor(player.getArmor() + 1);
            this->update_Gold();
            if (player.getArmor() + player.getIncreasedArmor() < 20) {
                shopGUI->deleteItem("ARMOR");
            }
            return true;
        }
    }

    return false;
}

const bool
PlayerGUI::hasClickedAbilityBuy(const sf::Vector2i &mousePos, bool mouseClicked,
                                SoundEngine &soundEngine,
                                FloatingTextSystem &floatingTextSystem)
{
    if (this->statsGUI->hasClickedAbilityUpgrade(mousePos, mouseClicked)) {
        updateIsBuyingAbility();
        return true;
    }

    if (this->isBuyingAbility()) {
        if (abilityUpgradeGUI->hasBoughtUpgrade(
                mousePos, mouseClicked, "LOWER_COOLDOWN", &floatingTextSystem,
                &soundEngine)) {
            player.setAbilityMaxTimeModifier(
                player.getAbilityMaxTimeModifier() - 0.1f);
            this->update_Gold();
            this->abilityUpgradeGUI->updatePlayerInfo("COOLDOWN", "Cooldown");
            return true;
        }
        else if (abilityUpgradeGUI->hasBoughtUpgrade(
                     mousePos, mouseClicked, "ATTACK", &floatingTextSystem,
                     &soundEngine)) {
            player.setAttackIncrease(player.getAttackIncrease() + 1);
            this->update_Gold();
            this->abilityUpgradeGUI->updatePlayerInfo("ATTACK",
                                                      this->lang["ATTACK"]);
            return true;
        }
        else if (abilityUpgradeGUI->hasBoughtUpgrade(
                     mousePos, mouseClicked, "PIERCING", &floatingTextSystem,
                     &soundEngine)) {
            player.setProjectilePiercing(player.getProjectilePiercing() + 1);
            this->update_Gold();
            this->abilityUpgradeGUI->updatePlayerInfo("PIERCING",
                                                      this->lang["PIERCING"]);
            return true;
        }
        else if (abilityUpgradeGUI->hasBoughtUpgrade(
                     mousePos, mouseClicked, "AREA", &floatingTextSystem,
                     &soundEngine)) {
            player.setProjectileArea(player.getProjectileArea() + 1);
            this->update_Gold();
            this->abilityUpgradeGUI->updatePlayerInfo("AREA",
                                                      this->lang["AREA"]);
            return true;
        }
        else if (this->player.getArmor() + this->player.getIncreasedArmor() <
                     20 &&
                 abilityUpgradeGUI->hasBoughtUpgrade(
                     mousePos, mouseClicked, "ARMOR", &floatingTextSystem,
                     &soundEngine)) {
            player.setIncreasedArmor(player.getIncreasedArmor() + 1);
            this->update_Gold();
            this->abilityUpgradeGUI->updatePlayerInfo("ARMOR",
                                                      this->lang["ARMOR"]);
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
    this->sprite_buttons["OPTION1"]->update(mousePos);
    if (this->sprite_buttons["OPTION1"]->isPressed() && !mouseClicked) {
        this->levelUpPlayer(this->option1_id, this->option1_val);
        this->leveling = false;
        this->sprite_buttons["OPTION1"]->setTransparent();
        soundEngine.addSound("option");
        return true;
    }

    this->sprite_buttons["OPTION2"]->update(mousePos);
    if (this->sprite_buttons["OPTION2"]->isPressed() && !mouseClicked) {
        this->levelUpPlayer(this->option2_id, this->option2_val);
        this->leveling = false;
        this->sprite_buttons["OPTION2"]->setTransparent();
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
            this->abilityUpgradeGUI->addPlayerStat(
                "ATTACK", calcX(32, vm), calcY(216, vm), this->lang["ATTACK"]);
            this->abilityUpgradeGUI->addAbilityUpgrade(
                "ATTACK", calcX(44, vm), calcY(448, vm), 1,
                this->lang["ATTACK"], "+1", 30);
            this->upgradePlayer("NINJA");
        }
        else if (player.getLevel() == 10) {
            if (player.getName() == "ninja") {
                this->abilityUpgradeGUI->addPlayerStat(
                    "PIERCING", calcX(32, vm), calcY(254, vm),
                    this->lang["PIERCING"]);
                this->abilityUpgradeGUI->addAbilityUpgrade(
                    "PIERCING", calcX(44, vm), calcY(576, vm), 2,
                    this->lang["PIERCING"], "+1", 50);
                this->upgradePlayer("SENSEI");
            }
            else if (player.getName() == "knight") {
                this->upgradePlayer("CRUSADER");
            }
        }

        this->upgrading = false;
        this->update_Gold();
        return true;
    }
    else if (this->upgradeGUI->hasClickedUpgrade(mousePos, mouseClicked,
                                                 "UPGRADE2", &soundEngine)) {
        if (player.getLevel() == 5) {
            this->abilityUpgradeGUI->addPlayerStat(
                "ARMOR", calcX(32, vm), calcY(216, vm), this->lang["ARMOR"]);
            this->abilityUpgradeGUI->addAbilityUpgrade(
                "ARMOR", calcX(44, vm), calcY(448, vm), 4, this->lang["ARMOR"],
                "+1", 30);
            this->upgradePlayer("KNIGHT");
        }
        else if (player.getLevel() == 10) {
            if (player.getName() == "ninja") {
                this->abilityUpgradeGUI->addPlayerStat(
                    "AREA", calcX(32, vm), calcY(254, vm), this->lang["AREA"]);
                this->abilityUpgradeGUI->addAbilityUpgrade(
                    "AREA", calcX(44, vm), calcY(576, vm), 3,
                    this->lang["AREA"], "+1", 100);
                this->upgradePlayer("BOMBER");
            }
            else if (player.getName() == "knight") {
                this->upgradePlayer("PALADIN");
            }
        }

        this->upgrading = false;
        this->update_Gold();
        return true;
    }

    if (player.getLevel() == 5) {
        if (this->upgradeGUI->hasClickedUpgrade(mousePos, mouseClicked,
                                                "UPGRADE3", &soundEngine)) {
            this->upgradePlayer("SCOUT");
            this->upgrading = false;
            this->update_Gold();
            return true;
        }
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
                0, 60, static_cast<int>(this->bossCooldown / 255.f * 528.f),
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

            this->sprites["OPTION1_FRAME"]->draw(target);
            this->sprites["OPTION1"]->draw(target);
            this->sprite_buttons["OPTION1"]->draw(target);
            this->texts["OPTION1"]->draw(target);
            this->texts["OPTION1_VALUE"]->draw(target);
            this->sprites["OPTION2_FRAME"]->draw(target);
            this->sprites["OPTION2"]->draw(target);
            this->sprite_buttons["OPTION2"]->draw(target);
            this->texts["OPTION2"]->draw(target);
            this->texts["OPTION2_VALUE"]->draw(target);
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
}
