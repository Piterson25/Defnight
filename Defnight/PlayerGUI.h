#ifndef PLAYERGUI_H
#define PLAYERGUI_H

#include "Player.h"
#include "Gui.h"
#include "FloatingTextSystem.h"
#include "MonsterSystem.h"
#include "SoundEngine.h"

class Player;
class FloatingTextSystem;
class MonsterSystem;
class SoundEngine;

class PlayerGUI
{
public:
	PlayerGUI(sf::Font* font, Player* player, sf::VideoMode& vm, const float& soundVolume, const std::string& heroName,
		const std::string& difficulty_name, std::unordered_map<std::string, std::string>& lang);
	virtual ~PlayerGUI();

	void update_options(uint32_t& option_id, uint32_t& option_val, std::vector<short>& id_vector, gui::Text* text, gui::Sprite* sprite, const float& pos);
	void levelUpPlayer(uint32_t& option_id, uint32_t& option_val);
	void upgradePlayer(const std::string& name);
	void update_level(SoundEngine* soundEngine);
	void update_XP();
	void updating_XP(const float& dt);
	void update_HP();
	void updating_HP(SoundEngine* soundEngine, const float& dt);
	void update_Gold();
	void update_ability(const float& dt);
	void setAbilityIcon();
	void setIsEscape(const bool& escape);
	void updatePaused(bool& paused);
	void updateArmor();
	void updateBossHP(const float& dt);
	const bool updateShop(const sf::Vector2i& mousePos, const bool& mouseClicked, SoundEngine* soundEngine, FloatingTextSystem* floatingTextSystem);

	const bool getIsEscape() const;
	const bool getIsLeveling() const;
	const bool getIsUpgrading() const;
	const bool getIsShopping() const;

	void updateIsShopping();
	void updateKills();
	void updateMonsterCountWave(const std::string& language, const uint32_t& wave, const bool& bossWave, const size_t& monsterCount, SoundEngine* soundEngine);
	void updateMonsterCount(const size_t& monsterCount);
	const uint8_t updateEscapeButton(const sf::Vector2i& mousePos, const bool& mouseClicked);
	const bool updateButtons(const sf::Vector2i& mousePos, const bool& mouseClicked, SoundEngine* soundEngine);
	const bool updateLevelUpButtons(const sf::Vector2i& mousePos, const bool& mouseClicked, SoundEngine* soundEngine);
	const bool updateUpgradeButtons(const sf::Vector2i& mousePos, const bool& mouseClicked, SoundEngine* soundEngine);
	const uint8_t updateDeathScreenButtons(const sf::Vector2i& mousePos, const bool& mouseClicked);
	void update(sf::Vector2f& mousePosView, const float& waveCountdown, MonsterSystem* monsterSystem, const float& dt);
	void draw(sf::RenderTarget& target);
private:
	sf::Font font;

	Player* player;
	sf::VideoMode& vm;

	sf::Texture attributes_texture;
	std::vector<sf::Sprite> attribute_vec;

	sf::Texture upgrades_texture;
	std::vector<sf::Sprite> upgrades_vec;

	sf::Texture abilities_texture;
	std::vector<sf::Sprite> abilities_vec;

	bool bossWave;

	bool isLevelshown;
	bool isLeveling;
	bool isUpgrading;
	bool isShopping;
	bool isEscape;
	float hp_bar_barrier;
	float xp_bar_barrier;
	float boss_bar_barrier;
	uint32_t option1_id;
	uint32_t option2_id;
	uint32_t option1_val;
	uint32_t option2_val;

	uint16_t item1Price;
	uint16_t item2Price;
	uint16_t item3Price;

	float titleCooldown;
	float waveCountdown;

	sf::RectangleShape ability_icon;

	sf::RectangleShape death_background;
	sf::RectangleShape escape_background;

	std::unordered_map<std::string, std::string> lang;
	std::unordered_map<std::string, gui::ButtonText*> text_buttons;
	std::unordered_map<std::string, gui::Text*> texts;
	std::unordered_map<std::string, gui::ButtonSprite*> sprite_buttons;
	std::unordered_map<std::string, gui::Sprite*> sprites;

};

#endif