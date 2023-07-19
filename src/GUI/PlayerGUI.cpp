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

    this->sprites["TOP_GUI"] = new gui::Sprite("assets/textures/top_gui.png", 0,
                                               0, calcScale(1, vm), false);

    this->sprites["PROGRESS_BAR"] =
        new gui::Sprite("assets/textures/progress_bar.png", calcX(640, vm),
                        calcY(44, vm), calcScale(1, vm), true);
    this->sprites["PROGRESS_BAR"]->setColor(sf::Color::Transparent);

    this->sprites["MINIATURE"] =
        new gui::Sprite(this->upgrades_texture, calcX(164, vm), calcX(16, vm),
                        calcScale(4, vm), false);
    this->sprites["MINIATURE"]->setTextureRect(sf::IntRect(0, 0, 16, 16));
    this->texts["NAME"] =
        new gui::Text(lang["WARRIOR"], calcChar(16, vm), calcX(196, vm),
                      calcY(96, vm), sf::Color(255, 255, 255), true);

    this->sprites["GOLD"] = new gui::Sprite(
        attributes_texture, 0, calcY(86, vm), calcScale(2, vm), false);
    this->sprites["GOLD"]->setTextureRect(sf::IntRect(0, 0, 16, 16));
    this->texts["GOLD"] = new gui::Text(
        std::to_string(player.getGold()), calcChar(16, vm), calcX(36, vm),
        calcY(96, vm), sf::Color(255, 246, 76), false);

    this->sprites["ARMOR"] =
        new gui::Sprite(attributes_texture, calcX(372, vm), calcY(16, vm),
                        calcScale(2, vm), false);
    this->sprites["ARMOR"]->setTextureRect(sf::IntRect(16, 0, 16, 16));
    this->texts["ARMOR"] = new gui::Text(
        std::to_string(player.getArmor()), calcChar(16, vm), calcX(388, vm),
        calcY(70, vm), sf::Color(192, 192, 192), true);

    this->sprites["REG"] =
        new gui::Sprite(attributes_texture, calcX(436, vm), calcY(16, vm),
                        calcScale(2, vm), false);
    this->sprites["REG"]->setTextureRect(sf::IntRect(32, 0, 16, 16));
    this->texts["REG"] = new gui::Text(
        std::to_string(player.getReg()), calcChar(16, vm), calcX(452, vm),
        calcY(70, vm), sf::Color(182, 60, 53), true);

    this->sprites["ATTACK"] =
        new gui::Sprite(attributes_texture, calcX(812, vm), calcY(16, vm),
                        calcScale(2, vm), false);
    this->sprites["ATTACK"]->setTextureRect(sf::IntRect(80, 0, 16, 16));
    this->texts["ATTACK"] = new gui::Text(
        std::to_string(player.getAttack()), calcChar(16, vm), calcX(828, vm),
        calcY(70, vm), sf::Color(192, 192, 192), true);

    this->sprites["ATTACK_SPEED"] =
        new gui::Sprite(attributes_texture, calcX(876, vm), calcY(16, vm),
                        calcScale(2, vm), false);
    this->sprites["ATTACK_SPEED"]->setTextureRect(sf::IntRect(96, 0, 16, 16));
    this->texts["ATTACK_SPEED"] = new gui::Text(
        std::to_string(player.getAttackSpeed()), calcChar(16, vm),
        calcX(892, vm), calcY(70, vm), sf::Color(192, 192, 192), true);

    this->sprites["SPEED"] =
        new gui::Sprite(attributes_texture, calcX(940, vm), calcY(16, vm),
                        calcScale(2, vm), false);
    this->sprites["SPEED"]->setTextureRect(sf::IntRect(112, 0, 16, 16));
    this->texts["SPEED"] = new gui::Text(
        std::to_string(player.getSpeed()), calcChar(16, vm), calcX(956, vm),
        calcY(70, vm), sf::Color(192, 192, 192), true);

    this->sprites["CRITICAL"] =
        new gui::Sprite(attributes_texture, calcX(1004, vm), calcY(16, vm),
                        calcScale(2, vm), false);
    this->sprites["CRITICAL"]->setTextureRect(sf::IntRect(128, 0, 16, 16));
    this->texts["CRITICAL"] = new gui::Text(
        std::to_string(player.getCriticalChance()) + "%", calcChar(16, vm),
        calcX(1020, vm), calcY(70, vm), sf::Color(192, 192, 192), true);

    this->sprites["XP_BAR"] =
        new gui::Sprite("assets/textures/bars.png", calcX(508, vm),
                        calcY(12, vm), calcScale(1, vm), false);
    this->sprites["XP_BAR"]->setTextureRect(sf::IntRect(0, 0, 0, 0));
    this->texts["XP"] =
        new gui::Text("XP:" + std::to_string(player.getXP()) + "/" +
                          std::to_string(player.getMaxXP()),
                      calcChar(16, vm), calcX(640, vm), calcY(15, vm),
                      sf::Color(255, 255, 255), true);
    this->texts["LEVEL"] = new gui::Text(
        "Level " + std::to_string(player.getLevel()), calcChar(16, vm),
        calcX(640, vm), calcY(15, vm), sf::Color(255, 255, 255), true);
    this->levelShown = true;
    this->xp_bar_percent = 0.f;

    this->sprites["HP_BAR"] =
        new gui::Sprite("assets/textures/bars.png", calcX(508, vm),
                        calcY(52, vm), calcScale(1, vm), false);
    this->sprites["HP_BAR"]->setTextureRect(sf::IntRect(0, 20, 264, 20));
    this->texts["HP"] =
        new gui::Text("HP:" + std::to_string(player.getHP()) + "/" +
                          std::to_string(player.getMaxHP()),
                      calcChar(16, vm), calcX(640, vm), calcY(55, vm),
                      sf::Color(255, 255, 255), true);
    this->hp_bar_percent = 1.f;

    this->sprites["SPRINT_BAR"] =
        new gui::Sprite("assets/textures/bars.png", calcX(508, vm),
                        calcY(92, vm), calcScale(1, vm), false);
    this->sprites["SPRINT_BAR"]->setTextureRect(sf::IntRect(0, 40, 264, 20));
    this->texts["SPRINT"] =
        new gui::Text(std::to_string(player.getSprint()) + "/" +
                          std::to_string(player.getMaxSprint()),
                      calcChar(16, vm), calcX(640, vm), calcY(95, vm),
                      sf::Color(255, 255, 255), true);

    this->texts["MONSTER_COUNT"] = new gui::Text(
        this->lang["MONSTER_COUNT"], calcChar(16, vm), calcX(906, vm),
        calcY(96, vm), sf::Color(130, 139, 152), false);
    this->texts["MONSTER_COUNT"]->setPosition(
        sf::Vector2f(calcX(1218, vm) - this->texts["MONSTER_COUNT"]->getWidth(),
                     calcY(96, vm)));

    this->texts["WAVE_NUMBER"] =
        new gui::Text(this->lang["WAVE"], calcChar(32, vm), calcX(1060, vm),
                      calcY(4, vm), sf::Color(228, 92, 95), false);
    this->texts["WAVE_COUNTDOWN"] =
        new gui::Text(this->lang["NEXT_WAVE"], calcChar(16, vm), calcX(954, vm),
                      calcY(96, vm), sf::Color(255, 246, 76), false);
    this->texts["WAVE_COUNTDOWN"]->setPosition(sf::Vector2f(
        calcX(1218, vm) - this->texts["WAVE_COUNTDOWN"]->getWidth(),
        calcY(96, vm)));
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

    this->upgrading = false;
    this->sprites["UPGRADES"] =
        new gui::Sprite("assets/textures/side_gui.png", calcX(1280, vm),
                        calcY(128, vm), calcScale(1, vm), false);
    this->sprites["UPGRADES"]->flipHorizontal();

    this->sprites["UPGRADE1_FRAME"] =
        new gui::Sprite(this->select_texture, calcX(1008, vm), calcY(210, vm),
                        calcScale(1, vm), false);
    this->sprites["UPGRADE1_FRAME"]->setTextureRect(sf::IntRect(88, 0, 88, 88));
    this->sprites["UPGRADE2_FRAME"] =
        new gui::Sprite(this->select_texture, calcX(1008, vm), calcY(380, vm),
                        calcScale(1, vm), false);
    this->sprites["UPGRADE2_FRAME"]->setTextureRect(sf::IntRect(88, 0, 88, 88));
    this->sprites["UPGRADE3_FRAME"] =
        new gui::Sprite(this->select_texture, calcX(1008, vm), calcY(550, vm),
                        calcScale(1, vm), false);
    this->sprites["UPGRADE3_FRAME"]->setTextureRect(sf::IntRect(88, 0, 88, 88));
    this->sprite_buttons["UPGRADE1"] =
        new gui::ButtonSprite(this->select_texture, calcX(1008, vm),
                              calcY(210, vm), calcScale(1, vm), false);
    this->sprite_buttons["UPGRADE1"]->setTextureRect(sf::IntRect(0, 0, 88, 88));
    this->sprite_buttons["UPGRADE2"] =
        new gui::ButtonSprite(this->select_texture, calcX(1008, vm),
                              calcY(380, vm), calcScale(1, vm), false);
    this->sprite_buttons["UPGRADE2"]->setTextureRect(sf::IntRect(0, 0, 88, 88));
    this->sprite_buttons["UPGRADE3"] =
        new gui::ButtonSprite(this->select_texture, calcX(1008, vm),
                              calcY(550, vm), calcScale(1, vm), false);
    this->sprite_buttons["UPGRADE3"]->setTextureRect(sf::IntRect(0, 0, 88, 88));

    this->sprites["UPGRADE1"] =
        new gui::Sprite(this->upgrades_texture, calcX(1020, vm), calcY(222, vm),
                        calcScale(4, vm), false);
    this->sprites["UPGRADE1"]->setTextureRect(sf::IntRect(16, 0, 16, 16));
    this->sprites["UPGRADE2"] =
        new gui::Sprite(this->upgrades_texture, calcX(1020, vm), calcY(392, vm),
                        calcScale(4, vm), false);
    this->sprites["UPGRADE2"]->setTextureRect(sf::IntRect(32, 0, 16, 16));
    this->sprites["UPGRADE3"] =
        new gui::Sprite(this->upgrades_texture, calcX(1020, vm), calcY(562, vm),
                        calcScale(4, vm), false);
    this->sprites["UPGRADE3"]->setTextureRect(sf::IntRect(48, 0, 16, 16));

    this->texts["UPGRADE1_NAME"] =
        new gui::Text(this->lang["NINJA"], calcChar(16, vm), calcX(1112, vm),
                      calcY(226, vm), sf::Color(255, 255, 255), false);
    this->texts["UPGRADE2_NAME"] =
        new gui::Text(this->lang["KNIGHT"], calcChar(16, vm), calcX(1112, vm),
                      calcY(396, vm), sf::Color(255, 255, 255), false);
    this->texts["UPGRADE3_NAME"] =
        new gui::Text(this->lang["SCOUT"], calcChar(16, vm), calcX(1112, vm),
                      calcY(566, vm), sf::Color(255, 255, 255), false);

    this->sprites["UPGRADE1_ABILITY"] =
        new gui::Sprite(this->abilities_texture, calcX(1112, vm),
                        calcY(256, vm), calcScale(2, vm), false);
    this->sprites["UPGRADE1_ABILITY"]->setTextureRect(
        sf::IntRect(0, 0, 16, 16));
    this->sprites["UPGRADE2_ABILITY"] =
        new gui::Sprite(this->abilities_texture, calcX(1112, vm),
                        calcY(426, vm), calcScale(2, vm), false);
    this->sprites["UPGRADE2_ABILITY"]->setTextureRect(
        sf::IntRect(16, 0, 16, 16));
    this->sprites["UPGRADE3_ABILITY"] =
        new gui::Sprite(this->abilities_texture, calcX(1112, vm),
                        calcY(596, vm), calcScale(2, vm), false);
    this->sprites["UPGRADE3_ABILITY"]->setTextureRect(
        sf::IntRect(32, 0, 16, 16));

    this->sprites["UPGRADE1_ADD"] =
        new gui::Sprite(this->attributes_texture, calcX(1184, vm),
                        calcY(256, vm), calcScale(2, vm), false);
    this->sprites["UPGRADE1_ADD"]->setTextureRect(sf::IntRect(112, 0, 16, 16));
    this->sprites["UPGRADE2_ADD"] =
        new gui::Sprite(this->attributes_texture, calcX(1184, vm),
                        calcY(426, vm), calcScale(2, vm), false);
    this->sprites["UPGRADE2_ADD"]->setTextureRect(sf::IntRect(80, 0, 16, 16));
    this->sprites["UPGRADE3_ADD"] =
        new gui::Sprite(this->attributes_texture, calcX(1184, vm),
                        calcY(596, vm), calcScale(2, vm), false);
    this->sprites["UPGRADE3_ADD"]->setTextureRect(sf::IntRect(32, 0, 16, 16));

    this->texts["UPGRADE1_ADD_VALUE"] =
        new gui::Text("+1", calcChar(16, vm), calcX(1224, vm), calcY(266, vm),
                      sf::Color(255, 255, 255), false);
    this->texts["UPGRADE2_ADD_VALUE"] =
        new gui::Text("+1", calcChar(16, vm), calcX(1224, vm), calcY(436, vm),
                      sf::Color(255, 255, 255), false);
    this->texts["UPGRADE3_ADD_VALUE"] =
        new gui::Text("+1", calcChar(16, vm), calcX(1224, vm), calcY(606, vm),
                      sf::Color(255, 255, 255), false);

    this->sprites["ABILITY_FRAME"] =
        new gui::Sprite(this->select_texture, calcX(264, vm), calcY(4, vm),
                        calcScale(1, vm), false);
    this->sprites["ABILITY_FRAME"]->setTextureRect(sf::IntRect(440, 0, 88, 88));
    this->sprites["ABILITY_ICON"] =
        new gui::Sprite("assets/textures/abilities_icons.png", calcX(276, vm),
                        calcY(16, vm), calcScale(4, vm), false);
    this->sprites["ABILITY_ICON"]->setTextureRect(sf::IntRect(0, 0, 16, 16));

    this->sprite_buttons["ABILITY_UPGRADE"] =
        new gui::ButtonSprite(this->select_texture, calcX(264, vm),
                              calcY(4, vm), calcScale(1, vm), false);
    this->sprite_buttons["ABILITY_UPGRADE"]->setTextureRect(
        sf::IntRect(0, 0, 88, 88));

    this->ability_icon.setFillColor(sf::Color(128, 128, 128, 128));
    this->ability_icon.setSize(sf::Vector2f(calcX(80, vm), calcY(80, vm)));
    this->ability_icon.setPosition(sf::Vector2f(calcX(268, vm), calcY(8, vm)));

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
            calcChar(16, vm), calcX(640, vm), calcY(144, vm),
            sf::Color(182, 60, 53), true);
    }
    else if (difficulty_name == "normal") {
        this->texts["DIFFICULTY"] = new gui::Text(
            this->lang["DIFFICULTY_LEVEL"] + " " + this->lang["NORMAL"],
            calcChar(16, vm), calcX(640, vm), calcY(144, vm),
            sf::Color(182, 60, 53), true);
    }
    else if (difficulty_name == "hard") {
        this->texts["DIFFICULTY"] = new gui::Text(
            this->lang["DIFFICULTY_LEVEL"] + " " + this->lang["HARD"],
            calcChar(16, vm), calcX(640, vm), calcY(144, vm),
            sf::Color(182, 60, 53), true);
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
                                               calcY(386, vm), 0, "Cooldown",
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
    delete this->abilityUpgradeGUI;
    delete this->shopGUI;
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
            this->texts["ARMOR"]->setText(std::to_string(player.getArmor()));
            this->texts["ARMOR"]->center(calcX(388, vm));
            break;
        case 2:
            player.setReg(player.getReg() + option_val);
            this->texts["REG"]->setText(std::to_string(player.getReg()));
            this->texts["REG"]->center(calcX(452, vm));
            break;
        case 3:
            player.setMaxHP(player.getMaxHP() + option_val);
            this->update_HP();
            player.setRegenerating(true);
            break;
        case 4:
            player.setMaxSprint(player.getMaxSprint() + option_val);
            this->texts["SPRINT"]->setText(
                std::to_string(static_cast<uint32_t>(player.getSprint())) +
                "/" + std::to_string(player.getMaxSprint()));
            this->texts["SPRINT"]->center(calcX(640, vm));
            break;
        case 5:
            player.setAttack(player.getAttack() + option_val);
            this->texts["ATTACK"]->setText(std::to_string(player.getAttack()));
            this->texts["ATTACK"]->center(calcX(828, vm));
            break;
        case 6:
            player.setAttackSpeed(player.getAttackSpeed() + option_val);
            this->texts["ATTACK_SPEED"]->setText(
                std::to_string(player.getAttackSpeed()));
            this->texts["ATTACK_SPEED"]->center(calcX(892, vm));
            break;
        case 7:
            player.setSpeed(player.getSpeed() + option_val);
            this->texts["SPEED"]->setText(std::to_string(player.getSpeed()));
            this->texts["SPEED"]->center(calcX(956, vm));
            break;
        case 8:
            player.setCriticalChance(player.getCriticalChance() + option_val);
            this->texts["CRITICAL"]->setText(
                std::to_string(player.getCriticalChance()) + "%");
            this->texts["CRITICAL"]->center(calcX(1020, vm));
            break;
    }
}

void PlayerGUI::upgradePlayer(const std::string &name)
{
    this->texts["NAME"]->setText(this->lang[name]);
    this->texts["NAME"]->center(calcX(196, this->vm));
    sf::IntRect intRect = sf::IntRect(0, 0, 16, 16);
    player.upgrade(name, intRect);
    this->sprites["MINIATURE"]->setTextureRect(intRect);
    this->sprites["ABILITY_ICON"]->setTextureRect(sf::IntRect(
        intRect.left - 16, intRect.top, intRect.width, intRect.height));
    this->abilityUpgradeGUI->setAbility(
        this->sprites["ABILITY_ICON"]->getTextureRect());
    this->abilityUpgradeGUI->updatePlayerInfo("COOLDOWN", "Cooldown");
    this->abilityUpgradeGUI->updatePlayerInfo("ATTACK", this->lang["ATTACK"]);
    updatePlayerAttributes();
}

void PlayerGUI::update_level(SoundEngine &soundEngine)
{
    soundEngine.addSound("levelup");
    this->leveling = true;
    this->sideGUI = SideGUI::NONE;
    this->sprites["XP_BAR"]->setTextureRect(sf::IntRect(0, 0, 0, 20));
    this->texts["LEVEL"]->setText("Level " + std::to_string(player.getLevel()));
    this->texts["LEVEL"]->center(calcX(640, this->vm));
    if (player.getLevel() == 5 || player.getLevel() == 10) {
        this->upgrading = true;
    }

    std::vector<short> id = {3, 4, 5};
    if (player.getArmor() < 10 ||
        (player.isIncreasedArmor() &&
         ((player.isAbilityActive() && player.getArmor() < 15) ||
          (!player.isAbilityActive() && player.getArmor() < 10)))) {
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
            this->sprites["UPGRADE1"]->setTextureRect(
                sf::IntRect(64, 0, 16, 16));
            this->sprites["UPGRADE2"]->setTextureRect(
                sf::IntRect(80, 0, 16, 16));

            this->texts["UPGRADE1_NAME"]->setText(this->lang["MASTER"]);
            this->texts["UPGRADE2_NAME"]->setText(this->lang["BOMBER"]);

            this->sprites["UPGRADE1_ABILITY"]->setTextureRect(
                sf::IntRect(48, 0, 16, 16));
            this->sprites["UPGRADE2_ABILITY"]->setTextureRect(
                sf::IntRect(64, 0, 16, 16));

            this->sprites["UPGRADE1_ADD"]->setTextureRect(
                sf::IntRect(96, 0, 16, 16));
            this->sprites["UPGRADE2_ADD"]->setTextureRect(
                sf::IntRect(128, 0, 16, 16));
        }
        else if (player.getName() == "knight") {
            this->sprites["UPGRADE1"]->setTextureRect(
                sf::IntRect(96, 0, 16, 16));
            this->sprites["UPGRADE2"]->setTextureRect(
                sf::IntRect(112, 0, 16, 16));

            this->texts["UPGRADE1_NAME"]->setText(this->lang["CRUSADER"]);
            this->texts["UPGRADE2_NAME"]->setText(this->lang["PALADIN"]);

            this->sprites["UPGRADE1_ABILITY"]->setTextureRect(
                sf::IntRect(80, 0, 16, 16));
            this->sprites["UPGRADE2_ABILITY"]->setTextureRect(
                sf::IntRect(96, 0, 16, 16));

            this->sprites["UPGRADE1_ADD"]->setTextureRect(
                sf::IntRect(48, 0, 16, 16));
            this->sprites["UPGRADE2_ADD"]->setTextureRect(
                sf::IntRect(96, 0, 16, 16));
        }
    }
}

void PlayerGUI::update_XP()
{
    this->xp_bar_percent =
        static_cast<float>(player.getXP() - player.getLastMaxXP()) /
        static_cast<float>(player.getMaxXP() - player.getLastMaxXP());
    this->texts["XP"]->setText("XP:" + std::to_string(player.getXP()) + "/" +
                               std::to_string(player.getMaxXP()));
    this->texts["XP"]->center(calcX(640, vm));
}

void PlayerGUI::updating_XP(float dt)
{
    const int width = this->sprites["XP_BAR"]->getTextureRect().width;
    const int barrier = static_cast<int>(this->xp_bar_percent * 264.f);

    if (width < barrier && player.isLeveling()) {
        const int distance = static_cast<int>(width + 1000.f * dt);
        if (distance > barrier) {
            this->sprites["XP_BAR"]->setTextureRect(
                sf::IntRect(0, 0, barrier, 20));
        }
        else if (distance > 264) {
            this->sprites["XP_BAR"]->setTextureRect(sf::IntRect(0, 0, 264, 20));
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

void PlayerGUI::update_HP()
{
    if (player.isDead()) {
        this->hp_bar_percent = 0.f;
        this->sprites["HP_BAR"]->setTextureRect(sf::IntRect(0, 20, 0, 20));
        this->sideGUI = SideGUI::NONE;
    }
    else {
        this->hp_bar_percent =
            static_cast<float>(player.getHP()) / player.getMaxHP();
    }

    this->texts["HP"]->setText("HP:" + std::to_string(player.getHP()) + "/" +
                               std::to_string(player.getMaxHP()));
    this->texts["HP"]->center(calcX(640, vm));
}

void PlayerGUI::updating_HP(SoundEngine &soundEngine, float dt)
{
    if (player.isHPRegenerating(dt) && !player.isDead()) {
        this->texts["HP"]->setText("HP:" + std::to_string(player.getHP()) +
                                   "/" + std::to_string(player.getMaxHP()));
        this->texts["HP"]->center(calcX(640, this->vm));
        this->hp_bar_percent =
            static_cast<float>(player.getHP()) / player.getMaxHP();
        player.setRegenerating(true);
        if (this->isShopping() && player.getHP() / player.getMaxHP() == 1) {
            this->shopGUI->disableItem("FULL_HP");
        }
    }
    else if (player.isDead()) {
        this->hp_bar_percent = 0.f;
        this->sprites["HP_BAR"]->setTextureRect(sf::IntRect(0, 20, 0, 20));
        this->text_buttons["QUIT"]->setPosition(
            sf::Vector2f(calcX(640, vm), calcY(488, vm)));
        this->text_buttons["QUIT"]->center(calcX(640, vm));
        soundEngine.addSound("gameover");
    }

    const int width = this->sprites["HP_BAR"]->getTextureRect().width;
    const int barrier = static_cast<int>(this->hp_bar_percent * 264.f);

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
        else if (distance > 264) {
            this->sprites["HP_BAR"]->setTextureRect(
                sf::IntRect(0, 20, 264, 20));
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

void PlayerGUI::update_Gold()
{
    this->texts["GOLD"]->setText(std::to_string(player.getGold()));

    shopGUI->updateItemFrames();
    abilityUpgradeGUI->updateItemFrames();
}

void PlayerGUI::update_ability(float dt)
{
    if (player.getAbilityCooldown() > 0.f) {
        const float value = player.getAbilityCooldown() /
                            (player.getAbilityTotalMaxTime()) * calcX(80, vm);
        this->ability_icon.setSize(
            sf::Vector2f(calcX(80, vm), calcX(80, vm) - value));
        this->ability_icon.setPosition(
            sf::Vector2f(calcX(268, vm), calcX(8, vm) + value));
        if (this->ability_icon.getSize().y <= 0.f) {
            this->sprites["ABILITY_FRAME"]->setTextureRect(
                sf::IntRect(440, 0, 88, 88));
        }
    }
}

void PlayerGUI::setAbilityIcon()
{
    this->sprites["ABILITY_FRAME"]->setTextureRect(sf::IntRect(352, 0, 88, 88));
    this->ability_icon.setSize(sf::Vector2f(calcX(80, vm), calcY(80, vm)));
    this->ability_icon.setPosition(sf::Vector2f(calcX(268, vm), calcY(8, vm)));
}

void PlayerGUI::updateSprint(float dt)
{
    const int bar_width = this->sprites["SPRINT_BAR"]->getTextureRect().width;
    const int barrier_width = static_cast<int>(
        player.getSprint() / static_cast<float>(player.getMaxSprint()) * 264.f);

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
        else if (new_width > 264) {
            this->sprites["SPRINT_BAR"]->setTextureRect(
                sf::IntRect(0, 40, 264, 20));
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

    this->texts["WAVE_NUMBER"]->setText(this->lang["WAVE"] +
                                        std::to_string(wave));

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

    this->texts["MONSTER_COUNT"]->setText(this->lang["MONSTER_COUNT"] +
                                          std::to_string(monsterCount));

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
    this->texts["MONSTER_COUNT"]->setText(this->lang["MONSTER_COUNT"] +
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
    this->texts["ARMOR"]->setText(std::to_string(player.getArmor()));
    this->texts["ARMOR"]->center(calcX(388, vm));
}

void PlayerGUI::updateAttack()
{
    this->texts["ATTACK"]->setText(std::to_string(player.getAttack()));
    this->texts["ATTACK"]->center(calcX(828, vm));
}

void PlayerGUI::updateReg()
{
    this->texts["REG"]->setText(std::to_string(player.getReg()));
    this->texts["REG"]->center(calcX(452, vm));
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
    if (this->isShopping()) {
        if (shopGUI->hasBoughtItem(mousePos, mouseClicked, "FULL_HP",
                                   &floatingTextSystem, &soundEngine)) {
            player.setHP(player.getMaxHP());
            this->update_HP();
            player.setRegenerating(true);
            this->update_Gold();
            return true;
        }
        else if (shopGUI->hasBoughtItem(mousePos, mouseClicked, "MAX_HP",
                                        &floatingTextSystem, &soundEngine)) {
            player.setMaxHP(player.getMaxHP() + 2);
            this->update_HP();
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
            if (player.getArmor() > 10 ||
                (player.isIncreasedArmor() &&
                 ((player.isAbilityActive() && player.getArmor() > 15) ||
                  (!player.isAbilityActive() && player.getArmor() > 10)))) {
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
    this->sprite_buttons["ABILITY_UPGRADE"]->update(mousePos);
    if (this->sprite_buttons["ABILITY_UPGRADE"]->isPressed() && !mouseClicked) {
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
    this->sprite_buttons["UPGRADE1"]->update(mousePos);
    if (this->sprite_buttons["UPGRADE1"]->isPressed() && !mouseClicked) {
        if (player.getLevel() == 5) {
            this->abilityUpgradeGUI->addPlayerStat(
                "ATTACK", calcX(16, vm), calcY(344, vm), this->lang["ATTACK"]);
            this->abilityUpgradeGUI->addAbilityUpgrade(
                "ATTACK", calcX(44, vm), calcY(514, vm), 1, "Attack", "+1", 30);
            this->upgradePlayer("NINJA");
        }
        else if (player.getLevel() == 10) {
            if (player.getName() == "ninja") {
                this->upgradePlayer("MASTER");
            }
            else if (player.getName() == "knight") {
                this->upgradePlayer("CRUSADER");
            }
        }

        this->upgrading = false;
        this->sprite_buttons["UPGRADE1"]->setTransparent();
        soundEngine.addSound("upgrade");
        return true;
    }

    this->sprite_buttons["UPGRADE2"]->update(mousePos);
    if (this->sprite_buttons["UPGRADE2"]->isPressed() && !mouseClicked) {
        if (player.getLevel() == 5) {
            this->upgradePlayer("KNIGHT");
        }
        else if (player.getLevel() == 10) {
            if (player.getName() == "ninja") {
                this->upgradePlayer("BOMBER");
            }
            else if (player.getName() == "knight") {
                this->upgradePlayer("PALADIN");
            }
        }
        this->upgrading = false;
        this->sprite_buttons["UPGRADE2"]->setTransparent();
        soundEngine.addSound("upgrade");
        return true;
    }

    if (player.getLevel() == 5) {
        this->sprite_buttons["UPGRADE3"]->update(mousePos);
        if (this->sprite_buttons["UPGRADE3"]->isPressed() && !mouseClicked) {
            this->upgradePlayer("SCOUT");
            this->upgrading = false;
            this->sprite_buttons["UPGRADE3"]->setTransparent();
            soundEngine.addSound("upgrade");
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
    this->texts["ARMOR"]->setText(std::to_string(player.getArmor()));
    this->texts["ARMOR"]->center(calcX(388, vm));
    this->texts["REG"]->setText(std::to_string(player.getReg()));
    this->texts["REG"]->center(calcX(452, vm));
    this->texts["ATTACK"]->setText(std::to_string(player.getAttack()));
    this->texts["ATTACK"]->center(calcX(828, vm));
    this->texts["ATTACK_SPEED"]->setText(
        std::to_string(player.getAttackSpeed()));
    this->texts["ATTACK_SPEED"]->center(calcX(892, vm));
    this->texts["SPEED"]->setText(std::to_string(player.getSpeed()));
    this->texts["SPEED"]->center(calcX(956, vm));
    this->texts["CRITICAL"]->setText(
        std::to_string(player.getCriticalChance()) + "%");
    this->texts["CRITICAL"]->center(calcX(1020, vm));
    this->update_HP();
    if (player.getHP() < player.getMaxHP()) {
        player.setRegenerating(true);
    }
}

void PlayerGUI::update(sf::Vector2f &mousePosView, float waveCountdown,
                       float bossHP, float dt)
{
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

    this->levelShown = true;
    if ((mousePosView.y >= calcY(12, vm) && mousePosView.y <= calcY(34, vm)) &&
        (mousePosView.x >= calcX(508, vm) &&
         mousePosView.x <= calcX(772, vm))) {
        this->levelShown = false;
    }

    if (this->waveCountdown < 10.f) {
        this->texts["WAVE_COUNTDOWN"]->setText(
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

        this->texts["WAVE_COUNTDOWN"]->setText(
            this->lang["NEXT_WAVE"] +
            std::to_string(10 - static_cast<int>(waveCountdown)));
    }
}

void PlayerGUI::draw(sf::RenderTarget &target)
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

    if (isShopping()) {
        this->sprites["SIDE_GUI"]->draw(target);

        this->shopGUI->draw(target);
    }

    if (player.isUpgraded()) {
        this->sprites["ABILITY_ICON"]->draw(target);
        if (player.getAbilityCooldown() < player.getAbilityMaxTime()) {
            target.draw(this->ability_icon);
        }
        this->sprites["ABILITY_FRAME"]->draw(target);
        this->sprite_buttons["ABILITY_UPGRADE"]->draw(target);

        if (isBuyingAbility() && !isShopping()) {
            this->sprites["SIDE_GUI"]->draw(target);

            this->abilityUpgradeGUI->draw(target);
        }
    }

    if (this->waveCountdown < 10.f) {
        this->texts["WAVE_COUNTDOWN"]->draw(target);
        if (this->waveCountdown > 8.f) {
            this->texts["BIG_WAVE_NUMBER"]->draw(target);
            this->texts["WAVE_NEW_MOBS"]->draw(target);
            this->texts["MOBS_TO_KILL"]->draw(target);
        }
    }
    else if (this->waveCountdown >= 10.f) {
        this->texts["WAVE_NUMBER"]->draw(target);
        this->texts["MONSTER_COUNT"]->draw(target);

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

            this->sprites["UPGRADE1_FRAME"]->draw(target);
            this->sprites["UPGRADE1"]->draw(target);
            this->sprite_buttons["UPGRADE1"]->draw(target);
            this->texts["UPGRADE1_NAME"]->draw(target);
            this->sprites["UPGRADE1_ABILITY"]->draw(target);
            this->sprites["UPGRADE1_ADD"]->draw(target);
            this->texts["UPGRADE1_ADD_VALUE"]->draw(target);

            this->sprites["UPGRADE2_FRAME"]->draw(target);
            this->sprites["UPGRADE2"]->draw(target);
            this->sprite_buttons["UPGRADE2"]->draw(target);
            this->texts["UPGRADE2_NAME"]->draw(target);
            this->sprites["UPGRADE2_ABILITY"]->draw(target);
            this->sprites["UPGRADE2_ADD"]->draw(target);
            this->texts["UPGRADE2_ADD_VALUE"]->draw(target);

            if (player.getLevel() == 5) {
                this->sprites["UPGRADE3_FRAME"]->draw(target);
                this->sprites["UPGRADE3"]->draw(target);
                this->sprite_buttons["UPGRADE3"]->draw(target);
                this->texts["UPGRADE3_NAME"]->draw(target);
                this->sprites["UPGRADE3_ABILITY"]->draw(target);
                this->sprites["UPGRADE3_ADD"]->draw(target);
                this->texts["UPGRADE3_ADD_VALUE"]->draw(target);
            }
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
