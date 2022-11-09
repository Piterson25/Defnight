#include "stdafx.h"
#include "Functions.h"
#include "MainMenuState.h"

MainMenuState::MainMenuState(const float& gridSize, sf::RenderWindow* window, GameSettings* grap,
	std::unordered_map<std::string, int>* supportedKeys, sf::Font* font, std::stack<State*>* states)
	: State(gridSize, window, grap, supportedKeys, font, states)
{
	initGUI();
	if (!this->music.openFromFile("external/music/main_menu.ogg")) {
		throw("ERROR - COULDN'T FIND MUSIC");
	}
	this->music.setLoop(true);
	this->music.play();
	this->music.setVolume(this->gameSettings->musicVolume);
}

MainMenuState::~MainMenuState()
{

}

void MainMenuState::initGUI()
{
	const sf::VideoMode vm = this->gameSettings->resolution;

	this->page = 0;
	this->keysClick["Escape"].first = false;
	this->keysClick["Escape"].second = false;

	this->fading = false;
	this->appearing = false;
	this->dimBackground.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
	this->dimBackground.setFillColor(sf::Color(0, 0, 0, 0));

	// PAGE 0 - Loading

	this->introCooldown = 0.f;
	this->dimAlpha = 0.f;
	this->sprites["LOGO"] = new gui::Sprite("external/assets/logo.png", calcX(640, vm), calcY(232, vm), calcScale(8, vm), true);
	this->sprites["LOGO"]->setColor(sf::Color(255, 255, 255, 0));

	// PAGE 1

	Random::Init();

	this->mapVelocity = sf::Vector2f(32.f * cos((3.1415f / 180.f)), 32.f * cos((3.1415f / 180.f)));
	this->mapRotate = Random::Float() * 360.f - 90.f;

	this->mapView.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
	this->mapView.setCenter(static_cast<float>(vm.width / 2), static_cast<float>(vm.height / 2));

	this->map_texture.loadFromFile("external/assets/ruins.png");
	this->map.setTexture(this->map_texture);
	this->map.setScale(calcScale(4, vm), calcScale(4, vm));
	this->map.setPosition(256.f, 256.f);
	this->map.setOrigin(256.f, 256.f);
	this->map.setRotation(this->mapRotate);

	this->dimMap.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
	this->dimMap.setFillColor(sf::Color(0, 0, 0, 192));

	this->sprites["TITLE"] = new gui::Sprite("external/assets/title.png", calcX(640, vm), calcY(144, vm), calcScale(1, vm), true);

	this->text_buttons["PLAY"] = new gui::ButtonText(&this->font, this->lang["PLAY"], calcChar(32, vm), calcX(640, vm), calcY(370, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), true);
	this->text_buttons["SETTINGS"] = new gui::ButtonText(&this->font, this->lang["SETTINGS"], calcChar(32, vm), calcX(640, vm), calcY(466, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), true);
	this->text_buttons["QUIT"] = new gui::ButtonText(&this->font, this->lang["QUIT"], calcChar(32, vm), calcX(640, vm), calcY(558, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), true);

	this->texts["VERSION"] = new gui::Text(&this->font, "v0.2.0", calcChar(16, vm), calcX(1272, vm), calcY(700, vm), sf::Color(255, 255, 255), false);
	this->texts["VERSION"]->setPosition(sf::Vector2f(calcX(1272, vm) - this->texts["VERSION"]->getWidth(), calcY(700, vm)));

	this->quitwindow = false;
	

	this->texts["ARE_YOU_SURE"] = new gui::Text(&this->font, this->lang["ARE_YOU_SURE"], calcChar(32, vm), calcX(640, vm), calcY(250, vm), sf::Color(255, 255, 255), true);
	this->text_buttons["YES"] = new gui::ButtonText(&this->font, this->lang["YES"], calcChar(32, vm), calcX(488, vm), calcY(306, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
	this->text_buttons["NO"] = new gui::ButtonText(&this->font, this->lang["NO"], calcChar(32, vm), calcX(704, vm), calcY(306, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);


	// PAGE 2

	this->sprites["GO_BACK"] = new gui::Sprite("external/assets/go_back.png", calcX(1192, vm), calcY(24, vm), calcX(4, vm), false);
	this->sprite_buttons["GO_BACK"] = new gui::ButtonSprite("external/assets/select_go_back.png", calcX(1192, vm), calcY(24, vm), calcX(4, vm), false);

	this->map_name = "";
	this->texts["CHOOSE_MAP"] = new gui::Text(&this->font, this->lang["CHOOSE_MAP"], calcChar(32, vm), calcX(640, vm), calcY(96, vm), sf::Color(255, 255, 255), true);
	this->sprite_buttons["SELECT_MAP1"] = new gui::ButtonSprite("external/assets/select_map.png", calcX(24, vm), calcY(248, vm), calcScale(1, vm), false);
	this->sprites["MAP1"] = new gui::Sprite("external/assets/ruins.png", calcX(48, vm), calcY(272, vm), calcScale(0.5f, vm), false);
	this->texts["RUINS"] = new gui::Text(&this->font, this->lang["RUINS"], calcChar(32, vm), calcX(176, vm), calcY(200, vm), sf::Color(255, 255, 255), true);


	// PAGE 3

	this->hero_name = "";
	this->texts["CHOOSE_HERO"] = new gui::Text(&this->font, this->lang["CHOOSE_HERO"], calcChar(32, vm), calcX(640, vm), calcY(96, vm), sf::Color(255, 255, 255), true);
	this->sprite_buttons["SELECT_HERO1"] = new gui::ButtonSprite("external/assets/select_hero.png", calcX(64, vm), calcY(256, vm), calcScale(1, vm), false);
	this->sprites["HERO1"] = new gui::Sprite("external/assets/upgrade_icons.png", calcX(88, vm), calcY(272, vm), calcScale(8, vm), false);
	this->sprites["HERO1"]->setTextureRect(sf::IntRect(0, 0, 16, 16));
	this->texts["WARRIOR"] = new gui::Text(&this->font, this->lang["WARRIOR"], calcChar(32, vm), calcX(150, vm), calcY(200, vm), sf::Color(255, 255, 255), true);

	this->choosing_hero = false;

	this->text_buttons["CHOOSE"] = new gui::ButtonText(&this->font, this->lang["CHOOSE"], calcChar(32, vm), calcX(200, vm), calcY(570, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
	this->sprites["HERO_PREVIEW"] = new gui::Sprite("external/assets/upgrade_icons.png", calcX(640, vm), calcY(512, vm), calcScale(8, vm), true);
	this->sprites["HERO_PREVIEW"]->setTextureRect(sf::IntRect(0, 0, 16, 16));
	this->sprites["HERO_PREVIEW"]->center(calcX(640, vm));
	this->sprites["HP_BAR"] = new gui::Sprite("external/assets/bars.png", calcX(860, vm), calcY(526, vm), calcScale(1, vm), true);
	this->sprites["HP_BAR"]->setTextureRect(sf::IntRect(0, 22, 264, 22));
	this->sprites["HP_BAR"]->center(calcX(860, vm));
	this->texts["HP"] = new gui::Text(&this->font, "HP:10/10", calcChar(16, vm), calcX(860, vm), calcY(530, vm), sf::Color(255, 255, 255), true);

	this->attributes_texture.loadFromFile("external/assets/icons.png");
	for (short i = 0; i < 8; ++i) {
		sf::Sprite att;
		att.setTexture(this->attributes_texture);
		sf::IntRect intRect(i * 16, 0, 16, 16);
		att.setTextureRect(intRect);
		att.setScale(calcScale(2, vm), calcScale(2, vm));
		this->attribute_vec.push_back(att);
	}

	this->abilities_texture.loadFromFile("external/assets/abilities_icons.png");
	for (short i = 0; i < 3; ++i) {
		sf::Sprite upgrade;
		upgrade.setTexture(this->abilities_texture);
		sf::IntRect intRect(i * 16, 0, 16, 16);
		upgrade.setTextureRect(intRect);
		upgrade.setScale(calcScale(4, vm), calcScale(4, vm));
		this->abilities_vec.push_back(upgrade);
	}

	this->sprites["ARMOR"] = new gui::Sprite(attribute_vec[7], calcX(728, vm), calcY(564, vm), calcScale(2, vm), false);
	this->texts["ARMOR"] = new gui::Text(&this->font, "3", calcChar(16, vm), calcX(744, vm), calcY(618, vm), sf::Color(192, 192, 192), true);
	this->sprites["REG"] = new gui::Sprite(attribute_vec[5], calcX(792, vm), calcY(564, vm), calcScale(2, vm), false);
	this->texts["REG"] = new gui::Text(&this->font, "1", calcChar(16, vm), calcX(808, vm), calcY(618, vm), sf::Color(182, 60, 53), true);
	this->sprites["ATTACK"] = new gui::Sprite(attribute_vec[1], calcX(856, vm), calcY(564, vm), calcScale(2, vm), false);
	this->texts["ATTACK"] = new gui::Text(&this->font, "3", calcChar(16, vm), calcX(872, vm), calcY(618, vm), sf::Color(192, 192, 192), true);
	this->sprites["ATTACK_SPEED"] = new gui::Sprite(attribute_vec[2], calcX(920, vm), calcY(564, vm), calcScale(2, vm), false);
	this->texts["ATTACK_SPEED"] = new gui::Text(&this->font, "3", calcChar(16, vm), calcX(936, vm), calcY(618, vm), sf::Color(192, 192, 192), true);
	this->sprites["SPEED"] = new gui::Sprite(attribute_vec[3], calcX(984, vm), calcY(564, vm), calcScale(2, vm), false);
	this->texts["SPEED"] = new gui::Text(&this->font, "4", calcChar(16, vm), calcX(1000, vm), calcY(618, vm), sf::Color(192, 192, 192), true);
	this->sprites["CRITICAL"] = new gui::Sprite(attribute_vec[4], calcX(1048, vm), calcY(564, vm), calcScale(2, vm), false);
	this->texts["CRITICAL"] = new gui::Text(&this->font, "10%", calcChar(16, vm), calcX(1064, vm), calcY(618, vm), sf::Color(192, 192, 192), true);
	
	this->sprites["ABILITY1"] = new gui::Sprite(abilities_vec[0], calcX(528, vm), calcY(526, vm), calcScale(2, vm), true);
	this->sprites["ABILITY2"] = new gui::Sprite(abilities_vec[1], calcX(528, vm), calcY(574, vm), calcScale(2, vm), true);
	this->sprites["ABILITY3"] = new gui::Sprite(abilities_vec[2], calcX(528, vm), calcY(622, vm), calcScale(2, vm), true);


	// PAGE 4

	this->difficulty_name = "";
	this->texts["CHOOSE_DIFFICULTY"] = new gui::Text(&this->font, this->lang["CHOOSE_DIFFICULTY"], calcChar(32, vm), calcX(640, vm), calcY(96, vm), sf::Color(255, 255, 255), true);

	this->sprite_buttons["SELECT_DIFFICULTY1"] = new gui::ButtonSprite("external/assets/select_difficulty.png", calcX(32, vm), calcY(184, vm), calcScale(1, vm), false);
	this->sprite_buttons["SELECT_DIFFICULTY2"] = new gui::ButtonSprite("external/assets/select_difficulty.png", calcX(448, vm), calcY(184, vm), calcScale(1, vm), false);
	this->sprite_buttons["SELECT_DIFFICULTY3"] = new gui::ButtonSprite("external/assets/select_difficulty.png", calcX(864, vm), calcY(184, vm), calcScale(1, vm), false);

	this->sprites["DIFFICULTY1"] = new gui::Sprite("external/assets/difficulty_icons.png", calcX(168, vm), calcY(272, vm), calcScale(16, vm), false);
	this->sprites["DIFFICULTY1"]->setTextureRect(sf::IntRect(0, 0, 7, 6));
	this->sprites["DIFFICULTY2"] = new gui::Sprite("external/assets/difficulty_icons.png", calcX(584, vm), calcY(272, vm), calcScale(16, vm), false);
	this->sprites["DIFFICULTY2"]->setTextureRect(sf::IntRect(7, 0, 7, 6));
	this->sprites["DIFFICULTY3"] = new gui::Sprite("external/assets/difficulty_icons.png", calcX(1000, vm), calcY(272, vm), calcScale(16, vm), false);
	this->sprites["DIFFICULTY3"]->setTextureRect(sf::IntRect(14, 0, 7, 6));

	this->texts["EASY"] = new gui::Text(&this->font, this->lang["EASY"], calcChar(32, vm), calcX(224, vm), calcY(400, vm), sf::Color(255, 255, 255), true);
	this->texts["NORMAL"] = new gui::Text(&this->font, this->lang["NORMAL"], calcChar(32, vm), calcX(640, vm), calcY(400, vm), sf::Color(255, 255, 255), true);
	this->texts["HARD"] = new gui::Text(&this->font, this->lang["HARD"], calcChar(32, vm), calcX(1056, vm), calcY(400, vm), sf::Color(255, 255, 255), true);
	this->texts["EASY_DESC"] = new gui::Text(&this->font, this->lang["MONSTERS_HAVE"] + "\n\n-25% HP\n\n-25% " + this->lang["ATTACK"], calcChar(16, vm), calcX(224, vm), calcY(460, vm), sf::Color(182, 60, 53), true);
	this->texts["NORMAL_DESC"] = new gui::Text(&this->font, this->lang["NORMAL_DESC"], calcChar(16, vm), calcX(640, vm), calcY(460, vm), sf::Color(182, 60, 53), true);
	this->texts["HARD_DESC"] = new gui::Text(&this->font, this->lang["MONSTERS_HAVE"] + "\n\n+25% HP\n\n+25% " + this->lang["ATTACK"], calcChar(16, vm), calcX(1056, vm), calcY(460, vm), sf::Color(182, 60, 53), true);

}

void MainMenuState::resetGUI()
{
	for (auto it = this->texts.begin(); it != this->texts.end(); ++it)
	{
		delete it->second;
	}
	this->texts.clear();

	for (auto it = this->text_buttons.begin(); it != this->text_buttons.end(); ++it)
	{
		delete it->second;
	}
	this->text_buttons.clear();

	for (auto it = this->sprites.begin(); it != this->sprites.end(); ++it)
	{
		delete it->second;
	}
	this->sprites.clear();

	for (auto it = this->sprite_buttons.begin(); it != this->sprite_buttons.end(); ++it)
	{
		delete it->second;
	}
	this->sprite_buttons.clear();

	initGUI();
}

void MainMenuState::update(const float& dt)
{
	if (this->music.getStatus() == sf::SoundSource::Status::Stopped)
	{
		this->music.play();
	}

	this->updateMousePositions();

	if (this->page >= 1) {
		if (!this->map.getGlobalBounds().intersects(this->mapView.getViewport())) {
			this->mapVelocity = sf::Vector2f(32.f * cos((3.1415f / 180.f)), 32.f * cos((3.1415f / 180.f)));
			this->mapRotate = Random::Float() * 360.f - 90.f;
			this->map.setPosition(256.f, 256.f);
			this->map.setRotation(this->mapRotate);
		}
		this->map.move(this->mapVelocity * dt);
	}

	if (this->fading) {
		fadingEffect(dt);
	}
	else {
		switch (this->page) {
		case 0:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				this->page = 1;
				this->dimAlpha = 1.f;
				this->fading = true;
				this->appearing = true;
				break;
			}

			this->introCooldown += dt;

			if (this->introCooldown > 1.f) {
				if (this->introCooldown > 5.f) {
					this->page = 1;
					this->dimAlpha = 1.f;
					this->fading = true;
					this->appearing = true;
					break;
				}
				else {
					if (this->introCooldown < 3.f) {
						if (this->dimAlpha < 1.f) {
							this->dimAlpha += dt;
						}
						if (this->dimAlpha > 1.f) {
							this->dimAlpha = 1.f;
						}
					}
					else if (this->introCooldown < 5.f && this->introCooldown > 3.f) {
						if (this->dimAlpha > 0.f) {
							this->dimAlpha -= dt;
						}
						if (this->dimAlpha < 0.f) {
							this->dimAlpha = 0.f;
						}

					}
					this->sprites["LOGO"]->setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(this->dimAlpha * 255.f)));
				}
			}
			break;
		case 1:
			

			if (!quitwindow) {
				this->text_buttons["PLAY"]->update(this->mousePosWindow);
				this->text_buttons["SETTINGS"]->update(this->mousePosWindow);
				this->text_buttons["QUIT"]->update(this->mousePosWindow);

				if (this->text_buttons["PLAY"]->isPressed() && !this->getMouseClick()) {
					this->setMouseClick(true);
					this->map_name = "";
					this->page = 2;
				}
				else if (this->text_buttons["SETTINGS"]->isPressed() && !this->getMouseClick()) {
					this->setMouseClick(true);
					this->states->push(new SettingsState(this->gridSize, this->window, this->gameSettings, this->supportedKeys, &this->font, this->states));
				}
				else if (this->text_buttons["QUIT"]->isPressed() && !this->getMouseClick()) {
					this->setMouseClick(true);
					this->quitwindow = true;
				}
			}
			else {
				this->text_buttons["YES"]->update(this->mousePosWindow);
				this->text_buttons["NO"]->update(this->mousePosWindow);

				if (this->text_buttons["YES"]->isPressed() && !this->getMouseClick()) {
					this->setMouseClick(true);
					this->endState();
				}
				else if (this->text_buttons["NO"]->isPressed() && !this->getMouseClick()) {
					this->setMouseClick(true);
					this->quitwindow = false;
				}
			}
			break;
		case 2:
			this->sprite_buttons["GO_BACK"]->update(this->mousePosWindow);
			if (this->sprite_buttons["GO_BACK"]->isPressed() && !this->getMouseClick()) {
				this->setMouseClick(true);
				--this->page;
				this->map_name = "";
			}

			this->sprite_buttons["SELECT_MAP1"]->update(this->mousePosWindow);
			if (this->sprite_buttons["SELECT_MAP1"]->isPressed() && !this->getMouseClick()) {
				this->setMouseClick(true);
				this->map_name = "ruins";
				this->page = 3;
				this->sprite_buttons["SELECT_MAP1"]->setTransparent();
			}
			break;
		case 3:
			this->sprite_buttons["GO_BACK"]->update(this->mousePosWindow);
			if (this->sprite_buttons["GO_BACK"]->isPressed() && !this->getMouseClick()) {
				this->setMouseClick(true);
				--this->page;
				this->hero_name = "";
				this->choosing_hero = false;
			}

			this->sprite_buttons["SELECT_HERO1"]->update(this->mousePosWindow);
			if (this->sprite_buttons["SELECT_HERO1"]->isPressed() && !this->getMouseClick()) {
				this->setMouseClick(true);
				this->choosing_hero = true;
				this->sprite_buttons["SELECT_HERO1"]->setTransparent();
				this->hero_name = "warrior";
			}

			if (this->choosing_hero) {
				this->text_buttons["CHOOSE"]->update(this->mousePosWindow);
				if (this->text_buttons["CHOOSE"]->isPressed() && !this->getMouseClick()) {
					this->page = 4;
				}
			}
			break;
		case 4:
			this->sprite_buttons["GO_BACK"]->update(this->mousePosWindow);
			if (this->sprite_buttons["GO_BACK"]->isPressed() && !this->getMouseClick()) {
				this->setMouseClick(true);
				--this->page;
				this->difficulty_name = "";
				this->choosing_hero = false;
			}

			this->sprite_buttons["SELECT_DIFFICULTY1"]->update(this->mousePosWindow);
			this->sprite_buttons["SELECT_DIFFICULTY2"]->update(this->mousePosWindow);
			this->sprite_buttons["SELECT_DIFFICULTY3"]->update(this->mousePosWindow);
			if (this->sprite_buttons["SELECT_DIFFICULTY1"]->isPressed() && !this->getMouseClick()) {
				this->setMouseClick(true);
				this->difficulty_name = "easy";
				this->page = 1;
				this->music.stop();
				this->states->push(new GameState(this->gridSize, this->window, this->gameSettings, this->supportedKeys, &this->font, this->states, this->map_name, this->hero_name, this->difficulty_name));
				this->sprite_buttons["SELECT_DIFFICULTY1"]->setTransparent();
				this->choosing_hero = false;
			}
			else if (this->sprite_buttons["SELECT_DIFFICULTY2"]->isPressed() && !this->getMouseClick()) {
				this->setMouseClick(true);
				this->difficulty_name = "normal";
				this->page = 1;
				this->music.stop();
				this->states->push(new GameState(this->gridSize, this->window, this->gameSettings, this->supportedKeys, &this->font, this->states, this->map_name, this->hero_name, this->difficulty_name));
				this->sprite_buttons["SELECT_DIFFICULTY2"]->setTransparent();
				this->choosing_hero = false;
			}
			else if (this->sprite_buttons["SELECT_DIFFICULTY3"]->isPressed() && !this->getMouseClick()) {
				this->setMouseClick(true);
				this->difficulty_name = "hard";
				this->page = 1;
				this->music.stop();
				this->states->push(new GameState(this->gridSize, this->window, this->gameSettings, this->supportedKeys, &this->font, this->states, this->map_name, this->hero_name, this->difficulty_name));
				this->sprite_buttons["SELECT_DIFFICULTY3"]->setTransparent();
				this->choosing_hero = false;
			}
			break;
		}

	}


	this->updateMouseClick();

	if (!this->quitwindow && this->page > 0) {
		this->updateKeysClick("Escape", sf::Keyboard::Escape);

		if (this->getKeysClick1("Escape") && !this->getKeysClick2("Escape")) {
			this->setKeysClick("Escape", true);
			if (this->page == 1) this->quitwindow = true;
			else --this->page;
		}
		this->setKeysClick("Escape", this->getKeysClick1("Escape"));
	}

}

void MainMenuState::draw(sf::RenderTarget* target)
{
	if (!target) target = this->window;

	

	switch (this->page) {
	case 0:
		this->sprites["LOGO"]->draw(*target);
		break;
	case 1:
		target->draw(this->map);
		target->draw(this->dimMap);

		this->sprites["TITLE"]->draw(*target);

		this->text_buttons["PLAY"]->draw(*target);
		this->text_buttons["SETTINGS"]->draw(*target);
		this->text_buttons["QUIT"]->draw(*target);

		this->texts["VERSION"]->draw(*target);
		if (this->quitwindow) {
			this->texts["ARE_YOU_SURE"]->draw(*target);

			this->text_buttons["YES"]->draw(*target);
			this->text_buttons["NO"]->draw(*target);
		}
		break;
	case 2:
		this->sprites["GO_BACK"]->draw(*target);
		this->sprite_buttons["GO_BACK"]->draw(*target);

		this->texts["CHOOSE_MAP"]->draw(*target);
		this->sprites["MAP1"]->draw(*target);
		this->sprite_buttons["SELECT_MAP1"]->draw(*target);
		
		this->texts["RUINS"]->draw(*target);
		break;
	case 3:
		this->sprites["GO_BACK"]->draw(*target);
		this->sprite_buttons["GO_BACK"]->draw(*target);

		this->texts["CHOOSE_HERO"]->draw(*target);
		this->sprites["HERO1"]->draw(*target);
		this->sprite_buttons["SELECT_HERO1"]->draw(*target);

		this->texts["WARRIOR"]->draw(*target);

		if (this->choosing_hero) {
			this->text_buttons["CHOOSE"]->draw(*target);

			this->sprites["ABILITY1"]->draw(*target);
			this->sprites["ABILITY2"]->draw(*target);
			this->sprites["ABILITY3"]->draw(*target);

			this->sprites["HERO_PREVIEW"]->draw(*target);
			this->sprites["HP_BAR"]->draw(*target);
			this->texts["HP"]->draw(*target);

			this->sprites["ARMOR"]->draw(*target);
			this->texts["ARMOR"]->draw(*target);
			this->sprites["REG"]->draw(*target);
			this->texts["REG"]->draw(*target);
			this->sprites["ATTACK"]->draw(*target);
			this->texts["ATTACK"]->draw(*target);
			this->sprites["ATTACK_SPEED"]->draw(*target);
			this->texts["ATTACK_SPEED"]->draw(*target);
			this->sprites["SPEED"]->draw(*target);
			this->texts["SPEED"]->draw(*target);
			this->sprites["CRITICAL"]->draw(*target);
			this->texts["CRITICAL"]->draw(*target);
		}
		break;
	case 4:
		this->sprites["GO_BACK"]->draw(*target);
		this->sprite_buttons["GO_BACK"]->draw(*target);

		this->texts["CHOOSE_DIFFICULTY"]->draw(*target);

		this->sprites["DIFFICULTY1"]->draw(*target);
		this->sprites["DIFFICULTY2"]->draw(*target);
		this->sprites["DIFFICULTY3"]->draw(*target);
		this->sprite_buttons["SELECT_DIFFICULTY1"]->draw(*target);
		this->sprite_buttons["SELECT_DIFFICULTY2"]->draw(*target);
		this->sprite_buttons["SELECT_DIFFICULTY3"]->draw(*target);

		this->texts["EASY"]->draw(*target);
		this->texts["NORMAL"]->draw(*target);
		this->texts["HARD"]->draw(*target);
		this->texts["EASY_DESC"]->draw(*target);
		this->texts["NORMAL_DESC"]->draw(*target);
		this->texts["HARD_DESC"]->draw(*target);
		break;
	}

	target->draw(dimBackground);
}

void MainMenuState::fadingEffect(const float& dt)
{
	if (this->appearing) {
		if (this->dimAlpha > 0.f) {
			this->dimAlpha -= dt;
			if (this->dimAlpha < 0.f) {
				this->dimAlpha = 0.f;
				this->fading = false;
			}
		}
	}
	else {
		if (this->dimAlpha < 1.f) {
			this->dimAlpha += dt;
			if (this->dimAlpha > 1.f) {
				this->dimAlpha = 1.f;
				this->fading = false;
			}
		}
	}
	this->dimBackground.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(this->dimAlpha * 255.f)));
}
