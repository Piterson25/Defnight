#include "Functions.h"
#include "GameState.h"

void GameState::initPointers()
{
	this->player = new Player;
}

GameState::GameState()
{
	initPointers();
	this->scale = 1.f;
	this->font.loadFromFile("external/font/PressStart2P-vaV7.ttf");
	this->tiles_texture.loadFromFile("external/assets/tiles.png");
	this->background_texture.loadFromFile("external/assets/background.png");
	this->goblin_texture.loadFromFile("external/assets/monsters/goblin.png");
	this->spider_texture.loadFromFile("external/assets/monsters/spider.png");
	this->orc_texture.loadFromFile("external/assets/monsters/orc.png");
	this->cyclope_texture.loadFromFile("external/assets/monsters/cyclope.png");
	this->player->texture.loadFromFile("external/assets/heroes/wojownik.png");
	this->stone_texture.loadFromFile("external/assets/stone.png");
	this->icons_texture.loadFromFile("external/assets/icons.png");
	this->shadow_texture.loadFromFile("external/assets/entity_shadow.png");
	this->shuriken_texture.loadFromFile("external/assets/shuriken.png");
	this->upgrade_texture.loadFromFile("external/assets/select_upgrade.png");
	this->upgrade_icons.loadFromFile("external/assets/upgrade_icons.png");
	this->ability_icon_texture.loadFromFile("external/assets/abilities_icons.png");
	this->ability_texture.loadFromFile("external/assets/abilities.png");
	this->drop_texture.loadFromFile("external/assets/drop.png");
}

GameState::~GameState()
{
	delete this->player;
}

void GameState::init(float& scale, std::vector<sf::Keyboard::Key>& klawisze, const std::string& mapName, const std::string& playerName, const std::string& difficulty)
{
	this->scale = scale;
	this->klawisze = klawisze;
	this->paused = false;
	this->escPause = false;
	this->escCounter = 0;

	this->numberOfMonsters = 0;
	this->wave = 0;
	this->sila = 10 + int(Random::Float() * 10.f);
	this->monsterType();
	this->titleCountdown = 0.f;
	this->option1_up = 0;
	this->option2_up = 0;

	this->mycha = false;

	this->temp_hp_bar = 0;
	this->temp_reg_bar = 0;
	this->temp_lvl_bar = 0;
	this->leveling = false;
	this->upgrading = false;

	this->regen = false;
	this->levelup = false;

	this->ability_icon_cooldown = 0.f;

	Random::Init();

	if (difficulty == "Latwy") this->difficultyModifier = 0.75f;
	else if (difficulty == "Normalny") this->difficultyModifier = 1.f;
	else if (difficulty == "Trudny") this->difficultyModifier = 1.25f;

	for (short i = 0; i < 7; ++i) {
		sf::Sprite icon;
		icon.setTexture(this->icons_texture);
		sf::IntRect intRect(i * 16, 0, 16, 16);
		icon.setTextureRect(intRect);
		icon.setScale(2 * this->scale, 2 * this->scale);
		this->icons.push_back(icon);
	}
	this->icons[0].setPosition(sf::Vector2f(0.f, 86.f * this->scale));
	this->icons[1].setPosition(sf::Vector2f(812.f * this->scale, 16.f * this->scale));
	this->icons[2].setPosition(sf::Vector2f(876.f * this->scale, 16.f * this->scale));
	this->icons[3].setPosition(sf::Vector2f(940.f * this->scale, 16.f * this->scale));
	this->icons[4].setPosition(sf::Vector2f(1004.f * this->scale, 16.f * this->scale));
	this->icons[5].setPosition(sf::Vector2f(436.f * this->scale, 16.f * this->scale));
	

	this->waveCountdown = 0.f;
	

	// Widok na mape
	this->view.setSize(1280 * this->scale, 720 * this->scale);
	
	// Mapa
	createSprite(this->background, this->background_texture, 4 * this->scale);
	std::ifstream mapa("external/maps/first.txt");
	std::string temp;
	sf::Sprite obstacle;
	createSprite(obstacle, this->tiles_texture, 4 * this->scale);
	obstacle.setTextureRect(sf::IntRect(48, 16, 16, 16));
	float x = 0.f, y = 0.f;
	if (mapa.is_open())
	{
		while (std::getline(mapa, temp)) {
			for (size_t i = 0; i < temp.size(); ++i) {
				if (temp[i] == '#') {
					obstacle.setPosition(x, y);
					this->obstacles.push_back(obstacle);
				}
				else if (temp[i] == 'S') {
					this->player->sprite.setPosition(x, y);
				}
				x += 64 * this->scale;
			}
			x = 0.f;
			y += 64 * this->scale;
		}
	}
	mapa.close();

	// Potwory
	this->monster.scale = this->scale;
	this->monster.shadow.setTexture(this->shadow_texture);
	this->monster.shadow.setScale(4 * this->scale, 4 * this->scale);
	

	// Gracz
	
	this->player->texture.loadFromFile("external/assets/heroes/wojownik.png");
	this->player->init(scale, playerName);
	this->player->shadow.setTexture(this->shadow_texture);
	this->player->shadow.setScale(4 * this->scale, 4 * this->scale);
	this->player->shadow.setPosition(this->player->sprite.getPosition().x, this->player->sprite.getPosition().y + 52 * this->scale);

	// Widok HUD
	this->viewHUD.setSize(1280 * this->scale, 720 * this->scale);
	this->viewHUD.setCenter(this->viewHUD.getSize().x / 2, this->viewHUD.getSize().y / 2);
	createSprite(this->HUD, this->HUD_texture, "external/assets/bar.png", this->scale);

	createSprite(this->side_bar, this->side_bar_texture, "external/assets/side_bar.png", this->scale);
	this->side_bar.setPosition(988 * this->scale, 128 * this->scale);

	createSprite(this->upgrade1_button, this->upgrade_texture, this->scale);
	this->upgrade1_button.setPosition(1008 * this->scale, 200 * this->scale);
	createSprite(this->upgrade2_button, this->upgrade_texture, this->scale);
	this->upgrade2_button.setPosition(1008 * this->scale, 360 * this->scale);
	createSprite(this->upgrade3_button, this->upgrade_texture, this->scale);
	this->upgrade3_button.setPosition(1008 * this->scale, 520 * this->scale);

	createSprite(this->upgrade1_icon, this->upgrade_icons, 4 * this->scale);
	this->upgrade1_icon.setTextureRect(sf::IntRect(16, 0, 16, 16));
	this->upgrade1_icon.setPosition(1020 * this->scale, 212 * this->scale);
	createText(this->upgrade1_text, this->font, 16 * this->scale, "Ninja", 1120 * this->scale, 228 * this->scale, 255, 255, 255);
	this->upgrade1_miniIcon = this->icons[3];
	this->upgrade1_miniIcon.setPosition(1220 * this->scale, 228 * this->scale);

	createSprite(this->upgrade2_icon, this->upgrade_icons, 4 * this->scale);
	this->upgrade2_icon.setTextureRect(sf::IntRect(32, 0, 16, 16));
	this->upgrade2_icon.setPosition(1020 * this->scale, 372 * this->scale);
	createText(this->upgrade2_text, this->font, 16 * this->scale, "Rycerz", 1120 * this->scale, 388 * this->scale, 255, 255, 255);
	this->upgrade2_miniIcon = this->icons[1];
	this->upgrade2_miniIcon.setPosition(1220 * this->scale, 388 * this->scale);

	createSprite(this->upgrade3_icon, this->upgrade_icons, 4 * this->scale);
	this->upgrade3_icon.setTextureRect(sf::IntRect(48, 0, 16, 16));
	this->upgrade3_icon.setPosition(1020 * this->scale, 532 * this->scale);
	createText(this->upgrade3_text, this->font, 16 * this->scale, "Skaut", 1120 * this->scale, 548 * this->scale, 255, 255, 255);
	this->upgrade3_miniIcon = this->icons[5];
	this->upgrade3_miniIcon.setPosition(1220 * this->scale, 548 * this->scale);


	createSprite(this->upgrade_bar, this->upgrade_bar_texture, "external/assets/upgrade_bar.png", this->scale);
	this->upgrade_bar.setPosition(400 * this->scale, 476 * this->scale);

	// Miniaturka postaci
	this->photo.setTexture(this->player->texture);
	this->photo.setTextureRect(sf::IntRect(0, 32, 16, 16));
	this->photo.setScale(4 * this->scale, 4 * this->scale);
	this->photo.setPosition(164 * this->scale, 16 * this->scale);

	// Nazwa postaci
	createText(this->playerName, this->font, 16 * this->scale, this->player->name, 0, 0, 255, 255, 255);
	this->playerName.setPosition(float(int(164 * this->scale + 32 * this->scale - (this->playerName.getGlobalBounds().width / 2))), 96 * this->scale);


	// Licznik potworow
	createText(this->monsterCount, this->font, 16 * this->scale, "", 906 * this->scale, 96 * this->scale, 130, 139, 152);
	this->monsterCount.setString(L"Pozosta³ych potworów:");
	this->monsterCount.setString(this->monsterCount.getString() + std::to_string(this->potwory.size()));

	// Numer fali
	createText(this->waveNumber, this->font, 32 * this->scale, "", 1060 * this->scale, 4 * this->scale, 228, 92, 95);

	// Odliczanie fali
	createText(this->nextWave, this->font, 16 * this->scale, "", 954 * this->scale, 96 * this->scale, 255, 246, 76);

	createText(this->bigWave, this->font, 64 * this->scale, "FALA " + std::to_string(this->wave + 1), 0 * this->scale, 256 * this->scale, 255, 255, 255);
	center(this->bigWave, 640 * this->scale);
	
	createText(this->killMobs, this->font, 32 * this->scale, "", 0 * this->scale, 512 * this->scale, 192, 192, 192);
	this->killMobs.setString(std::to_string(this->idiki.size()));

	if (const size_t rozmiar = this->idiki.size(); rozmiar > 10) {
		if (rozmiar % 10 >= 2 && rozmiar % 10 <= 4)
			this->killMobs.setString(this->killMobs.getString() + L" potwory");
		else
			this->killMobs.setString(this->killMobs.getString() + L" potworów");
	}
	else {
		if (rozmiar == 1) 
			this->killMobs.setString(this->killMobs.getString() + L" potwór");
		else if (rozmiar < 5)
			this->killMobs.setString(this->killMobs.getString() + L" potwory");
		else
			this->killMobs.setString(this->killMobs.getString() + L" potworów");
	}
	this->killMobs.setPosition(float(unsigned(640 * this->scale - this->killMobs.getGlobalBounds().width / 2)), this->killMobs.getPosition().y);

	if (this->idiki.size() < 5) this->killMobs.setString(std::to_string(this->idiki.size()) + " potwory");
	else {
		this->killMobs.setString(std::to_string(this->idiki.size()));
		this->killMobs.setString(this->killMobs.getString() + L" potworów");
	}
	this->killMobs.setPosition(float(unsigned(640 * this->scale - this->killMobs.getGlobalBounds().width / 2)), this->killMobs.getPosition().y);

	// Statystyki gracza
	createText(this->attack, this->font, 16 * this->scale, std::to_string(this->player->attack), 828 * this->scale, 70 * this->scale, 192, 192, 192);
	center(this->attack, 828);
	createText(this->attackSpeed, this->font, 16 * this->scale, std::to_string(this->player->attackSpeed), 892 * this->scale, 70 * this->scale, 192, 192, 192);
	center(this->attackSpeed, 892);
	createText(this->speed, this->font, 16 * this->scale, std::to_string(this->player->speed), 956 * this->scale, 70 * this->scale, 192, 192, 192);
	center(this->speed, 956);
	createText(this->critical, this->font, 16 * this->scale, std::to_string(this->player->critical) + "%", 1020 * this->scale, 70 * this->scale, 192, 192, 192);
	center(this->critical, 1020);

	// Licznik zlota
	createText(this->gold, this->font, 16 * this->scale, "", 36 * this->scale, 96 * this->scale, 255, 246, 76);
	this->gold.setString(std::to_string(this->player->gold));

	// Paski

	// XP i Level gracza
	createText(this->level, this->font, 16 * this->scale, "Level " + std::to_string(this->player->Level), 366 * this->scale, 16 * this->scale, 255, 255, 255);
	center(this->level, 640 * this->scale);

	createSprite(this->xp_bar, this->bars, "external/assets/bars.png", this->scale);
	this->xp_bar.setPosition(508 * this->scale, 12 * this->scale);
	this->xp_bar.setTextureRect(sf::IntRect(0, 0, 0, 0));
	
	createText(this->xp, this->font, 16 * this->scale, "XP:" + std::to_string(this->player->xp) + "/" + std::to_string(this->player->maxxp), 640 * this->scale, 16 * this->scale, 255, 255, 255);
	center(this->xp, 640 * this->scale);

	createText(this->levelUp_text, this->font, 32 * this->scale, "LEVEL UP!", 0, 256 * this->scale, 255, 246, 76);
	center(this->levelUp_text, 640 * this->scale);

	// Zdrowie i regeneracja gracza
	createText(this->Reg, this->font, 16 * this->scale, std::to_string(this->player->Reg), 452 * this->scale, 70 * this->scale, 182, 60, 53);
	center(this->Reg, 452 * this->scale);

	createSprite(this->hp_bar, this->bars, "external/assets/bars.png", this->scale);
	this->hp_bar.setPosition(508 * this->scale, 66 * this->scale);
	this->hp_bar.setTextureRect(sf::IntRect(0, 22, 264, 22));

	createText(this->hp, this->font, 16 * this->scale, "HP:" + std::to_string(this->player->hp) + "/" + std::to_string(this->player->maxhp), 640 * this->scale, 70 * this->scale, 255, 255, 255);
	center(this->hp, 640 * this->scale);
	

	// Lvl
	createSprite(this->option1_button, this->option_texture, "external/assets/select_levelup.png", this->scale);
	this->option1_button.setPosition(504 * this->scale, 500 * this->scale);
	createSprite(this->option2_button, this->option_texture, "external/assets/select_levelup.png", this->scale);
	this->option2_button.setPosition(688 * this->scale, 500 * this->scale);

	this->option1.setScale(4 * this->scale, 4 * this->scale);
	this->option1.setPosition(516 * this->scale, 512 * this->scale);
	this->option2.setScale(4 * this->scale, 4 * this->scale);
	this->option2.setPosition(700 * this->scale, 512 * this->scale);

	createText(this->option1_text, this->font, 16 * this->scale, "", 548 * this->scale, 608 * this->scale, 255, 255, 255);
	createText(this->option2_text, this->font, 16 * this->scale, "", 732 * this->scale, 608 * this->scale, 255, 255, 255);

	// Abilitki
	createRectangle(this->ability_icon_shadow, 64 * this->scale, 0 * this->scale, 288 * this->scale, 16 * this->scale, 128, 128, 128);
	this->ability_icon_shadow.setFillColor(sf::Color(128, 128, 128, 128));

	createSprite(this->ability_icon, this->ability_icon_texture, 4 * this->scale);
	this->ability_icon.setPosition(288 * this->scale, 16 * this->scale);
	this->ability_icon.setTextureRect(sf::IntRect(0, 0, 16, 16));

	createSprite(this->ability, this->ability_texture, 4 * this->scale);
	this->ability.setTextureRect(sf::IntRect(0, 0, 16, 16));
	this->ability.setColor(sf::Color(255, 255, 255, 128));

	// Smierc
	createRectangle(this->dieScreen, 1280 * this->scale, 592 * this->scale, 0, 128 * this->scale, 182, 60, 53);
	this->dieScreen.setFillColor(sf::Color(182, 60, 53, 192));

	createRectangle(this->escScreen, 1280 * this->scale, 592 * this->scale, 0, 128 * this->scale, 0, 0, 0);
	this->escScreen.setFillColor(sf::Color(0, 0, 0, 192));

	createText(this->dieText, this->font, 72 * this->scale, "Zginales", 400 * this->scale, 224 * this->scale, 255, 255, 255);
	this->dieText.setString(L"Zgina³eœ!");
	center(this->dieText, 640 * this->scale);
	
	createText(this->menu, this->font, 32 * this->scale, "Menu G³owne", 400 * this->scale, 392 * this->scale, 255, 255, 255);
	this->menu.setString(L"Menu G³ówne");
	center(this->menu, 640 * this->scale);

	createText(this->quit, this->font, 32 * this->scale, "Wyjdz", 400 * this->scale, 516 * this->scale, 255, 255, 255);
	this->quit.setString(L"WyjdŸ z gry");
	center(this->quit, 640 * this->scale);

	//this->player->Level = 5;
	//this->player->name = "Skaut";
	//this->player->abilityCooldown = 10.f;
	//this->player->abilityDuration = 5.f;
	
}

void GameState::monsterType()
{
	this->idiki.clear();
	unsigned hpsy = this->sila;
	short t = 0;
	while (hpsy > 0) {
		if (hpsy >= 18 && this->wave >= 6) {
			t = int(Random::Float() * 4.f);
		}
		else if (hpsy >= 12 && this->wave >= 4) {
			t = int(Random::Float() * 3.f);
		}
		else if (hpsy >= 7 && this->wave >= 2) {
			t = int(Random::Float() * 2.f);
		}
		else if (hpsy >= 3) {
			t = 0;
		}
		else break;

		this->idiki.push_back(t);

		if (t == 0) hpsy -= 3;
		else if (t == 1) hpsy -= 7;
		else if (t == 2) hpsy -= 12;
		else if (t == 3) hpsy -= 18;
	}
}

void GameState::addMonsters()
{
	for (const auto& id : this->idiki) {
		//Nazwa, atak, szybkosc ataku, hp, szybkosc, gold, xp
		switch (id)
		{
		case 0:
			this->monster.init(this->scale, "Goblin", int(1 * this->difficultyModifier), 2, int(3 * this->difficultyModifier), 2, 1, 5, this->player->sprite.getPosition().x, this->player->sprite.getPosition().y);
			this->monster.texture = this->goblin_texture;
			this->monster.sprite.setTexture(this->goblin_texture);
			break;

		case 1:
			this->monster.init(this->scale, "Pajak", int(2 * this->difficultyModifier), 3, int(7 * this->difficultyModifier), 3, 2, 10, this->player->sprite.getPosition().x, this->player->sprite.getPosition().y);
			this->monster.texture = this->spider_texture;
			this->monster.sprite.setTexture(this->spider_texture);
			break;

		case 2:
			this->monster.init(this->scale, "Ork", int(3 * this->difficultyModifier), 2, int(12 * this->difficultyModifier), 2, 3, 17, this->player->sprite.getPosition().x, this->player->sprite.getPosition().y);
			this->monster.texture = this->orc_texture;
			this->monster.sprite.setTexture(this->orc_texture);
			break;

		case 3:
			this->monster.init(this->scale, "Cyklop", int(4 * this->difficultyModifier), 1, int(18 * this->difficultyModifier), 1, 4, 26, this->player->sprite.getPosition().x, this->player->sprite.getPosition().y);
			this->monster.texture = this->cyclope_texture;
			this->monster.sprite.setTexture(this->cyclope_texture);
			break;
		default:
			break;
		}
		this->monster.sprite.setScale(4 * this->scale, 4 * this->scale);
		this->monster.sprite.setTextureRect(sf::IntRect(0, 32, 16, 16));
		unsigned rx = unsigned(Random::Float() * 32.f), ry = unsigned(Random::Float() * 32.f);
		for (int i = 0; i < this->obstacles.size(); ++i) {
			while ((this->obstacles[i].getPosition().x == 64 * rx * this->scale && this->obstacles[i].getPosition().y == 64 * ry * this->scale) ||
				vectorDistance(64 * rx * this->scale, 64 * ry * this->scale, this->player->sprite.getPosition().x, this->player->sprite.getPosition().y) <= 192 * this->scale)
			{
				rx = unsigned(Random::Float() * 32.f);
				ry = unsigned(Random::Float() * 32.f);
				i = 0;

			}
			for (const auto& e : this->potwory) {
				if (e.sprite.getPosition().x == 64 * rx * this->scale && e.sprite.getPosition().y == 64 * ry * this->scale) {
					rx = unsigned(Random::Float() * 32.f);
					ry = unsigned(Random::Float() * 32.f);
					i = 0;
				}
			}

		}
		this->monster.sprite.setPosition(64 * rx * this->scale, 64 * ry * this->scale);
		this->monster.shadow.setPosition(this->monster.sprite.getPosition().x, this->monster.sprite.getPosition().y + 52 * this->scale);
		this->monster.sprite.setColor(sf::Color(255, 255, 255, 0));
		this->monster.shadow.setColor(sf::Color(255, 255, 255, 0));
		this->potwory.push_back(this->monster);
	}
	this->numberOfMonsters = int(this->idiki.size());
	
}

void GameState::draw(sf::RenderWindow& window)
{
	window.setView(this->view);
	window.draw(this->background);

	for (const auto& e : this->drops) {
		window.draw(e.sprite);
	}
	for (const auto& mob : this->potwory) {
		window.draw(mob.shadow);
	}
	window.draw(this->player->shadow);

	for (const auto& e : this->obstacles) {
		window.draw(e);
	}
	for (const auto& e : this->pociski) {
		window.draw(e.sprite);
	}
	for (const auto& e : this->potwory) {
		window.draw(e.sprite);
	}

	
	window.draw(this->player->sprite);

	if ((this->player->name == "Rycerz" || this->player->name == "Skaut") && this->player->abilityActive) {
		window.draw(this->ability);
	}

	for (const auto& t : this->damage_texts) {
		window.draw(t.text);
	}
	

	window.setView(this->viewHUD);


	
	window.draw(this->HUD);
	window.draw(this->xp_bar);
	window.draw(this->hp_bar);
	window.draw(this->photo);
	window.draw(this->playerName);
	if (this->waveCountdown > 8.f && this->waveCountdown < 10.f) {
		window.draw(this->bigWave);
		window.draw(this->killMobs);
	}
	else if (this->waveCountdown > 10.f) window.draw(this->waveNumber);
	if (this->potwory.size() == 0) window.draw(this->nextWave);
	else window.draw(this->monsterCount);
	window.draw(this->icons[0]);
	window.draw(this->icons[1]);
	window.draw(this->icons[2]);
	window.draw(this->icons[3]);
	window.draw(this->icons[4]);
	window.draw(this->icons[5]);
	window.draw(this->attack);
	window.draw(this->attackSpeed);
	window.draw(this->speed);
	window.draw(this->critical);
	window.draw(this->gold);
	


	if (sf::Mouse::getPosition(window).y >= 12 * this->scale && sf::Mouse::getPosition(window).x >= 508 * this->scale &&
		sf::Mouse::getPosition(window).y <= 34 * this->scale && sf::Mouse::getPosition(window).x <= 772 * this->scale) {
		window.draw(this->xp);
	}
	else window.draw(this->level);
	
	window.draw(this->Reg);
	window.draw(this->hp);

	if (this->leveling) {
		window.draw(this->upgrade_bar);
		window.draw(this->levelUp_text);
		window.draw(this->option1);
		window.draw(this->option1_button);
		window.draw(this->option1_text);
		window.draw(this->option2);
		window.draw(this->option2_button);
		window.draw(this->option2_text);
	}
	if (this->upgrading) {
		window.draw(side_bar);
		window.draw(upgrade1_button);
		window.draw(upgrade1_icon);
		window.draw(upgrade1_text);
		window.draw(upgrade1_miniIcon);
		window.draw(upgrade2_button);
		window.draw(upgrade2_icon);
		window.draw(upgrade2_text);
		window.draw(upgrade2_miniIcon);
		window.draw(upgrade3_button);
		window.draw(upgrade3_icon);
		window.draw(upgrade3_text);
		window.draw(upgrade3_miniIcon);
	}
	if (this->player->Level >= 5) {
		if (this->player->name != "Wojownik") {
			window.draw(this->ability_icon);
			window.draw(this->ability_icon_shadow);
			
		}
		
	}
	
	if (this->paused) {
		if (this->player->dead) {
			window.draw(this->dieScreen);
			window.draw(this->dieText);
			window.draw(this->menu);
			window.draw(this->quit);
		}
		else if (this->escPause) {
			window.draw(this->escScreen);
		}
		
	}
}

void GameState::update(sf::RenderWindow& window, const float& dt, unsigned& state)
{
	/*if (this->escCounter == 0) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) escCounter += dt;
	}
	else escCounter += dt;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		this->escPause = !this->escPause;
		this->paused = !this->paused;
	}

	if (escCounter >= 1.0f) {
		this->escPause = !this->escPause;
		this->paused = !this->paused;
		escCounter = 0.f;
	}*/

	if (this->paused) {
		if (this->player->dead) this->deathScreenUpdate(window, state);
		//else if (this->escPause) this->deathScreenUpdate(window, state);
		else if (!this->escPause) {
			if (this->xp_bar.getGlobalBounds().width < this->temp_lvl_bar && this->levelup) {
				this->xp_bar.setTextureRect(sf::IntRect(0, 0, int(this->xp_bar.getGlobalBounds().width / this->scale + 1000.f * dt), 22));
			}
			else this->levelup = false;

			for (auto& t : this->damage_texts) {
				t.move_cd += dt;
				t.move(dt);
			}

			for (auto t = this->damage_texts.begin(); t != this->damage_texts.end();) {
				if (t->move_cd >= 1.f) {
					t = this->damage_texts.erase(t);
				}
				else ++t;
			}

			buttonSprite(window, this->option1_button, "option1");
			buttonSprite(window, this->option2_button, "option2");
			
			if (this->upgrading) {
				buttonSprite(window, this->upgrade1_button, "ninja");
				buttonSprite(window, this->upgrade2_button, "knight");
				buttonSprite(window, this->upgrade3_button, "scout");
			}
				
		}
	}
	else {
		this->gameUpdate(window, dt, state);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) == false) this->mycha = false;
	else this->mycha = true;
}

void GameState::gameUpdate(sf::RenderWindow& window, const float& dt, unsigned& state)
{
	this->player->controls(this->klawisze, dt);
	this->player->animation(dt);
	this->player->collision(this->obstacles);
	this->player->move();

	cameraMove(this->player->sprite, this->scale, this->background, this->view);

	if (this->player->regeneration(dt)) {
		this->hp.setString("HP:" + std::to_string(this->player->hp) + "/" + std::to_string(this->player->maxhp));
		center(this->hp, 640 * this->scale);
		this->temp_reg_bar = int(float(this->player->hp) / float(this->player->maxhp) * 264 * this->scale);
		this->regen = true;
	}

	this->player->attackLoad(dt);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Mouse::getPosition(window).y > 128 * this->scale && this->player->attackCooldown >= 1.f && this->player->attackAni == false) {
		this->player->attackAni = true;
		this->player->step = 0;
		this->player->attackCooldown = 0.f;
	}
	if (this->player->Level >= 5) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && sf::Mouse::getPosition(window).y > 128 * this->scale && this->ability_icon_shadow.getSize().y == 0) {
			this->player->abilityActive = true;
			this->player->abilityCounter = 0.f;
			this->ability_icon_shadow.setSize(sf::Vector2f(64 * this->scale, 64 * this->scale));
			this->ability_icon_shadow.setPosition(sf::Vector2f(288 * this->scale, 16 * this->scale));
			this->ability_icon_cooldown = 0.f;
		}

		if (this->ability_icon_shadow.getSize().y > 0) {
			
			this->player->abilityCounter += dt;
			
			if (this->player->abilityActive) {
				if (this->player->name == "Ninja") {
					Projectile shuriken;
					shuriken.init(this->scale, "Shuriken", 2, 4, 3);
					shuriken.texture = this->shuriken_texture;
					shuriken.sprite.setTexture(this->shuriken_texture);
					shuriken.sprite.setTextureRect(sf::IntRect(0, 0, 4, 4));
					shuriken.sprite.setScale(4 * this->scale, 4 * this->scale);

					shuriken.sprite.setPosition(this->player->sprite.getPosition().x + 32 * this->scale, this->player->sprite.getPosition().y + 32 * this->scale);
					shuriken.calculateVelocity(sf::Vector2f(window.mapPixelToCoords(sf::Mouse::getPosition(window), view)), dt);
					shuriken.sprite.setOrigin(2 * this->scale, 2 * this->scale);
					pociski.push_back(shuriken);
					
					this->player->abilityActive = false;
				}
				else if (this->player->name == "Rycerz") {
					this->ability.setTextureRect(sf::IntRect(0, 0, 16, 16));
					this->ability.setPosition(this->player->sprite.getPosition());
					if (this->player->abilityCounter >= this->player->abilityDuration) {
						this->player->abilityActive = false;
					}
				}
				else if (this->player->name == "Skaut") {
					this->ability.setTextureRect(sf::IntRect(16, 0, 16, 16));
					this->ability.setPosition(this->player->sprite.getPosition().x, this->player->sprite.getPosition().y + 4.f * this->scale);
					if (this->player->abilityCounter >= this->player->abilityDuration) {
						this->player->abilityActive = false;
					}
				}
			}

			this->ability_icon_cooldown += dt / this->player->abilityCooldown;
			if (this->ability_icon_cooldown >= 0.015625) {
				this->ability_icon_cooldown = 0.f;
				this->ability_icon_shadow.setSize(sf::Vector2f(64 * this->scale, this->ability_icon_shadow.getSize().y - 1.f));
				this->ability_icon_shadow.setPosition(this->ability_icon_shadow.getPosition().x, this->ability_icon_shadow.getPosition().y + 1.f);
			}

			
		}
		
	}
	
	

	if (this->potwory.size() > 0) {

		for (auto& mob : this->potwory) {
			float distance = 0.f;
			if (this->player->left)
				distance = vectorDistance(mob.getCenterPosition().x, mob.getCenterPosition().y, (this->player->sprite.getPosition().x + 8 * this->scale), (this->player->sprite.getPosition().y + 32 * this->scale));
			else if (this->player->right) 
				distance = vectorDistance(mob.getCenterPosition().x, mob.getCenterPosition().y, (this->player->sprite.getPosition().x + 56 * this->scale), (this->player->sprite.getPosition().y + 32 * this->scale));
			else if (this->player->up) 
				distance = vectorDistance(mob.getCenterPosition().x, mob.getCenterPosition().y, (this->player->sprite.getPosition().x + 32 * this->scale), (this->player->sprite.getPosition().y + 8 * this->scale));
			else if (this->player->down)
				distance = vectorDistance(mob.getCenterPosition().x, mob.getCenterPosition().y, (this->player->sprite.getPosition().x + 32 * this->scale), (this->player->sprite.getPosition().y + 56 * this->scale));

			if (distance <= this->player->reach * 32 * this->scale) {
				if (!mob.dead && !mob.punched && this->player->attackAni && this->player->step == 80) {
					if (int(Random::Float() * 100.f) + 1 <= this->player->critical) {
						mob.hp -= this->player->attack * 2;
						damage_t.init(this->scale, "-" + std::to_string(this->player->attack * 2), sf::Vector2f(mob.sprite.getPosition().x + 32 * this->scale, mob.sprite.getPosition().y + 32 * this->scale), this->font, 233, 134, 39);
					}
					else {
						mob.hp -= this->player->attack;
						damage_t.init(this->scale, "-" + std::to_string(this->player->attack), sf::Vector2f(mob.sprite.getPosition().x + 32 * this->scale, mob.sprite.getPosition().y + 32 * this->scale), this->font, 255, 255, 255);
					}

					damage_texts.push_back(damage_t);
					mob.punched = true;
					mob.punchedTime = 0.f;
					mob.step = 0;
					mob.attackAni = false;
					if (mob.hp <= 0) {
						mob.dead = true;
						mob.sprite.setColor(sf::Color(182, 60, 53));
					}
				}
				break;
			}
		}

		for (auto monster = this->potwory.begin(); monster != this->potwory.end();) {
			if (monster->dead) {
				monster->deadCountdown -= dt * 1000.f;
				if (monster->deadCountdown < 0.f) monster->deadCountdown = 0.f;
				monster->sprite.setColor(sf::Color(182, 60, 53, sf::Uint8(monster->deadCountdown)));
				monster->shadow.setColor(sf::Color(255, 255, 255, sf::Uint8(monster->deadCountdown)));


				if (monster->deadCountdown == 0.f) {
					
					Drop dropek;
					dropek.init(this->scale, sf::Vector2f(monster->sprite.getPosition().x + 16 * this->scale, monster->sprite.getPosition().y + 16 * this->scale), "coin", monster->gold);
					dropek.sprite.setTexture(this->drop_texture);
					dropek.sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
					this->drops.push_back(dropek);

					if (int(Random::Float() * 4) == 0) {
						Drop dropek2;
						dropek2.init(this->scale, sf::Vector2f(monster->sprite.getPosition().x + 16 * this->scale, monster->sprite.getPosition().y), "heart", 1);
						dropek2.sprite.setTexture(this->drop_texture);
						dropek2.sprite.setTextureRect(sf::IntRect(0, 16, 16, 16));
						this->drops.push_back(dropek2);
					}

					this->player->xp += monster->xp;


					while (this->player->xp >= this->player->maxxp) {
						this->player->Level++;
						this->player->xp = this->player->xp - this->player->maxxp;
						this->player->maxxp += (int(Random::Float() * this->player->maxxp) + 1);


						std::vector<short> id = { 1, 2, 3, 4, 5, 6 };
						if (const size_t t = unsigned(Random::Float() * id.size()); id[t] == 1) {
							id.erase(id.begin() + t);
							this->option1_id = 1;
							this->option1_up = 1;
							this->option1_text.setString("Atak\n+" + std::to_string(this->option1_up));
						}
						else if (id[t] == 2) {
							id.erase(id.begin() + t);
							this->option1_id = 2;
							this->option1_up = 1;
							this->option1_text.setString(L"Szyb.ataku\n+");
							this->option1_text.setString(this->option1_text.getString() + std::to_string(this->option1_up));
						}
						else if (id[t] == 3) {
							id.erase(id.begin() + t);
							this->option1_id = 3;
							this->option1_up = 1;
							this->option1_text.setString(L"Szybkoœæ\n+");
							this->option1_text.setString(this->option1_text.getString() + std::to_string(this->option1_up));
						}
						else if (id[t] == 4) {
							id.erase(id.begin() + t);
							this->option1_id = 4;
							this->option1_up = 10;
							this->option1_text.setString("Atak kryt.\n+" + std::to_string(this->option1_up) + "%");
						}
						else if (id[t] == 5) {
							id.erase(id.begin() + t);
							this->option1_id = 5;
							this->option1_up = 1;
							this->option1_text.setString("Reg\n+" + std::to_string(this->option1_up));
						}
						else if (id[t] == 6) {
							id.erase(id.begin() + t);
							this->option1_id = 6;
							this->option1_up = unsigned(Random::Float() * 2.f) + 1;
							this->option1_text.setString("Max HP\n+" + std::to_string(this->option1_up));
						}
						

						this->option1.setTexture(this->icons_texture);
						this->option1.setTextureRect(sf::IntRect(this->option1_id * 16, 0, 16, 16));
						this->option1.setScale(4 * this->scale, 4 * this->scale);
						center(this->option1_text, 548 * this->scale);



						if (const size_t t = unsigned(Random::Float() * id.size()); id[t] == 1) {
							id.erase(id.begin() + t);
							this->option2_id = 1;
							this->option2_up = 1;
							this->option2_text.setString("Atak\n+" + std::to_string(this->option2_up));
						}
						else if (id[t] == 2) {
							id.erase(id.begin() + t);
							this->option2_id = 2;
							this->option2_up = 1;
							this->option2_text.setString(L"Szyb.ataku\n+");
							this->option2_text.setString(this->option2_text.getString() + std::to_string(this->option2_up));
						}
						else if (id[t] == 3) {
							id.erase(id.begin() + t);
							this->option2_id = 3;
							this->option2_up = 1;
							this->option2_text.setString(L"Szybkoœæ\n+");
							this->option2_text.setString(this->option2_text.getString() + std::to_string(this->option2_up));
						}
						else if (id[t] == 4) {
							id.erase(id.begin() + t);
							this->option2_id = 4;
							this->option2_up = 10;
							this->option2_text.setString("Atak kryt.\n+" + std::to_string(this->option2_up) + "%");
						}
						else if (id[t] == 5) {
							id.erase(id.begin() + t);
							this->option2_id = 5;
							this->option2_up = 1;
							this->option2_text.setString("Reg\n+" + std::to_string(this->option2_up));
						}
						else if (id[t] == 6) {
							id.erase(id.begin() + t);
							this->option2_id = 6;
							this->option2_up = unsigned(Random::Float() * 2.f) + 1;
							this->option2_text.setString("Max HP\n+" + std::to_string(this->option2_up));
						}
						
						this->option2.setTexture(this->icons_texture);
						this->option2.setTextureRect(sf::IntRect(this->option2_id * 16, 0, 16, 16));
						this->option2.setScale(4 * this->scale, 4 * this->scale);
						center(this->option2_text, 732 * this->scale);

						
						this->xp_bar.setTextureRect(sf::IntRect(0, 0, 0, 22));
						this->leveling = true;
						this->paused = true;
						if (this->player->Level % 5 == 0) {
							this->upgrading = true;
						}
					}
					this->levelup = true;
					
					this->temp_lvl_bar = int(float(this->player->xp) / float(this->player->maxxp) * 264 * this->scale);
					this->level.setString("Level " + std::to_string(this->player->Level));
					center(this->level, 640 * this->scale);

					monster = this->potwory.erase(monster);

					this->xp.setString("XP:" + std::to_string(this->player->xp) + "/" + std::to_string(this->player->maxxp));
					this->xp.setPosition(float(int(640 * this->scale - (this->xp.getLocalBounds().width) / 2)), this->xp.getPosition().y);
					this->monsterCount.setString(L"Pozosta³ych potworów:");
					this->monsterCount.setString(this->monsterCount.getString() + std::to_string(this->potwory.size()));
				}
				else ++monster;
			}
			else ++monster;
		}

		if (this->potwory.size() == 0) {
			this->waveCountdown = 0.f;
			this->sila += unsigned(Random::Float() * this->sila) + 1;
			this->monsterType();
			this->bigWave.setString("FALA " + std::to_string(this->wave + 1));
			this->bigWave.setPosition(float(unsigned(640 * this->scale - this->bigWave.getGlobalBounds().width / 2)), this->bigWave.getPosition().y);
			this->killMobs.setString(std::to_string(this->idiki.size()));
			if (const size_t rozmiar = this->idiki.size(); rozmiar > 10) {
				if (rozmiar % 10 >= 2 && rozmiar % 10 <= 4)
					this->killMobs.setString(this->killMobs.getString() + L" potwory");
				else
					this->killMobs.setString(this->killMobs.getString() + L" potworów");
			}
			else {
				if (rozmiar == 1)
					this->killMobs.setString(this->killMobs.getString() + L" potwór");
				else if (rozmiar < 5)
					this->killMobs.setString(this->killMobs.getString() + L" potwory");
				else
					this->killMobs.setString(this->killMobs.getString() + L" potworów");
			}
			this->killMobs.setPosition(float(unsigned(640 * this->scale - this->killMobs.getGlobalBounds().width / 2)), this->killMobs.getPosition().y);
		}
	}


	
	if (this->waveCountdown >= 10.f) {
		if (this->potwory.size() == 0) {
			this->wave++;
			this->waveNumber.setString("FALA " + std::to_string(this->wave));
			
			this->addMonsters();
			this->monsterCount.setString(L"Pozosta³ych potworów:");
			this->monsterCount.setString(this->monsterCount.getString() + std::to_string(this->potwory.size()));
			this->titleCountdown = 0.f;
		}
		if (this->potwory.size() > 0) {
			for (auto& mob : this->potwory) {
				if (!mob.dead && !mob.punched) {

					if (mob.sprite.getColor().a == sf::Uint8(255)) {

						mob.AI(this->obstacles, this->player->sprite, this->potwory, this->player->sprite.getPosition().x, this->player->sprite.getPosition().y, dt);
						mob.collision(this->potwory);
						mob.animation(dt);
						mob.move();

						if (vectorDistance(mob.sprite.getPosition(), this->player->sprite.getPosition()) <= mob.reach * 64 * this->scale && mob.name != "Cyklop")
						{
							if (mob.attackCooldown < 1.f)
								mob.attackCooldown += mob.attackSpeed * dt;

							if (mob.attackCooldown >= 1.f && mob.attackAni == false) {
								mob.attackCooldown = 0.f;
								mob.step = 0;
								mob.attackAni = true;
							}

							if (mob.step == 80 && mob.attackAni && this->player->punched == false)
							{
								int mobAttack = mob.attack;
								if (this->player->name == "Rycerz" && this->player->abilityActive) {
									mobAttack = int(mobAttack / 2);
								}

								if (mobAttack > 0) {
									this->player->punched = true;
									this->player->hp -= mobAttack;

									damage_t.init(this->scale, "-" + std::to_string(mobAttack), sf::Vector2f(this->player->sprite.getPosition().x + 48 * this->scale, this->player->sprite.getPosition().y + 32 * this->scale), this->font, 228, 92, 95);
									damage_texts.push_back(damage_t);

									if (this->player->hp <= 0) {
										this->player->hp = 0;
										this->player->dead = true;
										this->paused = true;
										this->hp_bar.setTextureRect(sf::IntRect(0, 22, int(float(this->player->hp) / float(this->player->maxhp) * 264), 22));
										this->hp.setString("HP:" + std::to_string(this->player->hp) + "/" + std::to_string(this->player->maxhp));
										this->hp.setPosition(float(int(640 * this->scale - (this->hp.getLocalBounds().width) / 2)), this->hp.getPosition().y);
										break;
									}
									this->temp_hp_bar = int(float(this->player->hp) / float(this->player->maxhp) * 264 * this->scale);

									this->hp.setString("HP:" + std::to_string(this->player->hp) + "/" + std::to_string(this->player->maxhp));
									this->hp.setPosition(float(int(640 * this->scale - (this->hp.getLocalBounds().width) / 2)), this->hp.getPosition().y);
								}
							}
						}
						else if (vectorDistance(mob.sprite.getPosition(), this->player->sprite.getPosition()) <= mob.reach * 1024 * this->scale && mob.name == "Cyklop" && (mob.predkosc.x != 0 || mob.predkosc.y != 0)) {
							if (mob.attackCooldown < 1.f)
								mob.attackCooldown += mob.attackSpeed * dt / 2;

							if (mob.attackCooldown >= 1.f && mob.attackAni == false) {
								mob.attackCooldown = 0.f;
								mob.step = 0;
								mob.attackAni = true;
							}
							if (mob.step == 80 && mob.attackAni && !mob.fired)
							{
								mob.fired = true;
								//mob.spawnProjectile(this->pociski, this->projectile, this->stone_texture, this->player->sprite, dt);
								if (mob.name == "Cyklop") {
									Projectile skala;
									skala.init(this->scale, "Skala", int(3 * this->difficultyModifier), 1, 1);
									skala.texture = this->stone_texture;
									skala.sprite.setTexture(this->stone_texture);
									skala.sprite.setTextureRect(sf::IntRect(0, 0, 4, 4));
									skala.sprite.setScale(4 * this->scale, 4 * this->scale);

									if (mob.left)
										skala.sprite.setPosition(mob.sprite.getPosition().x, mob.sprite.getPosition().y + 16 * this->scale);
									else if (mob.right)
										skala.sprite.setPosition(mob.sprite.getPosition().x + 64 * this->scale, mob.sprite.getPosition().y + 16 * this->scale);
									else if (mob.up)
										skala.sprite.setPosition(mob.sprite.getPosition().x + 20 * this->scale, mob.sprite.getPosition().y + 16 * this->scale);
									else if (mob.down)
										skala.sprite.setPosition(mob.sprite.getPosition().x + 24 * this->scale, mob.sprite.getPosition().y + 36 * this->scale);
									skala.calculateVelocity(sf::Vector2f(this->player->sprite.getPosition().x + 32 * this->scale, this->player->sprite.getPosition().y + 32 * this->scale), dt);
									this->pociski.push_back(skala);
								}
							}
						}
					}
					else mob.spawning(dt);
				}
				else if (!mob.dead) {
					mob.damaged(dt);
				}
			}

		}
	}
	else {
		this->waveCountdown += dt;

		this->nextWave.setString("Nastepna fala za:" + std::to_string(int(11.f - this->waveCountdown)));
		if (float czas = this->waveCountdown; czas > 8.f && czas < 9.f && this->titleCountdown < 255.f) {
			this->titleCountdown += dt * 1000.f;
			if (this->titleCountdown > 255.f) this->titleCountdown = 255.f;
			this->bigWave.setFillColor(sf::Color(255, 255, 255, sf::Uint8(this->titleCountdown)));
			this->killMobs.setFillColor(sf::Color(192, 192, 192, sf::Uint8(this->titleCountdown)));
		}
		else if (czas > 9.f && this->titleCountdown > 0.f) {
			this->titleCountdown -= dt * 1000.f;
			if (this->titleCountdown < 0.f) this->titleCountdown = 0.f;
			this->bigWave.setFillColor(sf::Color(255, 255, 255, sf::Uint8(this->titleCountdown)));
			this->killMobs.setFillColor(sf::Color(192, 192, 192, sf::Uint8(this->titleCountdown)));
		}
	}

	for (auto dropek = this->drops.begin(); dropek != this->drops.end();) {
		dropek->spin(dt);
		const float distance = vectorDistance(sf::Vector2f(dropek->sprite.getPosition().x + 8 * this->scale, dropek->sprite.getPosition().y + 8 * this->scale), sf::Vector2f(this->player->sprite.getPosition().x + 32 * this->scale, this->player->sprite.getPosition().y + 32 * this->scale));
		if (distance <= this->player->reach * 64 * this->scale) {
			dropek->angle = getAngle(dropek->sprite.getPosition().x + 8 * this->scale, dropek->sprite.getPosition().y + 8 * this->scale, this->player->sprite.getPosition().x + 32 * this->scale, this->player->sprite.getPosition().y + 32 * this->scale) + 90.f;
			dropek->move(dt);
		}
		if (distance <= this->player->reach * 16 * this->scale) {
			if (dropek->name == "coin") {
				this->player->gold += dropek->worth;
				this->gold.setString(std::to_string(this->player->gold));
				this->damage_t.init(this->scale, "+" + std::to_string(dropek->worth), sf::Vector2f(dropek->sprite.getPosition().x - 16 * this->scale, dropek->sprite.getPosition().y), this->font, 255, 246, 76);
				this->damage_texts.push_back(this->damage_t);
			}
			else if (this->player->hp < this->player->maxhp) {
				this->player->hp++;
				this->damage_t.init(this->scale, "+1", sf::Vector2f(dropek->sprite.getPosition().x - 16 * this->scale, dropek->sprite.getPosition().y - 16 * this->scale), this->font, 182, 60, 53);

				this->hp.setString("HP:" + std::to_string(this->player->hp) + "/" + std::to_string(this->player->maxhp));
				center(this->hp, 640 * this->scale);
				this->temp_reg_bar = int(float(this->player->hp) / float(this->player->maxhp) * 264 * this->scale);
				this->regen = true;

				this->damage_texts.push_back(this->damage_t);
				
			}
			
			dropek = this->drops.erase(dropek);
		}
		else ++dropek;
	}


	this->player->damaged(dt);

	for (auto& poc : this->pociski) {
		if (poc.name == "Shuriken") {
			poc.sprite.rotate(90 / dt);
		}
		poc.checkCollision(this->obstacles);
		if (poc.name == "Skala") poc.collisionPlayer(this->player->sprite);
		poc.move(dt);
	}

	for (auto pocisk = this->pociski.begin(); pocisk != this->pociski.end();) {
		if (pocisk->collidedPlayer) {
			int pociskAttack = pocisk->attack;
			if (this->player->name == "Rycerz" && this->player->abilityActive) {
				pociskAttack = int(pociskAttack / 2);
			}

			if (pociskAttack > 0) {
				this->player->punched = true;
				this->player->hp -= pociskAttack;
				damage_t.init(this->scale, "-" + std::to_string(pociskAttack), sf::Vector2f(this->player->sprite.getPosition().x + 32 * this->scale, this->player->sprite.getPosition().y + 32 * this->scale), this->font, 228, 92, 95); damage_texts.push_back(damage_t);

				if (this->player->hp <= 0) {
					this->player->hp = 0;
					this->player->dead = true;
					this->paused = true;
					this->hp_bar.setTextureRect(sf::IntRect(0, 22, int(float(this->player->hp) / float(this->player->maxhp) * 264), 22));
					this->hp.setString("HP:" + std::to_string(this->player->hp) + "/" + std::to_string(this->player->maxhp));
					this->hp.setPosition(float(int(640 * this->scale - (this->hp.getLocalBounds().width) / 2)), this->hp.getPosition().y);
					break;
				}
				this->temp_hp_bar = int(float(this->player->hp) / float(this->player->maxhp) * 264 * this->scale);

				this->hp.setString("HP:" + std::to_string(this->player->hp) + "/" + std::to_string(this->player->maxhp));
				this->hp.setPosition(float(int(640 * this->scale - (this->hp.getLocalBounds().width) / 2)), this->hp.getPosition().y);
			}
			pocisk = this->pociski.erase(pocisk);
		}
		else if (pocisk->hp <= 0 || pocisk->sprite.getPosition().x < 0 || pocisk->sprite.getPosition().y < 0 || pocisk->sprite.getPosition().x > 2048 || pocisk->sprite.getPosition().y > 2048) {
			pocisk = this->pociski.erase(pocisk);
		}
		else ++pocisk;
	}

	for (auto& mob : this->potwory) {
		for (auto pocisk = this->pociski.begin(); pocisk != this->pociski.end();) {
			if (pocisk->name == "Shuriken") {
				pocisk->collisionMob(mob.sprite);
				if (pocisk->collidedMob) {
					if (int(Random::Float() * 100.f) + 1 <= this->player->critical) {
						mob.hp -= pocisk->attack * 2;
						damage_t.init(this->scale, "-" + std::to_string(pocisk->attack * 2), sf::Vector2f(mob.sprite.getPosition().x + 32 * this->scale, mob.sprite.getPosition().y + 32 * this->scale), this->font, 233, 134, 39);
					}
					else {
						mob.hp -= pocisk->attack;
						damage_t.init(this->scale, "-" + std::to_string(pocisk->attack), sf::Vector2f(mob.sprite.getPosition().x + 32 * this->scale, mob.sprite.getPosition().y + 32 * this->scale), this->font, 255, 255, 255);
					}
					damage_texts.push_back(damage_t);
					mob.punched = true;
					mob.punchedTime = 0.f;
					mob.step = 0;
					mob.attackAni = false;
					if (mob.hp <= 0) {
						mob.dead = true;
						mob.sprite.setColor(sf::Color(182, 60, 53));
					}
					pocisk = this->pociski.erase(pocisk);
				}
				else ++pocisk;
			}
			else ++pocisk;
		}
	}

	for (auto& t : this->damage_texts) {
		t.move_cd += dt;
		t.move(dt);
	}

	for (auto t = this->damage_texts.begin(); t != this->damage_texts.end();) {
		if (t->move_cd >= 1.f) {
			t = this->damage_texts.erase(t);
		}
		else ++t;
	}

	if (this->hp_bar.getGlobalBounds().width > this->temp_hp_bar && this->player->punched) {
		this->hp_bar.setTextureRect(sf::IntRect(0, 22, int(this->hp_bar.getGlobalBounds().width / this->scale - 1000.f * dt), 22));
	}
	else {
		if (this->hp_bar.getGlobalBounds().width < this->temp_reg_bar && this->regen) {
			this->hp_bar.setTextureRect(sf::IntRect(0, 22, int(this->hp_bar.getGlobalBounds().width / this->scale + 1000.f * dt), 22));
		}
		else this->regen = false;
	}

	if (this->xp_bar.getGlobalBounds().width < this->temp_lvl_bar && this->levelup) {
		this->xp_bar.setTextureRect(sf::IntRect(0, 0, int(this->xp_bar.getGlobalBounds().width / this->scale + 1000.f * dt), 22));
	}
	else this->levelup = false;
	
}

void GameState::deathScreenUpdate(sf::RenderWindow& window, unsigned& state)
{
	buttonText(window, this->menu, "menu", state);
	buttonText(window, this->quit, "wyjdz", state);
}

void GameState::buttonSprite(sf::RenderWindow& window, sf::Sprite& spr, const std::string& word)
{
	if (spr.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
		spr.setColor(sf::Color::White);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->mycha == false) {
			if (word == "option1") {
				if (this->option1_id == 1) {
					this->player->attack += this->option1_up;
					this->attack.setString(std::to_string(this->player->attack));
					center(this->attack, 828 * this->scale);
				}
				else if (this->option1_id == 2) {
					this->player->attackSpeed += this->option1_up;
					this->attackSpeed.setString(std::to_string(this->player->attackSpeed));
					center(this->attackSpeed, 892 * this->scale);
				}
				else if (this->option1_id == 3) {
					this->player->speed += this->option1_up;
					this->speed.setString(std::to_string(this->player->speed));
					center(this->speed, 956 * this->scale);
				}
				else if (this->option1_id == 4) {
					this->player->critical += this->option1_up;
					this->critical.setString(std::to_string(this->player->critical) + "%");
					center(this->critical, 1020 * this->scale);
				}
				else if (this->option1_id == 5) {
					this->player->Reg += this->option1_up;
					this->Reg.setString(std::to_string(this->player->Reg));
					center(this->Reg, 452 * this->scale);
				}
				else if (this->option1_id == 6) {
					this->player->maxhp += this->option1_up;
					this->hp.setString("HP:" + std::to_string(this->player->hp) + "/" + std::to_string(this->player->maxhp));
					center(this->hp, 640 * this->scale);
					this->hp_bar.setTextureRect(sf::IntRect(0, 22, int(float(this->player->hp) / float(this->player->maxhp) * 264), 22));
				}

				if (this->potwory.size() == 0) this->waveCountdown = 0.f;
				this->leveling = false;
				if (!this->upgrading) this->paused = false;
				
			}
			else if (word == "option2") {
				if (this->option2_id == 1) {
					this->player->attack += this->option2_up;
					this->attack.setString(std::to_string(this->player->attack));
					center(this->attack, 828 * this->scale);
				}
				else if (this->option2_id == 2) {
					this->player->attackSpeed += this->option2_up;
					this->attackSpeed.setString(std::to_string(this->player->attackSpeed));
					center(this->attackSpeed, 892 * this->scale);
				}
				else if (this->option2_id == 3) {
					this->player->speed += this->option2_up;
					this->speed.setString(std::to_string(this->player->speed));
					center(this->speed, 956 * this->scale);
				}
				else if (this->option2_id == 4) {
					this->player->critical += this->option2_up;
					this->critical.setString(std::to_string(this->player->critical) + "%");
					center(this->critical, 1020 * this->scale);
				}
				else if (this->option2_id == 5) {
					this->player->Reg += this->option2_up;
					this->Reg.setString(std::to_string(this->player->Reg));
					center(this->Reg, 452 * this->scale);
				}
				else if (this->option2_id == 6) {
					this->player->maxhp += this->option2_up;
					this->hp.setString("HP:" + std::to_string(this->player->hp) + "/" + std::to_string(this->player->maxhp));
					center(this->hp, 640 * this->scale);
					this->hp_bar.setTextureRect(sf::IntRect(0, 22, int(float(this->player->hp) / float(this->player->maxhp) * 264), 22));
				}

				if (this->potwory.size() == 0) this->waveCountdown = 0.f;
				this->leveling = false;
				if (!this->upgrading) this->paused = false;
			}
			else if (word == "ninja") {
				if (this->player->Level == 5) {
					this->player->texture.loadFromFile("external/assets/heroes/ninja.png");
					this->player->name = "Ninja";
					this->playerName.setString(this->player->name);
					this->playerName.setPosition(float(int(164 * this->scale + 32 * this->scale - (this->playerName.getGlobalBounds().width / 2))), 96 * this->scale);
					this->player->speed++;
					this->speed.setString(std::to_string(this->player->speed));
					center(this->speed, 956 * this->scale);
					this->ability_icon.setTextureRect(sf::IntRect(0, 0, 16, 16));
					this->player->abilityCooldown = 2.f;
				}
				this->upgrading = false;
				if (!this->leveling) this->paused = false;
			}
			else if (word == "knight") {
				if (this->player->Level == 5) {
					this->player->texture.loadFromFile("external/assets/heroes/knight.png");
					this->player->name = "Rycerz";
					this->playerName.setString(this->player->name);
					this->playerName.setPosition(float(int(164 * this->scale + 32 * this->scale - (this->playerName.getGlobalBounds().width / 2))), 96 * this->scale);
					this->player->attack++;
					this->attack.setString(std::to_string(this->player->attack));
					center(this->attack, 828 * this->scale);
					this->ability_icon.setTextureRect(sf::IntRect(16, 0, 16, 16));
					this->player->abilityCooldown = 10.f;
					this->player->abilityDuration = 5.f;
				}
				this->upgrading = false;
				if (!this->leveling) this->paused = false;
			}
			else if (word == "scout") {
				if (this->player->Level == 5) {
					this->player->texture.loadFromFile("external/assets/heroes/scout.png");
					this->player->name = "Skaut";
					this->playerName.setString(this->player->name);
					this->playerName.setPosition(float(int(164 * this->scale + 32 * this->scale - (this->playerName.getGlobalBounds().width / 2))), 96 * this->scale);
					this->player->Reg++;
					this->Reg.setString(std::to_string(this->player->Reg));
					center(this->Reg, 452 * this->scale);
					this->ability_icon.setTextureRect(sf::IntRect(32, 0, 16, 16));
					this->player->abilityCooldown = 10.f;
					this->player->abilityDuration = 5.f;
				}
				this->upgrading = false;
				if (!this->leveling) this->paused = false;
			}


			this->mycha = true;
			spr.setColor(sf::Color::Transparent);
		}
	}
	else spr.setColor(sf::Color::Transparent);
}

void GameState::buttonText(sf::RenderWindow& window, sf::Text& text, const std::string& word, unsigned& state)
{
	if (text.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
		text.setFillColor(sf::Color(192, 192, 192));
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->mycha == false) {
			if (word == "menu") {
				this->obstacles.clear();
				this->potwory.clear();
				this->pociski.clear();
				this->damage_texts.clear();
				this->drops.clear();
				state = 0;
			}
			else if (word == "wyjdz") {
				this->obstacles.clear();
				this->potwory.clear();
				this->pociski.clear();
				this->damage_texts.clear();
				this->drops.clear();
				window.close();
			}
			this->mycha = true;
			text.setFillColor(sf::Color(255, 255, 255));
		}
	}
	else {
		text.setFillColor(sf::Color(255, 255, 255));
	}
}

