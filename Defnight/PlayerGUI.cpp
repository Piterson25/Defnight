#include "stdafx.h"
#include "Functions.h"
#include "PlayerGUI.h"

PlayerGUI::PlayerGUI(sf::Font* font, Player* player, sf::VideoMode& vm, const float& soundVolume, 
	const std::string& heroName, const std::string& difficulty_name, std::unordered_map<std::string, std::string>& lang)
	:player(player), vm(vm), lang(lang), font(*font)
{
	
	this->attributes_texture.loadFromFile("external/assets/icons.png");
	for (short i = 0; i < 9; ++i) {
		sf::Sprite att;
		att.setTexture(this->attributes_texture);
		sf::IntRect intRect(i * 16, 0, 16, 16);
		att.setTextureRect(intRect);
		att.setScale(calcScale(2, vm), calcScale(2, vm));
		this->attribute_vec.push_back(att);
	}

	this->upgrades_texture.loadFromFile("external/assets/upgrade_icons.png");
	for (short i = 0; i < 7; ++i) {
		sf::Sprite upgrade;
		upgrade.setTexture(this->upgrades_texture);
		sf::IntRect intRect(i * 16, 0, 16, 16);
		upgrade.setTextureRect(intRect);
		upgrade.setScale(calcScale(4, vm), calcScale(4, vm));
		this->upgrades_vec.push_back(upgrade);
	}

	this->abilities_texture.loadFromFile("external/assets/abilities_icons.png");
	for (short i = 0; i < 6; ++i) {
		sf::Sprite upgrade;
		upgrade.setTexture(this->abilities_texture);
		sf::IntRect intRect(i * 16, 0, 16, 16);
		upgrade.setTextureRect(intRect);
		this->abilities_vec.push_back(upgrade);
	}

	this->sprites["BAR"] = new gui::Sprite("external/assets/bar.png", 0, 0, calcScale(1, vm), false);

	this->sprites["PROGRESS_BAR"] = new gui::Sprite("external/assets/progress_bar.png", calcX(640, vm), calcY(58, vm), calcScale(1, vm), true);
	this->sprites["PROGRESS_BAR"]->setColor(sf::Color::Transparent);

	this->sprites["MINIATURE"] = new gui::Sprite(this->upgrades_texture, calcX(164, vm), calcX(16, vm), calcScale(4, vm), false);
	this->sprites["MINIATURE"]->setTextureRect(sf::IntRect(0, 0, 16, 16));
	this->texts["NAME"] = new gui::Text(&this->font, this->lang["WARRIOR"], calcChar(16, vm), calcX(196, vm), calcY(96, vm), sf::Color(255, 255, 255), true);

	this->sprites["GOLD"] = new gui::Sprite(attribute_vec[0], 0, calcY(86, vm), calcScale(2, vm), false);
	this->texts["GOLD"] = new gui::Text(&this->font, std::to_string(player->getGold()), calcChar(16, vm), calcX(36, vm), calcY(96, vm), sf::Color(255, 246, 76), false);

	this->sprites["ARMOR"] = new gui::Sprite(attribute_vec[7], calcX(372, vm), calcY(16, vm), calcScale(2, vm), false);
	this->texts["ARMOR"] = new gui::Text(&this->font, std::to_string(player->getArmor()), calcChar(16, vm), calcX(388, vm), calcY(70, vm), sf::Color(192, 192, 192), true);

	this->sprites["ATTACK"] = new gui::Sprite(attribute_vec[1], calcX(812, vm), calcY(16, vm), calcScale(2, vm), false);
	this->texts["ATTACK"] = new gui::Text(&this->font, std::to_string(player->getAttack()), calcChar(16, vm), calcX(828, vm), calcY(70, vm), sf::Color(192, 192, 192), true);

	this->sprites["ATTACK_SPEED"] = new gui::Sprite(attribute_vec[2], calcX(876, vm), calcY(16, vm), calcScale(2, vm), false);
	this->texts["ATTACK_SPEED"] = new gui::Text(&this->font, std::to_string(player->getAttackSpeed()), calcChar(16, vm), calcX(892, vm), calcY(70, vm), sf::Color(192, 192, 192), true);

	this->sprites["SPEED"] = new gui::Sprite(attribute_vec[3], calcX(940, vm), calcY(16, vm), calcScale(2, vm), false);
	this->texts["SPEED"] = new gui::Text(&this->font, std::to_string(player->getSpeed()), calcChar(16, vm), calcX(956, vm), calcY(70, vm), sf::Color(192, 192, 192), true);

	this->sprites["CRITICAL"] = new gui::Sprite(attribute_vec[4], calcX(1004, vm), calcY(16, vm), calcScale(2, vm), false);
	this->texts["CRITICAL"] = new gui::Text(&this->font, std::to_string(player->getCriticalChance()) + "%", calcChar(16, vm), calcX(1020, vm), calcY(70, vm), sf::Color(192, 192, 192), true);

	this->sprites["REG"] = new gui::Sprite(attribute_vec[5], calcX(436, vm), calcY(16, vm), calcScale(2, vm), false);
	this->texts["REG"] = new gui::Text(&this->font, std::to_string(player->getReg()), calcChar(16, vm), calcX(452, vm), calcY(70, vm), sf::Color(182, 60, 53), true);

	this->sprites["XP_BAR"] = new gui::Sprite("external/assets/bars.png", calcX(508, vm), calcY(12, vm), calcScale(1, vm), false);
	this->sprites["XP_BAR"]->setTextureRect(sf::IntRect(0, 0, 0, 0));
	this->texts["XP"] = new gui::Text(&this->font, "XP:" + std::to_string(this->player->getXP()) + "/" + std::to_string(this->player->getMaxXP()), calcChar(16, vm), calcX(640, vm), calcY(16, vm), sf::Color(255, 255, 255), true);
	this->texts["LEVEL"] = new gui::Text(&this->font, "Level " + std::to_string(this->player->getLevel()), calcChar(16, vm), calcX(640, vm), calcY(16, vm), sf::Color(255, 255, 255), true);
	this->isLevelshown = true;
	this->xp_bar_barrier = 0.f;

	this->sprites["HP_BAR"] = new gui::Sprite("external/assets/bars.png", calcX(508, vm), calcY(66, vm), calcScale(1, vm), false);
	this->sprites["HP_BAR"]->setTextureRect(sf::IntRect(0, 22, 264, 22));
	this->texts["HP"] = new gui::Text(&this->font, "HP:" + std::to_string(this->player->getHP()) + "/" + std::to_string(this->player->getMaxHP()), calcChar(16, vm), calcX(640, vm), calcY(70, vm), sf::Color(255, 255, 255), true);
	this->hp_bar_barrier = 1.f;

	this->texts["MONSTER_COUNT"] = new gui::Text(&this->font, this->lang["MONSTER_COUNT"], calcChar(16, vm), calcX(906, vm), calcY(96, vm), sf::Color(130, 139, 152), false);
	this->texts["MONSTER_COUNT"]->setPosition(sf::Vector2f(calcX(1218, vm) - this->texts["MONSTER_COUNT"]->getWidth(), calcY(96, vm)));

	this->texts["WAVE_NUMBER"] = new gui::Text(&this->font, this->lang["WAVE"], calcChar(32, vm), calcX(1060, vm), calcY(4, vm), sf::Color(228, 92, 95), false);
	this->texts["WAVE_COUNTDOWN"] = new gui::Text(&this->font, this->lang["NEXT_WAVE"], calcChar(16, vm), calcX(954, vm), calcY(96, vm), sf::Color(255, 246, 76), false);
	this->texts["WAVE_COUNTDOWN"]->setPosition(sf::Vector2f(calcX(1218, vm) - this->texts["WAVE_COUNTDOWN"]->getWidth(), calcY(96, vm)));
	this->titleCooldown = 0.f;
	this->texts["WAVE_NEW_MOBS"] = new gui::Text(&this->font, "", calcChar(16, vm), calcX(640, vm), calcY(562, vm), sf::Color(228, 92, 95), true);
	this->texts["BIG_WAVE_NUMBER"] = new gui::Text(&this->font, this->lang["WAVE"], calcChar(64, vm), calcX(640, vm), calcY(256, vm), sf::Color(255, 255, 255), true);
	this->texts["MOBS_TO_KILL"] = new gui::Text(&this->font, this->lang["MONSTER"], calcChar(32, vm), calcX(640, vm), calcY(512, vm), sf::Color(192, 192, 192), true);
	this->waveCountdown = 0.f;

	this->isLeveling = false;
	
	this->texts["LEVEL_UP"] = new gui::Text(&this->font, "LEVEL UP!", calcChar(32, vm), calcX(640, vm), calcY(256, vm), sf::Color(255, 246, 76), true);
	this->sprites["UPGRADE_BAR"] = new gui::Sprite("external/assets/upgrade_bar.png", calcX(640, vm), calcY(476, vm), calcScale(1, vm), true);
	this->sprites["SIDE_BAR"] = new gui::Sprite("external/assets/side_bar.png", calcX(988, vm), calcY(128, vm), calcScale(1, vm), false);

	this->sprite_buttons["OPTION1"] = new gui::ButtonSprite("external/assets/select_levelup.png", calcX(504, vm), calcY(500, vm), calcScale(1, vm), false);
	this->sprite_buttons["OPTION2"] = new gui::ButtonSprite("external/assets/select_levelup.png", calcX(688, vm), calcY(500, vm), calcScale(1, vm), false);
	this->sprites["OPTION1"] = new gui::Sprite(this->attributes_texture, calcX(516, vm), calcY(512, vm), calcScale(4, vm), false);
	this->sprites["OPTION2"] = new gui::Sprite(this->attributes_texture, calcX(700, vm), calcY(512, vm), calcScale(4, vm), false);
	this->texts["OPTION1"] = new gui::Text(&this->font, "", calcChar(16, vm), calcX(548, vm), calcY(608, vm), sf::Color(255, 255, 255), false);
	this->texts["OPTION2"] = new gui::Text(&this->font, "", calcChar(16, vm), calcX(732, vm), calcY(608, vm), sf::Color(255, 255, 255), false);

	this->option1_id = 0;
	this->option1_val = 0;
	this->option2_id = 0;
	this->option2_val = 0;

	this->isUpgrading = false;

	this->sprite_buttons["UPGRADE1"] = new gui::ButtonSprite("external/assets/select_upgrade.png", calcX(1008, vm), calcY(216, vm), calcScale(1, vm), false);
	this->sprite_buttons["UPGRADE2"] = new gui::ButtonSprite("external/assets/select_upgrade.png", calcX(1008, vm), calcY(380, vm), calcScale(1, vm), false);
	this->sprite_buttons["UPGRADE3"] = new gui::ButtonSprite("external/assets/select_upgrade.png", calcX(1007, vm), calcY(544, vm), calcScale(1, vm), false);

	this->sprites["UPGRADE1"] = new gui::Sprite(this->upgrades_vec[1], calcX(1020, vm), calcY(228, vm), calcScale(4, vm), false);
	this->sprites["UPGRADE2"] = new gui::Sprite(this->upgrades_vec[2], calcX(1020, vm), calcY(392, vm), calcScale(4, vm), false);
	this->sprites["UPGRADE3"] = new gui::Sprite(this->upgrades_vec[3], calcX(1020, vm), calcY(556, vm), calcScale(4, vm), false);

	this->texts["UPGRADE1_NAME"] = new gui::Text(&this->font, this->lang["NINJA"], calcChar(16, vm), calcX(1104, vm), calcY(228, vm), sf::Color(255, 255, 255), false);
	this->texts["UPGRADE2_NAME"] = new gui::Text(&this->font, this->lang["KNIGHT"], calcChar(16, vm), calcX(1104, vm), calcY(388, vm), sf::Color(255, 255, 255), false);
	this->texts["UPGRADE3_NAME"] = new gui::Text(&this->font, this->lang["SCOUT"], calcChar(16, vm), calcX(1096, vm), calcY(548, vm), sf::Color(255, 255, 255), false);

	this->sprites["UPGRADE1_ABILITY"] = new gui::Sprite(this->abilities_vec[0], calcX(1168, vm), calcY(262, vm), calcScale(2, vm), false);
	this->sprites["UPGRADE2_ABILITY"] = new gui::Sprite(this->abilities_vec[1], calcX(1168, vm), calcY(426, vm), calcScale(2, vm), false);
	this->sprites["UPGRADE3_ABILITY"] = new gui::Sprite(this->abilities_vec[2], calcX(1168, vm), calcY(590, vm), calcScale(2, vm), false);

	this->sprites["UPGRADE1_ADD"] = new gui::Sprite(this->attribute_vec[3], calcX(1216, vm), calcY(228, vm), calcScale(2, vm), false);
	this->sprites["UPGRADE2_ADD"] = new gui::Sprite(this->attribute_vec[1], calcX(1216, vm), calcY(392, vm), calcScale(2, vm), false);
	this->sprites["UPGRADE3_ADD"] = new gui::Sprite(this->attribute_vec[5], calcX(1216, vm), calcY(556, vm), calcScale(2, vm), false);
	
	this->texts["UPGRADE1_ADD_VALUE"] = new gui::Text(&this->font, "+1", calcChar(16, vm), calcX(1216, vm), calcY(278, vm), sf::Color(255, 255, 255), false);
	this->texts["UPGRADE2_ADD_VALUE"] = new gui::Text(&this->font, "+1", calcChar(16, vm), calcX(1216, vm), calcY(442, vm), sf::Color(255, 255, 255), false);
	this->texts["UPGRADE3_ADD_VALUE"] = new gui::Text(&this->font, "+1", calcChar(16, vm), calcX(1216, vm), calcY(606, vm), sf::Color(255, 255, 255), false);

	this->sprites["ABILITY_ICON"] = new gui::Sprite("external/assets/abilities_icons.png", calcX(288, vm), calcY(16, vm), calcScale(4, vm), false);
	this->sprites["ABILITY_ICON"]->setTextureRect(sf::IntRect(0, 0, 16, 16));

	this->ability_icon.setFillColor(sf::Color(128, 128, 128, 128));
	this->ability_icon.setSize(sf::Vector2f(calcX(64, vm), calcY(64, vm)));
	this->ability_icon.setPosition(sf::Vector2f(calcX(288,vm), calcY(16, vm)));

	this->death_background.setFillColor(sf::Color(182, 60, 53, 192));
	this->death_background.setSize(sf::Vector2f(calcX(1280, vm), calcY(592, vm)));
	this->death_background.setPosition(sf::Vector2f(0, calcY(128, vm)));

	this->texts["YOU_DIED"] = new gui::Text(&this->font, this->lang["YOU_DIED"], calcChar(72, vm), calcX(640, vm), calcY(224, vm), sf::Color(255, 255, 255), true);
	this->text_buttons["RESUME"] = new gui::ButtonText(&this->font, this->lang["RESUME"], calcChar(32, vm), calcX(640, vm), calcY(296, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), true);
	this->text_buttons["MAIN_MENU"] = new gui::ButtonText(&this->font, this->lang["MAIN_MENU"], calcChar(32, vm), calcX(640, vm), calcY(392, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), true);
	this->text_buttons["SETTINGS"] = new gui::ButtonText(&this->font, this->lang["SETTINGS"], calcChar(32, vm), calcX(640, vm), calcY(488, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), true);
	this->text_buttons["QUIT"] = new gui::ButtonText(&this->font, this->lang["QUIT"], calcChar(32, vm), calcX(640, vm), calcY(584, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), true);
	
	
	
	this->texts["KILLS"] = new gui::Text(&this->font, this->lang["KILLS"] + std::to_string(this->player->getKills()), calcChar(16, vm), calcX(640, vm), calcY(144, vm), sf::Color(192, 192, 192), true);

	if (difficulty_name == "easy") 
		this->texts["DIFFICULTY"] = new gui::Text(&this->font, this->lang["DIFFICULTY_LEVEL"] + " " + this->lang["EASY"], calcChar(16, vm), calcX(640, vm), calcY(144, vm), sf::Color(182, 60, 53), true);
	else if (difficulty_name == "normal") 
		this->texts["DIFFICULTY"] = new gui::Text(&this->font, this->lang["DIFFICULTY_LEVEL"] + " " + this->lang["NORMAL"], calcChar(16, vm), calcX(640, vm), calcY(144, vm), sf::Color(182, 60, 53), true);
	else if (difficulty_name == "hard") 
		this->texts["DIFFICULTY"] = new gui::Text(&this->font, this->lang["DIFFICULTY_LEVEL"] + " " + this->lang["HARD"], calcChar(16, vm), calcX(640, vm), calcY(144, vm), sf::Color(182, 60, 53), true);

	
	this->escape_background.setFillColor(sf::Color(0, 0, 0, 192));
	this->escape_background.setSize(sf::Vector2f(calcX(1280, vm), calcY(720, vm)));
	this->isEscape = false;

	this->isShopping = false;

	this->sprites["SHOP"] = new gui::Sprite("external/assets/shop_bar.png", 0, calcY(128, vm), calcScale(1, vm), false);

	this->sprites["ITEM1"] = new gui::Sprite(attribute_vec[8], calcX(64, vm), calcY(192, vm), calcScale(4, vm), false);
	this->sprites["ITEM2"] = new gui::Sprite(attribute_vec[6], calcX(64, vm), calcY(320, vm), calcScale(4, vm), false);
	this->sprites["ITEM3"] = new gui::Sprite(attribute_vec[1], calcX(64, vm), calcY(448, vm), calcScale(4, vm), false);
	this->sprites["ITEM4"] = new gui::Sprite(attribute_vec[7], calcX(64, vm), calcY(576, vm), calcScale(4, vm), false);

	this->sprite_buttons["ITEM1"] = new gui::ButtonSprite("external/assets/select_levelup.png", calcX(96, vm), calcY(180, vm), calcScale(1, vm), true);
	this->sprite_buttons["ITEM2"] = new gui::ButtonSprite("external/assets/select_levelup.png", calcX(96, vm), calcY(308, vm), calcScale(1, vm), true);
	this->sprite_buttons["ITEM3"] = new gui::ButtonSprite("external/assets/select_levelup.png", calcX(96, vm), calcY(436, vm), calcScale(1, vm), true);
	this->sprite_buttons["ITEM4"] = new gui::ButtonSprite("external/assets/select_levelup.png", calcX(96, vm), calcY(564, vm), calcScale(1, vm), true);

	this->texts["ITEM1"] = new gui::Text(&this->font, "Full HP", calcChar(16, vm), calcX(160, vm), calcY(208, vm), sf::Color(255, 255, 255), false);
	this->texts["ITEM2"] = new gui::Text(&this->font, this->lang["MAX_HP"] + "\n+2", calcChar(16, vm), calcX(160, vm), calcY(320, vm), sf::Color(255, 255, 255), false);
	this->texts["ITEM3"] = new gui::Text(&this->font, this->lang["ATTACK"] + "\n+1", calcChar(16, vm), calcX(160, vm), calcY(448, vm), sf::Color(255, 255, 255), false);
	this->texts["ITEM4"] = new gui::Text(&this->font, this->lang["ARMOR"] + "\n+1", calcChar(16, vm), calcX(160, vm), calcY(576, vm), sf::Color(255, 255, 255), false);

	this->sprites["ITEM1_COIN"] = new gui::Sprite(attribute_vec[0], calcX(176, vm), calcY(232, vm), calcScale(2, vm), false);
	this->sprites["ITEM2_COIN"] = new gui::Sprite(attribute_vec[0], calcX(176, vm), calcY(360, vm), calcScale(2, vm), false);
	this->sprites["ITEM3_COIN"] = new gui::Sprite(attribute_vec[0], calcX(176, vm), calcY(488, vm), calcScale(2, vm), false);
	this->sprites["ITEM4_COIN"] = new gui::Sprite(attribute_vec[0], calcX(176, vm), calcY(616, vm), calcScale(2, vm), false);

	this->item1Price = 30;
	this->item2Price = 20;
	this->item3Price = 15;
	this->item4Price = 10;

	this->texts["ITEM1_PRICE"] = new gui::Text(&this->font, std::to_string(this->item1Price), calcChar(16, vm), calcX(208, vm), calcY(240, vm), sf::Color(255, 246, 76), false);
	this->texts["ITEM2_PRICE"] = new gui::Text(&this->font, std::to_string(this->item2Price), calcChar(16, vm), calcX(208, vm), calcY(368, vm), sf::Color(255, 246, 76), false);
	this->texts["ITEM3_PRICE"] = new gui::Text(&this->font, std::to_string(this->item3Price), calcChar(16, vm), calcX(208, vm), calcY(496, vm), sf::Color(255, 246, 76), false);
	this->texts["ITEM4_PRICE"] = new gui::Text(&this->font, std::to_string(this->item4Price), calcChar(16, vm), calcX(208, vm), calcY(624, vm), sf::Color(255, 246, 76), false);

	this->bossWave = false;

	this->texts["BOSS"] = new gui::Text(&this->font, "Minotaur", calcChar(16, vm), calcX(640, vm), calcY(136, vm), sf::Color(113, 43, 59), true);
	this->sprites["BOSS_BAR"] = new gui::Sprite("external/assets/bars.png", calcX(376, vm), calcY(158, vm), calcScale(1, vm), false);
	this->sprites["BOSS_BAR"]->setTextureRect(sf::IntRect(0, 44, 528, 22));
	this->sprites["BOSS_BAR_EMPTY"] = new gui::Sprite("external/assets/bars.png", calcX(376, vm), calcY(158, vm), calcScale(1, vm), false);
	this->sprites["BOSS_BAR_EMPTY"]->setTextureRect(sf::IntRect(0, 66, 528, 22));

	this->boss_bar_barrier = 1.f;
}

PlayerGUI::~PlayerGUI()
{
}

void PlayerGUI::update_options(uint32_t& option_id, uint32_t& option_val, std::vector<short>& id_vector, gui::Text* text, gui::Sprite* sprite, const float& pos)
{
	const uint32_t id = static_cast<uint32_t>(Random::Float() * id_vector.size());
	
	option_id = id_vector[id];
	switch (option_id)
	{
	case 1:
		option_val = 1;
		text->setText(this->lang["ATTACK"] + "\n+" + std::to_string(option_val));
		break;
	case 2:
		option_val = 1;
		text->setText(this->lang["ATTACK_SPEED"] + "\n+" + std::to_string(option_val));
		break;
	case 3:
		option_val = 1;
		text->setText(this->lang["SPEED"] + "\n+" + std::to_string(option_val));
		break;
	case 4:
		option_val = 10;
		text->setText(this->lang["CRITICAL"] + "\n+" + std::to_string(option_val) + "%");
		break;
	case 5:
		option_val = 1;
		text->setText(this->lang["REG"] + "\n+" + std::to_string(option_val));
		break;
	case 6:
		option_val = static_cast<uint32_t>(Random::Float() * 2.f) + 2;
		text->setText(this->lang["MAX_HP"] + "\n+" + std::to_string(option_val));
		break;
	}
	id_vector.erase(id_vector.begin() + id);
	sprite->setTextureRect(sf::IntRect(option_id * 16, 0, 16, 16));
	text->center(pos);
}

void PlayerGUI::levelUpPlayer(uint32_t& option_id, uint32_t& option_val)
{
	const sf::VideoMode vm = this->vm;

	switch (option_id) {
	case 1:
		this->player->setAttack(this->player->getAttack() + option_val);
		this->texts["ATTACK"]->setText(std::to_string(player->getAttack()));
		this->texts["ATTACK"]->center(calcX(828, vm));
		break;
	case 2:
		this->player->setAttackSpeed(this->player->getAttackSpeed() + option_val);
		this->texts["ATTACK_SPEED"]->setText(std::to_string(player->getAttackSpeed()));
		this->texts["ATTACK_SPEED"]->center(calcX(892, vm));
		break;
	case 3:
		this->player->setSpeed(this->player->getSpeed() + option_val);
		this->texts["SPEED"]->setText(std::to_string(player->getSpeed()));
		this->texts["SPEED"]->center(calcX(956, vm));
		break;
	case 4:
		this->player->setCriticalChance(this->player->getCriticalChance() + option_val);
		this->texts["CRITICAL"]->setText(std::to_string(player->getCriticalChance()) + "%");
		this->texts["CRITICAL"]->center(calcX(1020, vm));
		break;
	case 5:
		this->player->setReg(this->player->getReg() + option_val);
		this->texts["REG"]->setText(std::to_string(player->getReg()));
		this->texts["REG"]->center(calcX(452, vm));
		break;
	case 6:
		this->player->setMaxHP(this->player->getMaxHP() + option_val);
		this->update_HP();
		this->player->setIsRegenerating(true);
		break;
	}
}

void PlayerGUI::upgradePlayer(const std::string& name)
{
	this->texts["NAME"]->setText(this->lang[name]);
	this->texts["NAME"]->center(calcX(196, this->vm));
	if (name == "NINJA") {
		this->sprites["MINIATURE"]->setTextureRect(sf::IntRect(16, 0, 16, 16));
		this->player->setTexturePath("external/assets/heroes/ninja.png");
		this->player->setSpeed(this->player->getSpeed() + 1);
		this->texts["SPEED"]->setText(std::to_string(this->player->getSpeed()));
		this->texts["SPEED"]->center(calcX(956, vm));
		this->player->setName("ninja");
		this->sprites["ABILITY_ICON"]->setTextureRect(sf::IntRect(0, 0, 16, 16));
		this->player->setAbilityMaxTime(2.f);
		this->player->setAbilityCooldown(2.f);
		this->player->setAbilityTime(2.f);
	}
	else if (name == "KNIGHT") {
		this->sprites["MINIATURE"]->setTextureRect(sf::IntRect(32, 0, 16, 16));
		this->player->setTexturePath("external/assets/heroes/knight.png");
		this->player->setAttack(this->player->getAttack() + 1);
		this->texts["ATTACK"]->setText(std::to_string(this->player->getAttack()));
		this->texts["ATTACK"]->center(calcX(828, vm));
		this->player->setName("knight");
		this->sprites["ABILITY_ICON"]->setTextureRect(sf::IntRect(16, 0, 16, 16));
		this->player->setAbilityMaxTime(10.f);
		this->player->setAbilityCooldown(10.f);
		this->player->setAbilityTime(5.f);
	}
	else if (name == "SCOUT") {
		this->sprites["MINIATURE"]->setTextureRect(sf::IntRect(48, 0, 16, 16));
		this->player->setTexturePath("external/assets/heroes/scout.png");
		this->player->setReg(this->player->getReg() + 1);
		this->texts["REG"]->setText(std::to_string(this->player->getReg()));
		this->texts["REG"]->center(calcX(452, vm));
		this->player->setName("scout");
		this->sprites["ABILITY_ICON"]->setTextureRect(sf::IntRect(32, 0, 16, 16));
		this->player->setAbilityMaxTime(10.f);
		this->player->setAbilityCooldown(10.f);
		this->player->setAbilityTime(5.f);
	}
	else if (name == "MASTER") {
		this->sprites["MINIATURE"]->setTextureRect(sf::IntRect(64, 0, 16, 16));
		this->player->setTexturePath("external/assets/heroes/master.png");
		this->player->setAttackSpeed(this->player->getAttackSpeed() + 1);
		this->texts["ATTACK_SPEED"]->setText(std::to_string(this->player->getAttackSpeed()));
		this->texts["ATTACK_SPEED"]->center(calcX(892, vm));
		this->player->setName("master");
		this->sprites["ABILITY_ICON"]->setTextureRect(sf::IntRect(48, 0, 16, 16));
		this->player->setAbilityMaxTime(2.f);
		this->player->setAbilityCooldown(2.f);
		this->player->setAbilityTime(2.f);
	}
	else if (name == "BOMBER") {
		this->sprites["MINIATURE"]->setTextureRect(sf::IntRect(80, 0, 16, 16));
		this->player->setTexturePath("external/assets/heroes/bomber.png");
		this->player->setCriticalChance(this->player->getCriticalChance() + 10);
		this->texts["CRITICAL"]->setText(std::to_string(this->player->getCriticalChance()));
		this->texts["CRITICAL"]->center(calcX(1020, vm));
		this->player->setName("bomber");
		this->sprites["ABILITY_ICON"]->setTextureRect(sf::IntRect(64, 0, 16, 16));
		this->player->setAbilityMaxTime(2.f);
		this->player->setAbilityCooldown(2.f);
		this->player->setAbilityTime(2.f);
	}
	else if (name == "CRUSADER") {
		this->sprites["MINIATURE"]->setTextureRect(sf::IntRect(96, 0, 16, 16));
		this->player->setTexturePath("external/assets/heroes/crusader.png");
		this->player->setMaxHP(this->player->getMaxHP() + 2);
		this->update_HP();
		this->player->setIsRegenerating(true);
		this->player->setName("crusader");
		this->sprites["ABILITY_ICON"]->setTextureRect(sf::IntRect(80, 0, 16, 16));
		this->player->setAbilityMaxTime(10.f);
		this->player->setAbilityCooldown(10.f);
		this->player->setAbilityTime(5.f);
	}
}

void PlayerGUI::update_level(SoundEngine* soundEngine)
{
	soundEngine->addSound("levelup");
	this->isLeveling = true;
	this->isShopping = false;
	this->sprites["XP_BAR"]->setTextureRect(sf::IntRect(0, 0, 0, 22));
	this->texts["LEVEL"]->setText("Level " + std::to_string(this->player->getLevel()));
	this->texts["LEVEL"]->center(calcX(640, this->vm));
	if (this->player->getLevel() % 5 == 0) this->isUpgrading = true;

	std::vector<short> id = { 1 };
	if (this->player->getAttackSpeed() < 10) id.push_back(2);
	if (this->player->getSpeed() < 10) id.push_back(3);
	if (this->player->getCriticalChance() < 100) id.push_back(4);
	if (this->player->getReg() < 10) id.push_back(5);
	id.push_back(6);
	update_options(this->option1_id, this->option1_val, id, this->texts["OPTION1"], this->sprites["OPTION1"], calcX(548, vm));
	update_options(this->option2_id, this->option2_val, id, this->texts["OPTION2"], this->sprites["OPTION2"], calcX(732, vm));

	if (this->player->getLevel() == 10) {
		if (this->player->getName() == "ninja") {
			this->sprites["UPGRADE1"] = new gui::Sprite(this->upgrades_vec[4], calcX(1020, vm), calcY(228, vm), calcScale(4, vm), false);
			this->sprites["UPGRADE2"] = new gui::Sprite(this->upgrades_vec[5], calcX(1020, vm), calcY(392, vm), calcScale(4, vm), false);

			this->texts["UPGRADE1_NAME"] = new gui::Text(&this->font, this->lang["MASTER"], calcChar(16, vm), calcX(1104, vm), calcY(228, vm), sf::Color(255, 255, 255), false);
			this->texts["UPGRADE2_NAME"] = new gui::Text(&this->font, this->lang["BOMBER"], calcChar(16, vm), calcX(1104, vm), calcY(388, vm), sf::Color(255, 255, 255), false);

			this->sprites["UPGRADE1_ABILITY"] = new gui::Sprite(this->abilities_vec[3], calcX(1168, vm), calcY(262, vm), calcScale(2, vm), false);
			this->sprites["UPGRADE2_ABILITY"] = new gui::Sprite(this->abilities_vec[4], calcX(1168, vm), calcY(426, vm), calcScale(2, vm), false);

			this->sprites["UPGRADE1_ADD"] = new gui::Sprite(this->attribute_vec[2], calcX(1216, vm), calcY(228, vm), calcScale(2, vm), false);
			this->sprites["UPGRADE2_ADD"] = new gui::Sprite(this->attribute_vec[4], calcX(1216, vm), calcY(392, vm), calcScale(2, vm), false);
		}
		else if (this->player->getName() == "knight") {
			this->sprites["UPGRADE1"] = new gui::Sprite(this->upgrades_vec[6], calcX(1020, vm), calcY(228, vm), calcScale(4, vm), false);
			//this->sprites["UPGRADE2"] = new gui::Sprite(this->upgrades_vec[5], calcX(1020, vm), calcY(392, vm), calcScale(4, vm), false);

			this->texts["UPGRADE1_NAME"] = new gui::Text(&this->font, this->lang["CRUSADER"], calcChar(16, vm), calcX(1104, vm), calcY(228, vm), sf::Color(255, 255, 255), false);
			//this->texts["UPGRADE2_NAME"] = new gui::Text(&this->font, this->lang["BOMBER"], calcChar(16, vm), calcX(1104, vm), calcY(388, vm), sf::Color(255, 255, 255), false);

			this->sprites["UPGRADE1_ABILITY"] = new gui::Sprite(this->abilities_vec[5], calcX(1168, vm), calcY(262, vm), calcScale(2, vm), false);
			//this->sprites["UPGRADE2_ABILITY"] = new gui::Sprite(this->abilities_vec[4], calcX(1168, vm), calcY(426, vm), calcScale(2, vm), false);

			this->sprites["UPGRADE1_ADD"] = new gui::Sprite(this->attribute_vec[6], calcX(1216, vm), calcY(228, vm), calcScale(2, vm), false);
			//this->sprites["UPGRADE2_ADD"] = new gui::Sprite(this->attribute_vec[4], calcX(1216, vm), calcY(392, vm), calcScale(2, vm), false);
			this->texts["UPGRADE1_ADD_VALUE"]->setText("+2");
		}
	}
}

void PlayerGUI::update_XP()
{
	this->xp_bar_barrier = static_cast<float>(this->player->getXP() - this->player->getLastMaxXP()) / static_cast<float>(this->player->getMaxXP() - this->player->getLastMaxXP());
	this->texts["XP"]->setText("XP:" + std::to_string(this->player->getXP()) + "/" + std::to_string(this->player->getMaxXP()));
	this->texts["XP"]->center(calcX(640, vm));
}

void PlayerGUI::updating_XP(const float& dt)
{
	const int width = this->sprites["XP_BAR"]->getTextureRect().width;
	const int barrier = static_cast<int>(this->xp_bar_barrier * 264.f);
	
	if (width < barrier && this->player->getLeveling()) {
		const int distance = int(width + 1000.f * dt);
		if (distance > barrier) {
			this->sprites["XP_BAR"]->setTextureRect(sf::IntRect(0, 0, barrier, 22));
		}
		else if (distance > 264) {
			this->sprites["XP_BAR"]->setTextureRect(sf::IntRect(0, 0, 264, 22));
		}
		else {
			this->sprites["XP_BAR"]->setTextureRect(sf::IntRect(0, 0, distance, 22));
		}
	}
	else this->player->setIsLeveling(false);
}

void PlayerGUI::update_HP()
{
	if (this->player->isDead()) {
		this->hp_bar_barrier = 0.f;
		this->sprites["HP_BAR"]->setTextureRect(sf::IntRect(0, 22, 0, 22));
		this->isShopping = false;
	}
	else this->hp_bar_barrier = static_cast<float>(this->player->getHP()) / this->player->getMaxHP();

	this->texts["HP"]->setText("HP:" + std::to_string(this->player->getHP()) + "/" + std::to_string(this->player->getMaxHP()));
	this->texts["HP"]->center(calcX(640, vm));
}

void PlayerGUI::updating_HP(SoundEngine* soundEngine, const float& dt)
{
	if (this->player->regeneration(dt) && !this->player->isDead()) {
		this->texts["HP"]->setText("HP:" + std::to_string(this->player->getHP()) + "/" + std::to_string(this->player->getMaxHP()));
		this->texts["HP"]->center(calcX(640, this->vm));
		this->hp_bar_barrier = static_cast<float>(this->player->getHP()) / this->player->getMaxHP();
		this->player->setIsRegenerating(true);
	}
	else if (this->player->isDead()) {
		this->hp_bar_barrier = 0.f;
		this->sprites["HP_BAR"]->setTextureRect(sf::IntRect(0, 22, 0, 22));
		this->text_buttons["QUIT"]->setPosition(sf::Vector2f(calcX(640, vm), calcY(488, vm)));
		this->text_buttons["QUIT"]->center(calcX(640, vm));
		soundEngine->addSound("gameover");
	}

	const int width = this->sprites["HP_BAR"]->getTextureRect().width;
	const int barrier = static_cast<int>(this->hp_bar_barrier * 264.f);

	if (width > barrier) {
		const int distance = int(width - 1000.f * dt);
		if (distance < barrier) {
			this->sprites["HP_BAR"]->setTextureRect(sf::IntRect(0, 22, barrier, 22));
			this->sprites["PROGRESS_BAR"]->setColor(sf::Color::White);
		}
		else if (distance < 0) {
			this->sprites["HP_BAR"]->setTextureRect(sf::IntRect(0, 22, 0, 22));
			this->sprites["PROGRESS_BAR"]->setColor(sf::Color::Transparent);
		}
		else {
			this->sprites["HP_BAR"]->setTextureRect(sf::IntRect(0, 22, distance, 22));
			this->sprites["PROGRESS_BAR"]->setColor(sf::Color::White);
		}
	}
	else if (width < barrier && this->player->getRegenerating()) {
		const int distance = int(width + 1000.f * dt);
		if (distance > barrier) {
			this->sprites["HP_BAR"]->setTextureRect(sf::IntRect(0, 22, barrier, 22));
			this->sprites["PROGRESS_BAR"]->setColor(sf::Color::White);
		}
		else if (distance > 264) {
			this->sprites["HP_BAR"]->setTextureRect(sf::IntRect(0, 22, 264, 22));
			this->sprites["PROGRESS_BAR"]->setColor(sf::Color::Transparent);
		}
		else {
			this->sprites["HP_BAR"]->setTextureRect(sf::IntRect(0, 22, distance, 22));
			this->sprites["PROGRESS_BAR"]->setColor(sf::Color::White);
		}
	}
	else {
		this->player->setIsRegenerating(false);
		this->sprites["PROGRESS_BAR"]->setColor(sf::Color::Transparent);
	}

}

void PlayerGUI::update_Gold()
{
	this->texts["GOLD"]->setText(std::to_string(this->player->getGold()));
}

void PlayerGUI::update_ability(const float& dt)
{
	if (this->player->getAbilityCooldown() > 0.f) {
		const float value = this->player->getAbilityCooldown() / this->player->getAbilityMaxTime() * calcX(64, vm);
		this->ability_icon.setSize(sf::Vector2f(calcX(64, vm), calcX(64.f, vm) - value));
		this->ability_icon.setPosition(sf::Vector2f(calcX(288, vm), calcX(16.f, vm) + value));
	}
}

void PlayerGUI::setAbilityIcon()
{
	this->ability_icon.setSize(sf::Vector2f(calcX(64, vm), calcY(64, vm)));
	this->ability_icon.setPosition(sf::Vector2f(calcX(288, vm), calcY(16, vm)));
}

void PlayerGUI::updateIsShopping()
{
	if (this->isShopping) {
		this->isShopping = false;
	}
	else {
		this->isShopping = true;
	}
}

void PlayerGUI::updateKills()
{
	this->texts["KILLS"]->setText(this->lang["KILLS"] + std::to_string(this->player->getKills()));
}

void PlayerGUI::updateMonsterCountWave(const std::string& language, const uint32_t& wave, const bool& bossWave, const size_t& monsterCount, SoundEngine* soundEngine)
{
	this->bossWave = bossWave;

	this->texts["WAVE_NUMBER"]->setText(this->lang["WAVE"] + std::to_string(wave));

	this->texts["BIG_WAVE_NUMBER"]->setText(this->lang["WAVE"] + std::to_string(wave));
	this->texts["BIG_WAVE_NUMBER"]->center(calcX(640, this->vm));


	switch (wave) {
	case 1:
		this->texts["WAVE_NEW_MOBS"]->setText(this->lang["NEW_TYPE"] + this->lang["GOBLIN"]);
		break;
	case 3:
		this->texts["WAVE_NEW_MOBS"]->setText(this->lang["NEW_TYPE"] + this->lang["SPIDER"]);
		break;
	case 5:
		this->texts["WAVE_NEW_MOBS"]->setText(this->lang["NEW_TYPE"] + this->lang["ORC"]);
		break;
	case 7:
		this->texts["WAVE_NEW_MOBS"]->setText(this->lang["NEW_TYPE"] + this->lang["CYCLOPE"]);
		break;
	case 10:
		this->texts["WAVE_NEW_MOBS"]->setText(this->lang["NEW_TYPE"] + this->lang["MINOTAUR"]);
		break;
	default:
		this->texts["WAVE_NEW_MOBS"]->setText("");
		break;
	}

	this->texts["WAVE_NEW_MOBS"]->center(calcX(640, this->vm));

	this->texts["MONSTER_COUNT"]->setText(this->lang["MONSTER_COUNT"] + std::to_string(monsterCount));
	
	if (language == "polish") {
		if (monsterCount > 10) {
			if (monsterCount % 10 >= 2 && monsterCount % 10 <= 4)
				this->texts["MOBS_TO_KILL"]->setText(std::to_string(monsterCount) + " potwory");
			else
				this->texts["MOBS_TO_KILL"]->setText(std::to_string(monsterCount) + " potwor�w");
		}
		else {
			if (monsterCount == 1)
				this->texts["MOBS_TO_KILL"]->setText(std::to_string(monsterCount) + " potwor");
			else if (monsterCount < 5)
				this->texts["MOBS_TO_KILL"]->setText(std::to_string(monsterCount) + " potwory");
			else
				this->texts["MOBS_TO_KILL"]->setText(std::to_string(monsterCount) + " potwor�w");
		}
	}
	else {
		if (monsterCount == 1)
			this->texts["MOBS_TO_KILL"]->setText(std::to_string(monsterCount) + " monster");
		else
			this->texts["MOBS_TO_KILL"]->setText(std::to_string(monsterCount) + " monsters");
	}
	this->texts["MOBS_TO_KILL"]->center(calcX(640, this->vm));

	soundEngine->addSound("new_wave");
}

void PlayerGUI::updateMonsterCount(const size_t& monsterCount)
{
	this->texts["MONSTER_COUNT"]->setText(this->lang["MONSTER_COUNT"] + std::to_string(monsterCount));
}

void PlayerGUI::setIsEscape(const bool& escape)
{
	this->isEscape = escape;
}

void PlayerGUI::updatePaused(bool& paused)
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
	this->texts["ARMOR"]->setText(std::to_string(this->player->getArmor()));
	this->texts["ARMOR"]->center(calcX(388, vm));
}

void PlayerGUI::updateAttack()
{
	this->texts["ATTACK"]->setText(std::to_string(this->player->getAttack()));
	this->texts["ATTACK"]->center(calcX(828, vm));
}

void PlayerGUI::updateBossHP(const float& dt)
{
	if (this->bossWave) {
		const int width = this->sprites["BOSS_BAR"]->getTextureRect().width;
		const int barrier = static_cast<int>(this->boss_bar_barrier * 528.f);

		if (width > barrier) {
			const int distance = int(width - 1000.f * dt);
			if (distance < barrier) {
				this->sprites["BOSS_BAR"]->setTextureRect(sf::IntRect(0, 44, barrier, 22));
			}
			else if (distance < 0) {
				this->sprites["BOSS_BAR"]->setTextureRect(sf::IntRect(0, 44, 0, 22));
			}
			else {
				this->sprites["BOSS_BAR"]->setTextureRect(sf::IntRect(0, 44, distance, 22));
			}
		}
	}
}

const bool PlayerGUI::updateShop(const sf::Vector2i& mousePos, const bool& mouseClicked, SoundEngine* soundEngine, FloatingTextSystem* floatingTextSystem)
{
	if (this->isShopping) {
		if (this->player->getGold() >= this->item1Price && this->player->getHP() < this->player->getMaxHP()) {
			this->sprite_buttons["ITEM1"]->update(mousePos);
			if (this->sprite_buttons["ITEM1"]->isPressed() && !mouseClicked) {
				this->player->setGold(this->player->getGold() - this->item1Price);
				floatingTextSystem->addFloatingText("-" + std::to_string(this->item1Price), calcChar(16, vm), calcX(20, vm), calcY(96, vm), sf::Color(255, 246, 76), true);
				floatingTextSystem->addFloatingText("Full HP", calcChar(16, vm), static_cast<float>(mousePos.x), static_cast<float>(mousePos.y), sf::Color(255, 255, 255), true);
				this->texts["GOLD"]->setText(std::to_string(this->player->getGold()));
				this->player->setHP(this->player->getMaxHP());
				this->update_HP();
				this->player->setIsRegenerating(true);
				this->sprite_buttons["ITEM1"]->setTransparent();
				this->item1Price += static_cast<uint32_t>((((1 + sqrtf(5)) / 2.f) - 1) * this->item1Price);
				this->texts["ITEM1_PRICE"]->setText(std::to_string(this->item1Price));
				soundEngine->addSound("buy");
				return true;
			}
		}

		if (this->player->getGold() >= this->item2Price) {
			this->sprite_buttons["ITEM2"]->update(mousePos);
			if (this->sprite_buttons["ITEM2"]->isPressed() && !mouseClicked) {
				this->player->setGold(this->player->getGold() - this->item2Price);
				floatingTextSystem->addFloatingText("-" + std::to_string(this->item2Price), calcChar(16, vm), calcX(20, vm), calcY(96, vm), sf::Color(255, 246, 76), true);
				floatingTextSystem->addFloatingText("+2", calcChar(16, vm), static_cast<float>(mousePos.x), static_cast<float>(mousePos.y), sf::Color(255, 255, 255), true);
				this->texts["GOLD"]->setText(std::to_string(this->player->getGold()));
				this->player->setMaxHP(this->player->getMaxHP() + 2);
				this->update_HP();
				this->player->setIsRegenerating(true);
				this->sprite_buttons["ITEM2"]->setTransparent();
				this->item2Price += static_cast<uint32_t>((((1 + sqrtf(5)) / 2.f) - 1) * this->item2Price);
				this->texts["ITEM2_PRICE"]->setText(std::to_string(this->item2Price));
				soundEngine->addSound("buy");
				return true;
			}
		}


		if (this->player->getGold() >= this->item3Price) {
			this->sprite_buttons["ITEM3"]->update(mousePos);
			if (this->sprite_buttons["ITEM3"]->isPressed() && !mouseClicked) {
				this->player->setGold(this->player->getGold() - this->item3Price);
				floatingTextSystem->addFloatingText("-" + std::to_string(this->item3Price), calcChar(16, vm), calcX(20, vm), calcY(96, vm), sf::Color(255, 246, 76), true);
				floatingTextSystem->addFloatingText("+1", calcChar(16, vm), static_cast<float>(mousePos.x), static_cast<float>(mousePos.y), sf::Color(255, 255, 255), true);
				this->texts["GOLD"]->setText(std::to_string(this->player->getGold()));
				this->player->setAttack(this->player->getAttack() + 1);
				this->texts["ATTACK"]->setText(std::to_string(this->player->getAttack()));
				this->texts["ATTACK"]->center(calcX(828, vm));
				this->sprite_buttons["ITEM3"]->setTransparent();
				this->item3Price += static_cast<uint32_t>((((1 + sqrtf(5)) / 2.f) - 1) * this->item3Price);
				this->texts["ITEM3_PRICE"]->setText(std::to_string(this->item3Price));
				soundEngine->addSound("buy");
				return true;
			}
		}

		if (this->player->getArmor() < 10 || (this->player->getName() == "knight" && ((this->player->getAbilityActive() && this->player->getArmor() < 15))
			|| (!this->player->getAbilityActive() && this->player->getArmor() < 10))) {
			if (this->player->getGold() >= this->item4Price) {
				this->sprite_buttons["ITEM4"]->update(mousePos);
				if (this->sprite_buttons["ITEM4"]->isPressed() && !mouseClicked) {
					this->player->setGold(this->player->getGold() - this->item4Price);
					floatingTextSystem->addFloatingText("-" + std::to_string(this->item4Price), calcChar(16, vm), calcX(20, vm), calcX(96, vm), sf::Color(255, 246, 76), true);
					floatingTextSystem->addFloatingText("+1", calcChar(16, vm), static_cast<float>(mousePos.x), static_cast<float>(mousePos.y), sf::Color(255, 255, 255), true);
					this->texts["GOLD"]->setText(std::to_string(this->player->getGold()));
					this->player->setArmor(this->player->getArmor() + 1);
					this->texts["ARMOR"]->setText(std::to_string(this->player->getArmor()));
					this->texts["ARMOR"]->center(calcX(388, vm));
					this->sprite_buttons["ITEM4"]->setTransparent();
					this->item4Price += static_cast<uint32_t>((((1 + sqrtf(5)) / 2.f) - 1) * this->item4Price);
					this->texts["ITEM4_PRICE"]->setText(std::to_string(this->item4Price));
					soundEngine->addSound("buy");
					return true;
				}
			}
		}
	}

	return false;
}

const bool PlayerGUI::getIsEscape() const
{
	return this->isEscape;
}

const bool PlayerGUI::getIsLeveling() const
{
	return this->isLeveling;
}

const bool PlayerGUI::getIsUpgrading() const
{
	return this->isUpgrading;
}

const bool PlayerGUI::getIsShopping() const
{
	return this->isShopping;
}

const uint8_t PlayerGUI::updateEscapeButton(const sf::Vector2i& mousePos, const bool& mouseClicked)
{
	this->text_buttons["RESUME"]->update(mousePos);
	if (this->text_buttons["RESUME"]->isPressed() && !mouseClicked) {
		this->isEscape = false;
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

const bool PlayerGUI::updateButtons(const sf::Vector2i& mousePos, const bool& mouseClicked, SoundEngine* soundEngine)
{
	if (this->isLeveling && this->isUpgrading) {
		const bool u = this->updateLevelUpButtons(mousePos, mouseClicked, soundEngine);
		const bool w = this->updateUpgradeButtons(mousePos, mouseClicked, soundEngine);
	}
	else if (this->isLeveling) {
		return this->updateLevelUpButtons(mousePos, mouseClicked, soundEngine);
	}
	else if (this->isUpgrading) {
		return this->updateUpgradeButtons(mousePos, mouseClicked, soundEngine);
	}
	return false;
}

const bool PlayerGUI::updateLevelUpButtons(const sf::Vector2i& mousePos, const bool& mouseClicked, SoundEngine* soundEngine)
{
	this->sprite_buttons["OPTION1"]->update(mousePos);
	if (this->sprite_buttons["OPTION1"]->isPressed() && !mouseClicked) {
		this->levelUpPlayer(this->option1_id, this->option1_val);
		this->isLeveling = false;
		this->sprite_buttons["OPTION1"]->setTransparent();
		soundEngine->addSound("option");
		return true;
	}

	this->sprite_buttons["OPTION2"]->update(mousePos);
	if (this->sprite_buttons["OPTION2"]->isPressed() && !mouseClicked) {
		this->levelUpPlayer(this->option2_id, this->option2_val);
		this->isLeveling = false;
		this->sprite_buttons["OPTION2"]->setTransparent();
		soundEngine->addSound("option");
		return true;
	}
	return false;
}

const bool PlayerGUI::updateUpgradeButtons(const sf::Vector2i& mousePos, const bool& mouseClicked, SoundEngine* soundEngine)
{
	this->sprite_buttons["UPGRADE1"]->update(mousePos);
	if (this->sprite_buttons["UPGRADE1"]->isPressed() && !mouseClicked) {
		if (this->player->getLevel() == 5) {
			this->upgradePlayer("NINJA");
		}
		else if (this->player->getLevel() == 10) {
			if (this->player->getName() == "ninja") {
				this->upgradePlayer("MASTER");
			}
			else if (this->player->getName() == "knight") {
				this->upgradePlayer("CRUSADER");
			}
		}
		
		this->isUpgrading = false;
		this->sprite_buttons["UPGRADE1"]->setTransparent();
		soundEngine->addSound("upgrade");
		return true;
	}

	this->sprite_buttons["UPGRADE2"]->update(mousePos);
	if (this->sprite_buttons["UPGRADE2"]->isPressed() && !mouseClicked) {
		if (this->player->getLevel() == 5) {
			this->upgradePlayer("KNIGHT");
		}
		else if (this->player->getLevel() == 10) {
			if (this->player->getName() == "ninja") {
				this->upgradePlayer("BOMBER");
			}
			else if (this->player->getName() == "knight") {
				
			}
		}
		this->isUpgrading = false;
		this->sprite_buttons["UPGRADE2"]->setTransparent();
		soundEngine->addSound("upgrade");
		return true;
	}

	if (this->player->getLevel() == 5) {
		this->sprite_buttons["UPGRADE3"]->update(mousePos);
		if (this->sprite_buttons["UPGRADE3"]->isPressed() && !mouseClicked) {
			this->upgradePlayer("SCOUT");
			this->isUpgrading = false;
			this->sprite_buttons["UPGRADE3"]->setTransparent();
			soundEngine->addSound("upgrade");
			return true;
		}
	}
	return false;
}

const uint8_t PlayerGUI::updateDeathScreenButtons(const sf::Vector2i& mousePos, const bool& mouseClicked)
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

void PlayerGUI::update(sf::Vector2f& mousePosView, const float& waveCountdown, MonsterSystem* monsterSystem, const float& dt)
{
	this->waveCountdown = waveCountdown;

	if (this->bossWave) this->boss_bar_barrier = monsterSystem->bossHP();

	this->isLevelshown = true;
	if ((mousePosView.y >= calcY(12, vm) && mousePosView.y <= calcY(34, vm))
		&& (mousePosView.x >= calcX(508, vm) && mousePosView.x <= calcX(772, vm))) {
		this->isLevelshown = false;
	}

	if (this->waveCountdown < 10.f) {
		this->texts["WAVE_COUNTDOWN"]->setText(this->lang["NEXT_WAVE"] + std::to_string(int(11.f - this->waveCountdown)));
		if (this->waveCountdown > 8.f && this->waveCountdown < 9.f && this->titleCooldown < 255.f) {
			this->titleCooldown += dt * 1000.f;
			if (this->titleCooldown > 255.f) this->titleCooldown = 255.f;
			this->texts["BIG_WAVE_NUMBER"]->setFillColor(sf::Color(255, 255, 255, sf::Uint8(this->titleCooldown)));
			this->texts["WAVE_NEW_MOBS"]->setFillColor(sf::Color(228, 92, 95, sf::Uint8(this->titleCooldown)));
			this->texts["MOBS_TO_KILL"]->setFillColor(sf::Color(192, 192, 192, sf::Uint8(this->titleCooldown)));
		}
		else if (this->waveCountdown > 9.f && this->titleCooldown > 0.f) {
			this->titleCooldown -= dt * 1000.f;
			if (this->titleCooldown < 0.f) this->titleCooldown = 0.f;
			this->texts["BIG_WAVE_NUMBER"]->setFillColor(sf::Color(255, 255, 255, sf::Uint8(this->titleCooldown)));
			this->texts["WAVE_NEW_MOBS"]->setFillColor(sf::Color(228, 92, 95, sf::Uint8(this->titleCooldown)));
			this->texts["MOBS_TO_KILL"]->setFillColor(sf::Color(192, 192, 192, sf::Uint8(this->titleCooldown)));
		}

		this->texts["WAVE_COUNTDOWN"]->setText(this->lang["NEXT_WAVE"] + std::to_string(10 - int(waveCountdown)));
	}
}

void PlayerGUI::draw(sf::RenderTarget& target)
{
	this->sprites["BAR"]->draw(target);
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
	if (this->isLevelshown)
		this->texts["LEVEL"]->draw(target);
	else
		this->texts["XP"]->draw(target);
	this->sprites["HP_BAR"]->draw(target);
	this->texts["HP"]->draw(target);

	if (this->isShopping) {
		this->sprites["SHOP"]->draw(target);
		this->sprite_buttons["ITEM1"]->draw(target);
		this->sprites["ITEM1"]->draw(target);
		this->texts["ITEM1"]->draw(target);
		this->texts["ITEM1_PRICE"]->draw(target);
		this->sprites["ITEM1_COIN"]->draw(target);

		this->sprite_buttons["ITEM2"]->draw(target);
		this->sprites["ITEM2"]->draw(target);
		this->texts["ITEM2"]->draw(target);
		this->texts["ITEM2_PRICE"]->draw(target);
		this->sprites["ITEM2_COIN"]->draw(target);

		this->sprite_buttons["ITEM3"]->draw(target);
		this->sprites["ITEM3"]->draw(target);
		this->texts["ITEM3"]->draw(target);
		this->texts["ITEM3_PRICE"]->draw(target);
		this->sprites["ITEM3_COIN"]->draw(target);

		if (this->player->getArmor() < 10 || (this->player->getName() == "knight" && ((this->player->getAbilityActive() && this->player->getArmor() < 15))
			|| (!this->player->getAbilityActive() && this->player->getArmor() < 10))) {
			this->sprite_buttons["ITEM4"]->draw(target);
			this->sprites["ITEM4"]->draw(target);
			this->texts["ITEM4"]->draw(target);
			this->texts["ITEM4_PRICE"]->draw(target);
			this->sprites["ITEM4_COIN"]->draw(target);
		}
	}

	if (this->player->getName() != "warrior") {
		this->sprites["ABILITY_ICON"]->draw(target);
		if (this->player->getAbilityCooldown() < this->player->getAbilityMaxTime())
			target.draw(this->ability_icon);
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

		if (this->isLeveling) {
			this->texts["LEVEL_UP"]->draw(target);
			this->sprites["UPGRADE_BAR"]->draw(target);
			this->sprites["OPTION1"]->draw(target);
			this->sprite_buttons["OPTION1"]->draw(target);
			this->texts["OPTION1"]->draw(target);
			this->sprites["OPTION2"]->draw(target);
			this->sprite_buttons["OPTION2"]->draw(target);
			this->texts["OPTION2"]->draw(target);
		}
		if (this->isUpgrading) {
			this->sprites["SIDE_BAR"]->draw(target);

			this->sprites["UPGRADE1"]->draw(target);
			this->sprite_buttons["UPGRADE1"]->draw(target);
			this->texts["UPGRADE1_NAME"]->draw(target);
			this->sprites["UPGRADE1_ABILITY"]->draw(target);
			this->sprites["UPGRADE1_ADD"]->draw(target);
			this->texts["UPGRADE1_ADD_VALUE"]->draw(target);

			this->sprites["UPGRADE2"]->draw(target);
			this->sprite_buttons["UPGRADE2"]->draw(target);
			this->texts["UPGRADE2_NAME"]->draw(target);
			this->sprites["UPGRADE2_ABILITY"]->draw(target);
			this->sprites["UPGRADE2_ADD"]->draw(target);
			this->texts["UPGRADE2_ADD_VALUE"]->draw(target);

			this->sprites["UPGRADE3"]->draw(target);
			this->sprite_buttons["UPGRADE3"]->draw(target);
			this->texts["UPGRADE3_NAME"]->draw(target);
			this->sprites["UPGRADE3_ABILITY"]->draw(target);
			this->sprites["UPGRADE3_ADD"]->draw(target);
			this->texts["UPGRADE3_ADD_VALUE"]->draw(target);
		}
		if (this->bossWave) {
			this->texts["BOSS"]->draw(target);
			this->sprites["BOSS_BAR_EMPTY"]->draw(target);
			this->sprites["BOSS_BAR"]->draw(target);
		}
	}

	if (this->player->isDead()) {
		target.draw(this->death_background);
		this->texts["YOU_DIED"]->draw(target);
		this->texts["KILLS"]->draw(target);
		this->text_buttons["MAIN_MENU"]->draw(target);
		this->text_buttons["QUIT"]->draw(target);
	}
	else if (this->isEscape) {
		target.draw(this->escape_background);
		this->texts["DIFFICULTY"]->draw(target);
		this->text_buttons["RESUME"]->draw(target);
		this->text_buttons["MAIN_MENU"]->draw(target);
		this->text_buttons["QUIT"]->draw(target);
		this->text_buttons["SETTINGS"]->draw(target);
	}
}
