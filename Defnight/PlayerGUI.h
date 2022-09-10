#ifndef PLAYERGUI_H
#define PLAYERGUI_H

#include "Player.h"
#include "Gui.h"
#include "FloatingText.h"

class PlayerGUI
{
public:
	PlayerGUI(sf::Font* font, Player* player, sf::VideoMode& vm, const std::string& heroName, 
		const std::string& difficulty_name, std::unordered_map<std::string, std::string>& lang);
	virtual ~PlayerGUI();

	void update_options(unsigned& option_id, unsigned& option_val, std::vector<short>& id_vector, gui::Text* text, gui::Sprite* sprite, const float& pos);
	void levelUpPlayer(unsigned& option_id, unsigned& option_val);
	void upgradePlayer(const std::string& name);
	void update_level();
	void update_XP();
	void updating_XP(const float& dt);
	void update_HP();
	void updating_HP(const float& dt);
	void update_Gold();
	void update_ability(const float& dt);
	void setAbilityIcon();
	void setIsEscape(const bool& escape);
	void updatePaused(bool& paused);
	const bool updateShop(const sf::Vector2i& mousePos, const bool& mouseClicked);

	const bool getIsEscape() const;
	const bool getIsLeveling() const;
	const bool getIsUpgrading() const;
	const bool getIsShopping() const;

	void updateIsShopping(const bool& start);
	void updateKills();
	void updateMonsterCountWave(const std::string& language, const unsigned& wave, const size_t& monsterCount);
	void updateMonsterCount(const size_t& monsterCount);
	const uint16_t updateEscapeButton(const sf::Vector2i& mousePos, const bool& mouseClicked);
	const bool updateButtons(const sf::Vector2i& mousePos, const bool& mouseClicked);
	const bool updateLevelUpButtons(const sf::Vector2i& mousePos, const bool& mouseClicked);
	const bool updateUpgradeButtons(const sf::Vector2i& mousePos, const bool& mouseClicked);
	const uint16_t updateDeathScreenButtons(const sf::Vector2i& mousePos, const bool& mouseClicked);
	void update(sf::Vector2f& mousePosView, const float& waveCountdown, const float& dt);
	void draw(sf::RenderTarget& target);
private:
	sf::Font font;

	Player* player;
	sf::VideoMode& vm;

	sf::Texture attributes_texture;
	std::vector<sf::Sprite> attribute_vec;

	sf::Texture upgrades_texture;
	std::vector<sf::Sprite> upgrades_vec;

	bool isLevelshown;
	bool isLeveling;
	bool isUpgrading;
	bool isShopping;
	bool isEscape;
	float hp_bar_barrier;
	float xp_bar_barrier;
	unsigned option1_id;
	unsigned option2_id;
	unsigned option1_val;
	unsigned option2_val;

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