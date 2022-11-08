#include "Functions.h"
#include "GameState.h"

GameState::GameState(const float& gridSize, sf::RenderWindow* window, GameSettings* grap,
	std::unordered_map<std::string, int>* supportedKeys, sf::Font* font, std::stack<State*>* states,
	const std::string& map_name, const std::string& hero_name, const std::string& difficulty_name)
	: State(gridSize, window, grap, supportedKeys, font, states)
{

	const sf::VideoMode vm = this->gameSettings->resolution;

	this->currentTrackNumber = 0;
	this->tracks.push_back("battle1.ogg");
	this->tracks.push_back("battle2.ogg");
	this->tracks.push_back("battle3.ogg");
	this->tracks.push_back("battle4.ogg");
	this->tracks.push_back("battle5.ogg");

	this->music.setVolume(this->gameSettings->musicVolume);
	this->music.pause();

	this->soundEngine = new SoundEngine(this->gameSettings->soundsVolume);

	this->background_texture.loadFromFile("external/assets/" + map_name + ".png");
	this->background.setTexture(this->background_texture);
	this->background.setScale(calcScale(4, vm), calcScale(4, vm));

	std::ifstream ifs("external/config/game_keybinds.ini");
	if (ifs.is_open()) {
		std::string key = "";
		std::string key2 = "";
		while (ifs >> key >> key2) {
			this->keybinds[key] = this->supportedKeys->at(key2);
		}
	}
	ifs.close();

	this->tiles_texture.loadFromFile("external/assets/tiles.png");
	this->vertexArray.setPrimitiveType(sf::Quads);
	this->vertexArray.resize(static_cast<size_t>(calcX(64, vm) * calcY(64, vm) * 4.f));
	std::ifstream mapa("external/maps/" + map_name + ".txt");
	float x = 0.f, y = 0.f, pos = calcX(this->gridSize, vm);
	size_t t = 0;
	const sf::Vector2f tile = sf::Vector2f(calcX(64, vm), calcY(64, vm));
	if (mapa.is_open()) {
		std::string temp;
		while (std::getline(mapa, temp)) {
			for (size_t i = 0; i < temp.size(); ++i) {
				if (temp[i] == '#') {
					this->tiles.push_back(new Tile(x, y, tile, "wall"));
					sf::Vertex* quad = &this->vertexArray[t * 4];

					quad[0].position = sf::Vector2f(x, y);
					quad[1].position = sf::Vector2f(x + calcX(64, vm), y);
					quad[2].position = sf::Vector2f(x + calcX(64, vm), y + calcY(64, vm));
					quad[3].position = sf::Vector2f(x, y + calcY(64, vm));

					quad[0].texCoords = sf::Vector2f(48, 16);
					quad[1].texCoords = sf::Vector2f(64, 16);
					quad[2].texCoords = sf::Vector2f(64, 32);
					quad[3].texCoords = sf::Vector2f(48, 32);
				}
				else if (temp[i] == '@') {
					this->tiles.push_back(new Tile(x, y, tile, "wall"));
					sf::Vertex* quad = &this->vertexArray[t * 4];

					quad[0].position = sf::Vector2f(x, y);
					quad[1].position = sf::Vector2f(x + calcX(64, vm), y);
					quad[2].position = sf::Vector2f(x + calcX(64, vm), y + calcY(64, vm));
					quad[3].position = sf::Vector2f(x, y + calcY(64, vm));

					quad[0].texCoords = sf::Vector2f(64, 16);
					quad[1].texCoords = sf::Vector2f(80, 16);
					quad[2].texCoords = sf::Vector2f(80, 32);
					quad[3].texCoords = sf::Vector2f(64, 32);
				}
				else if (temp[i] == 'S') {
					this->player = new Player(x, y, vm, hero_name);
				}
				x += pos;
				t++;
			}
			x = 0.f;
			y += pos;
		}
	}
	mapa.close();

	this->playerGUI = new PlayerGUI(&this->font, this->player, this->gameSettings->resolution, this->gameSettings->soundsVolume, hero_name, difficulty_name, this->lang);

	if (difficulty_name == "easy") this->difficultyModifier = 0.75f;
	else if (difficulty_name == "hard") this->difficultyModifier = 1.25f;
	else this->difficultyModifier = 1.f;

	this->view.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
	this->view.setCenter(sf::Vector2f(this->player->getPosition().x + calcX(32, vm), this->player->getPosition().y + calcY(32, vm)));

	this->viewHUD.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
	this->viewHUD.setCenter(sf::Vector2f(static_cast<float>(vm.width) / 2.f, static_cast<float>(vm.height) / 2.f));

	Random::Init();

	this->keysClick["Escape"].first = false;
	this->keysClick["Escape"].second = false;

	sf::Texture texture;
	texture.loadFromFile("external/assets/monsters/goblin.png");
	this->textures["GOBLIN"] = texture;
	texture.loadFromFile("external/assets/monsters/spider.png");
	this->textures["SPIDER"] = texture;
	texture.loadFromFile("external/assets/monsters/orc.png");
	this->textures["ORC"] = texture;
	texture.loadFromFile("external/assets/monsters/cyclope.png");
	this->textures["CYCLOPE"] = texture;
	texture.loadFromFile("external/assets/monsters/minotaur.png");
	this->textures["MINOTAUR"] = texture;
	texture.loadFromFile("external/assets/entity_shadow.png");
	this->textures["SHADOW"] = texture;

	this->wave = 0;
	this->sumHP = 10;
	this->waveCountdown = 0.f;
}

GameState::~GameState()
{
	delete this->player;
}

void GameState::initGUI()
{
}

void GameState::resetGUI()
{
}

void GameState::prepareWave()
{
	this->wave++;
	if (this->wave % 10 == 0) this->sumHP = 69;
	else this->sumHP += static_cast<uint16_t>(Random::Float() * this->sumHP) + 1;
	uint16_t monstersHP = this->sumHP;
	short t = 0;
	while (monstersHP > 0) {
		if (monstersHP >= 69 && this->wave >= 10) 
			t = 4;
		else if (monstersHP >= 18 && this->wave >= 7) 
			t = static_cast<short>(Random::Float() * 4.f);
		else if (monstersHP >= 12 && this->wave >= 5) 
			t = static_cast<short>(Random::Float() * 3.f);
		else if (monstersHP >= 7 && this->wave >= 3) 
			t = static_cast<short>(Random::Float() * 2.f);
		else if (monstersHP >= 3) 
			t = 0;
		else break;
			
		this->monsterIDs.push_back(t);
			
		switch (t) {
		case 0:
			monstersHP -= 3;
			break;
		case 1:
			monstersHP -= 7;
			break;
		case 2:
			monstersHP -= 12;
			break;
		case 3:
			monstersHP -= 18;
			break;
		case 4:
			monstersHP -= 69;
			break;
		}
	}
	this->playerGUI->updateMonsterCountWave(this->gameSettings->language, this->wave, this->monsterIDs.size());
}

void GameState::spawnMonsters()
{
	const sf::VideoMode vm = this->gameSettings->resolution;

	const float wave_mod = 1.f + static_cast<uint16_t>(this->wave / 10.f);

	for (const auto& id : this->monsterIDs) {
		uint16_t rx = static_cast<uint16_t>(Random::Float() * 32.f), ry = static_cast<uint16_t>(Random::Float() * 32.f);
		for (size_t i = 0; i < this->tiles.size(); ++i) {
			while ((this->tiles[i]->getPosition().x == calcX(this->gridSize * rx, vm) && this->tiles[i]->getPosition().y == calcY(this->gridSize * ry, vm)) ||
				vectorDistance(calcX(this->gridSize * rx, vm), calcY(this->gridSize * ry, vm), this->player->getPosition().x, this->player->getPosition().y) <= calcX(3.f * this->gridSize, vm))
			{
				rx = static_cast<uint16_t>(Random::Float() * 32.f);
				ry = static_cast<uint16_t>(Random::Float() * 32.f);
				i = 0;
			}
			for (const auto& mob : this->monsters) {
				if (mob->getPosition().x == calcX(this->gridSize * rx, vm) && mob->getPosition().y == calcY(this->gridSize * ry, vm)) {
					rx = static_cast<uint16_t>(Random::Float() * 32.f);
					ry = static_cast<uint16_t>(Random::Float() * 32.f);
					i = 0;
				}
			}
		}

		switch (id)
		{
		case 0:
			this->monsters.push_back(new Monster(calcX(this->gridSize * rx, vm), calcY(this->gridSize * ry, vm), this->textures["GOBLIN"], this->textures["SHADOW"], this->tiles, vm, "goblin", this->difficultyModifier, wave_mod));
			break;
		case 1:
			this->monsters.push_back(new Monster(calcX(this->gridSize * rx, vm), calcY(this->gridSize * ry, vm), this->textures["SPIDER"], this->textures["SHADOW"], this->tiles, vm, "spider", this->difficultyModifier, wave_mod));
			break;
		case 2:
			this->monsters.push_back(new Monster(calcX(this->gridSize * rx, vm), calcY(this->gridSize * ry, vm), this->textures["ORC"], this->textures["SHADOW"], this->tiles, vm, "orc", this->difficultyModifier, wave_mod));
			break;
		case 3:
			this->monsters.push_back(new Monster(calcX(this->gridSize * rx, vm), calcY(this->gridSize * ry, vm), this->textures["CYCLOPE"], this->textures["SHADOW"], this->tiles, vm, "cyclope", this->difficultyModifier, wave_mod));
			break;
		case 4:
			this->monsters.push_back(new Monster(calcX(this->gridSize * 16, vm), calcY(this->gridSize * 16, vm), this->textures["MINOTAUR"], this->textures["SHADOW"], this->tiles, vm, "minotaur", this->difficultyModifier, wave_mod));
			break;
		default:
			break;
		}
	}
}

void GameState::draw(sf::RenderTarget* target)
{
	if (!target) target = this->window;

	target->setView(this->view);

	target->draw(this->background);

	for (const auto& monster : this->monsters) {
		monster->drawShadow(*target);
	}

	this->player->drawShadow(*target);

	target->draw(this->vertexArray, &this->tiles_texture);

	for (const auto& proj : this->projectiles) {
		proj->draw(*target);
	}

	for (const auto& drop : this->drops) {
		drop->draw(*target);
	}

	this->player->draw(*target);

	for (const auto& monster : this->monsters) {
		monster->draw(*target);
	}

	for (const auto& text : floatingTexts) {
		if(!text->isGui()) text->draw(*target);
	}

	target->setView(this->viewHUD);

	this->playerGUI->draw(*target);

	for (const auto& text : floatingTexts) {
		if (text->isGui() && this->playerGUI->getIsShopping()) text->draw(*target);
	}
}

void GameState::update(const float& dt)
{
	if (this->music.getStatus() == sf::SoundSource::Status::Stopped)
	{
		this->currentTrackNumber = static_cast<size_t>(Random::Float() * this->tracks.size());

		this->music.openFromFile("external/music/" + this->tracks[this->currentTrackNumber]);
		this->music.play();
	}

	this->updateMousePositions(&this->viewHUD);

	if (this->playerGUI->updateButtons(this->mousePosWindow, this->getMouseClick())) {
		this->setMouseClick(true);
		this->unpauseState();
	}

	if (this->player->isDead()) {
		const uint16_t result = this->playerGUI->updateDeathScreenButtons(this->mousePosWindow, this->getMouseClick());
		if (result == 1) {
			this->setMouseClick(true);
			this->endState();
		}
		else if (result == 2) {
			this->setMouseClick(true);
			this->window->close();
		}
	}

	if (!this->playerGUI->getIsLeveling() && !this->playerGUI->getIsUpgrading()) {

		this->updateKeysClick("Escape", sf::Keyboard::Escape);

		if (this->getKeysClick1("Escape") && !this->getKeysClick2("Escape")) {
			this->setKeysClick("Escape", true);
			this->playerGUI->updatePaused(this->paused);
			if (this->paused) {
				this->music.pause();
				this->soundEngine->stopSounds();
			}
			else this->music.play();
			
		}
		this->setKeysClick("Escape", this->getKeysClick1("Escape"));

	}

	if (this->playerGUI->getIsEscape() && (!this->playerGUI->getIsLeveling() && !this->playerGUI->getIsUpgrading())) {
		const uint16_t result = this->playerGUI->updateEscapeButton(this->mousePosWindow, this->getMouseClick());
		if (result == 1) {
			this->endState();
		}
		else if (result == 2) {
			this->window->close();
		}
		else if (result == 3) {
			this->paused = false;
			this->music.play();
			this->soundEngine->playSounds();
		}
		else if (result == 4) {
			this->states->push(new SettingsState(this->gridSize, this->window, this->gameSettings, this->supportedKeys, &this->font, this->states));
		}
	}

	

	if (!this->paused) {
		if (this->player->getSpawned()) {
			this->player->controls(this->keybinds, dt);

			if (this->player->getPunched()) this->player->smashed(dt);

			if (this->player->hasVelocity()) {
				this->player->obstacleCollision(this->tiles);

				this->player->move();

				this->player->update(dt);
				this->player->swipeSound(this->soundEngine);
				
				const float _32 = calcX(32, this->gameSettings->resolution);
				const bool osY = this->player->getPosition().y <= calcY(200, this->gameSettings->resolution) || this->player->getPosition().y >= this->background.getGlobalBounds().height - calcY(392, this->gameSettings->resolution);
				const bool osX = this->player->getPosition().x <= calcX(608, this->gameSettings->resolution) || this->player->getPosition().x >= this->background.getGlobalBounds().width - calcX(672, this->gameSettings->resolution);

				if (osX || osY) {
					if (osX && osY) {
						this->view.setCenter(sf::Vector2f(this->view.getCenter().x, this->view.getCenter().y));
					}
					else {
						if (osX) {
							this->view.setCenter(sf::Vector2f(this->view.getCenter().x, this->player->getPosition().y + _32));
						}
						else if (osY) {
							this->view.setCenter(sf::Vector2f(this->player->getPosition().x + _32, view.getCenter().y));
						}
					}
				}
				else this->view.setCenter(this->player->getPosition().x + _32, this->player->getPosition().y + _32);
			}

			this->player->loadAttack(dt);

			if (this->mousePosView.y > calcY(128, this->gameSettings->resolution)) {
				if (this->playerGUI->getIsShopping()) {
					if (this->mousePosView.x > calcX(292, this->gameSettings->resolution) && this->mouseClick)
						this->player->doAttack();
				}
				else if (this->mouseClick) this->player->doAttack();

				if (this->player->checkIfAbility()) {
					this->player->doAbility(sf::Vector2f(this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window), this->view)), this->projectiles);
					this->playerGUI->setAbilityIcon();
					this->playerGUI->updateArmor();
				}
			}
			this->player->spawn(dt);
			this->player->animation(dt);
			this->player->abilityCounter(dt);
			this->playerGUI->updateArmor();
			this->playerGUI->update_ability(dt);

		}
		else this->player->spawn(dt);

		if (this->waveCountdown < 10.f)
		{
			this->waveCountdown += dt;

			if (this->player->getSpawned()) {
				this->updateKeysClick("E", sf::Keyboard::E);

				if (this->getKeysClick1("E") && !this->getKeysClick2("E")) {
					this->setKeysClick("E", true);
					this->playerGUI->updateIsShopping(true);
				}
				this->setKeysClick("E", this->getKeysClick1("E"));

				if (this->playerGUI->updateShop(this->mousePosWindow, this->getMouseClick(), this->floatingTexts)) {
					this->setMouseClick(true);
				}
				
			}

			if (this->waveCountdown >= 10.f) {
				this->spawnMonsters();
				this->playerGUI->updateIsShopping(false);
			}
			else if (this->waveCountdown > 8.f && this->monsterIDs.size() == 0) {
				this->prepareWave();
			}
		}
		else {
			if (this->monsters.size() == 0) {
				this->waveCountdown = 0.f;
				this->monsterIDs.clear();
			}
			else {
				const sf::VideoMode vm = this->gameSettings->resolution;
				this->player->attackMonster(&this->font, this->monsters, this->floatingTexts, this->soundEngine);
				for (const auto& monster : this->monsters) {
					if (monster->getSpawned()) {
						if (monster->isDead()) {
							monster->dying(dt);
						}
						else if (monster->getPunched()) {
							monster->smashed(dt);
						}
						else {
							monster->AI(this->tiles, this->player, dt);
							if (monster->hasVelocity()) {
								monster->obstacleCollision(this->tiles);
								monster->monsterCollision(this->monsters);
								monster->move();
								monster->update(dt);
							}
							monster->loadAttack(dt);
							if (monster->attackPlayer(this->player, &this->font, this->tiles, this->projectiles, this->floatingTexts, this->soundEngine)) {
								this->playerGUI->update_HP();
							}
							monster->animation(dt);
						}
					}
					else monster->spawn(dt);
				}
				for (auto monster = this->monsters.begin(); monster != this->monsters.end();) {
					if ((*monster)->getDeadCountdown()) {
						if (this->player->addXP((*monster)->getXP())) {
							this->playerGUI->update_level(this->soundEngine);
							this->pauseState();
						}
						this->playerGUI->update_XP();
						drops.push_back(new Drop("coin", (*monster)->getPosition().x + calcX(16, vm), (*monster)->getPosition().y + calcY(16, vm), (*monster)->getGold(), this->gameSettings->resolution));
						if (int(Random::Float() * 4) == 0) {
							drops.push_back(new Drop("heart", (*monster)->getPosition().x + calcX(16, vm), (*monster)->getPosition().y, 1, this->gameSettings->resolution));
						}
						monster = this->monsters.erase(monster);

						this->player->setKills(this->player->getKills() + 1);
						this->playerGUI->updateKills();
						if (this->monsters.size() > 0)
							this->playerGUI->updateMonsterCount(this->monsters.size());
					}
					else ++monster;
				}
			}
		}
		this->playerGUI->updating_XP(dt);
		this->playerGUI->updating_HP(this->soundEngine, dt);

		for (const auto& proj : this->projectiles) {
			proj->obstacleCollision(this->tiles);
			proj->playerCollision(this->player);
			for (const auto& monster : this->monsters) {
				proj->monsterCollision(monster, &this->font, this->player, this->floatingTexts);
				if (proj->getCollidedMonster()) break;
			}
			proj->update(dt);
		}
		const sf::VideoMode vm = this->gameSettings->resolution;
		for (auto proj = this->projectiles.begin(); proj != this->projectiles.end();) {
			if ((*proj)->getHP() == 0) {
				proj = this->projectiles.erase(proj);
			}
			else if ((*proj)->getCollidedPlayer()) {
				int attack = static_cast<int>(round((*proj)->getAttack() - ((*proj)->getAttack() * player->getArmor() * 0.05f)));

				if (attack > 0) {
					if (static_cast<int>(this->player->getHP() - attack) < 0) this->player->setHP(0);
					else this->player->setHP(this->player->getHP() - attack);

					this->player->punch();

					this->floatingTexts.push_back(new FloatingText(&this->font, "-" + std::to_string(attack), calcChar(16, vm), this->player->getPosition().x + calcX(32, vm), this->player->getPosition().y + calcY(32, vm), sf::Color(228, 92, 95), false, this->gameSettings->resolution));
					this->playerGUI->update_HP();
				}

				proj = this->projectiles.erase(proj);
			}
			else if (!this->background.getGlobalBounds().contains((*proj)->getPosition())) {
				proj = this->projectiles.erase(proj);
			}
			else ++proj;
		}

		for (const auto& drop : this->drops) {
			drop->update(dt);
		}

		for (auto drop = this->drops.begin(); drop != this->drops.end();) {
			if ((*drop)->getSpawned() && (*drop)->playerPick(this->player, &this->font, this->playerGUI, this->floatingTexts, this->soundEngine, dt)) {
				drop = this->drops.erase(drop);
			}
			else ++drop;
		}

		if (this->player->isDead()) {
			this->paused = true;
			this->player->setIsRegenerating(false);
			this->playerGUI->update_HP();
		}

		this->soundEngine->deleteSound();
	}

	this->playerGUI->update(this->mousePosView, this->waveCountdown, dt);

	for (const auto& text : this->floatingTexts) {
		text->update(dt);
	}

	for (auto text = this->floatingTexts.begin(); text != this->floatingTexts.end();) {
		if ((*text)->isOver()) {
			text = this->floatingTexts.erase(text);
		}
		else ++text;
	}

	this->updateMouseClick();
}
