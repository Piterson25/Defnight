#include "stdafx.h"
#include "Functions.h"
#include "GameState.h"

GameState::GameState(const float& gridSize, sf::RenderWindow* window, GameSettings* grap,
	std::unordered_map<std::string, int>* supportedKeys, sf::Font* font, SoundEngine* soundEngine, MusicEngine* musicEngine, std::stack<State*>* states,
	const std::string& map_name, const std::string& hero_name, const std::string& difficulty_name)
	: State(gridSize, window, grap, supportedKeys, font, soundEngine, musicEngine, states)
{
	const sf::VideoMode vm = this->gameSettings->resolution;

	this->musicEngine->clearMusic();
	this->musicEngine->addMusic("battle1.ogg");
	this->musicEngine->addMusic("battle2.ogg");
	this->musicEngine->addMusic("battle3.ogg");
	this->musicEngine->addMusic("battle4.ogg");
	this->musicEngine->addMusic("battle5.ogg");
	this->musicEngine->addMusic("battle6.ogg");

	this->floatingTextSystem = new FloatingTextSystem(&this->font, vm);
	this->tileMap = new TileMap();

	this->background_texture.loadFromFile("external/assets/maps/" + map_name + ".png");
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
	this->vertexArray.resize(static_cast<size_t>(calcX(64 * 64 * 4, vm)));
	std::ifstream mapa("external/maps/" + map_name + ".txt");
	float x = 0.f, y = 0.f, pos = calcX(this->gridSize, vm);

	float offsetY = 0.f;
	if (map_name == "desert") offsetY = 48.f;
	else if (map_name == "permafrost") offsetY = 96.f;
	size_t t = 0;
	const sf::Vector2f tile = sf::Vector2f(calcX(64, vm), calcY(64, vm));
	if (mapa.is_open()) {
		std::string temp;
		while (std::getline(mapa, temp)) {
			for (size_t i = 0; i < temp.size(); ++i) {
				if (temp[i] == '#') {
					this->tileMap->addTile(x, y, tile, "wall");
					sf::Vertex* quad = &this->vertexArray[t * 4];

					quad[0].position = sf::Vector2f(x, y);
					quad[1].position = sf::Vector2f(x + calcX(64, vm), y);
					quad[2].position = sf::Vector2f(x + calcX(64, vm), y + calcY(64, vm));
					quad[3].position = sf::Vector2f(x, y + calcY(64, vm));

					quad[0].texCoords = sf::Vector2f(48, 16 + offsetY);
					quad[1].texCoords = sf::Vector2f(64, 16 + offsetY);
					quad[2].texCoords = sf::Vector2f(64, 32 + offsetY);
					quad[3].texCoords = sf::Vector2f(48, 32 + offsetY);
				}
				else if (temp[i] == '@') {
					this->tileMap->addTile(x, y, tile, "wall");
					sf::Vertex* quad = &this->vertexArray[t * 4];

					quad[0].position = sf::Vector2f(x, y);
					quad[1].position = sf::Vector2f(x + calcX(64, vm), y);
					quad[2].position = sf::Vector2f(x + calcX(64, vm), y + calcY(64, vm));
					quad[3].position = sf::Vector2f(x, y + calcY(64, vm));

					quad[0].texCoords = sf::Vector2f(64, 16 + offsetY);
					quad[1].texCoords = sf::Vector2f(80, 16 + offsetY);
					quad[2].texCoords = sf::Vector2f(80, 32 + offsetY);
					quad[3].texCoords = sf::Vector2f(64, 32 + offsetY);
				}
				else if (temp[i] == 'S') {
					this->player = new Player(vm, hero_name, x, y);
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


	this->view.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
	this->view.setCenter(sf::Vector2f(this->player->getPosition().x + calcX(32, vm), this->player->getPosition().y + calcY(32, vm)));

	this->viewHUD.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
	this->viewHUD.setCenter(sf::Vector2f(static_cast<float>(vm.width) / 2.f, static_cast<float>(vm.height) / 2.f));

	Random::Init();

	this->keysClick["Escape"].first = false;
	this->keysClick["Escape"].second = false;

	if (difficulty_name == "easy") {
		this->monsterSystem = new MonsterSystem(vm, this->tileMap, this->gridSize, 0.75f);
		this->dropSystem = new DropSystem(0.75f, vm);
	}
	else if (difficulty_name == "hard") {
		this->monsterSystem = new MonsterSystem(vm, this->tileMap, this->gridSize, 1.25f);
		this->dropSystem = new DropSystem(1.25f, vm);
	}
	else {
		this->monsterSystem = new MonsterSystem(vm, this->tileMap, this->gridSize, 1.f);
		this->dropSystem = new DropSystem(1.f, vm);
	}

	this->projectileSystem = new ProjectileSystem(vm);
	this->particleSystem = new ParticleSystem(vm);

	this->wave = 0;
	this->sumHP = 15;
	this->waveCountdown = 0.f;
	this->gems = 0;
}

GameState::~GameState()
{
	this->musicEngine->clearMusic();
	delete this->playerGUI;
	delete this->player;
	delete this->monsterSystem;
	delete this->projectileSystem;
	delete this->floatingTextSystem;
	delete this->dropSystem;
	delete this->tileMap;
	delete this->particleSystem;
}

void GameState::initGUI()
{
}

void GameState::resetGUI()
{
}

void GameState::draw(sf::RenderTarget* target)
{
	if (!target) target = this->window;

	target->setView(this->view);

	target->draw(this->background);

	this->monsterSystem->drawShadow(*target);

	this->player->drawShadow(*target);

	target->draw(this->vertexArray, &this->tiles_texture);

	this->projectileSystem->draw(*target);

	this->dropSystem->draw(*target);

	this->player->draw(*target);

	this->monsterSystem->draw(*target);

	this->particleSystem->draw(*target);

	this->floatingTextSystem->draw(*target);

	target->setView(this->viewHUD);

	this->playerGUI->draw(*target);

	this->floatingTextSystem->drawGui(*target);
}

void GameState::update(const float& dt)
{
	this->updateMousePositions(&this->viewHUD);

	if (this->playerGUI->updateButtons(this->mousePosWindow, this->getMouseClick(), this->soundEngine)) {
		this->setMouseClick(true);
		this->unpauseState();
	}

	if (this->player->isDead()) {
		const uint16_t result = this->playerGUI->updateDeathScreenButtons(this->mousePosWindow, this->getMouseClick());
		if (result == 1) {
			this->setMouseClick(true);
			this->soundEngine->addSound("button");
			this->endState();
		}
		else if (result == 2) {
			this->setMouseClick(true);
			this->soundEngine->addSound("button");
			this->window->close();
		}
	}

	if (!this->playerGUI->getIsLeveling() && !this->playerGUI->getIsUpgrading()) {

		this->updateKeysClick("Escape", sf::Keyboard::Escape);

		if (this->getKeysClick1("Escape") && !this->getKeysClick2("Escape")) {
			this->setKeysClick("Escape", true);
			this->playerGUI->updatePaused(this->paused);
			if (this->paused) {
				this->musicEngine->pauseMusic();
				this->soundEngine->stopSounds();
			}
			else this->musicEngine->playMusic();
			
		}
		this->setKeysClick("Escape", this->getKeysClick1("Escape"));

	}

	if (this->playerGUI->getIsEscape() && (!this->playerGUI->getIsLeveling() && !this->playerGUI->getIsUpgrading())) {
		const uint8_t result = this->playerGUI->updateEscapeButton(this->mousePosWindow, this->getMouseClick());
		if (result == 1) {
			this->soundEngine->addSound("button");
			this->endState();
		}
		else if (result == 2) {
			this->soundEngine->addSound("button");
			this->window->close();
		}
		else if (result == 3) {
			this->paused = false;
			this->musicEngine->playMusic();
			this->soundEngine->playSounds();
			this->soundEngine->addSound("button");
		}
		else if (result == 4) {
			this->soundEngine->addSound("button");
			this->states->push(new SettingsState(this->gridSize, this->window, this->gameSettings, this->supportedKeys, &this->font, this->soundEngine, this->musicEngine, this->states));
		}
	}

	

	if (!this->paused) {
		if (this->player->getSpawned()) {
			this->player->controls(this->keybinds, dt);
			this->player->updateSprint(dt);

			if (this->player->getPunched()) this->player->smashed(dt);

			if (this->player->hasVelocity()) {
				this->player->obstacleCollision(this->tileMap);

				this->player->move();
				
				this->player->update(dt);
				
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
			this->player->whooshSound(this->soundEngine);

			if (this->mousePosView.y > calcY(128, this->gameSettings->resolution)) {
				if (!this->playerGUI->getIsShopping() && this->mouseClick) {
					this->player->doAttack();
				}

				if (this->player->checkIfAbility()) {
					this->player->doAbility(sf::Vector2f(this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window), this->view)), this->projectileSystem, this->soundEngine);
					this->playerGUI->setAbilityIcon();
				}
			}
			this->player->spawn(dt);
			this->player->animation(dt);
			this->player->abilityCounter(dt);
			this->playerGUI->updateArmor();
			this->playerGUI->updateAttack();
			this->playerGUI->updateReg();
			this->playerGUI->update_ability(dt);

		}
		else this->player->spawn(dt);

		if (this->player->getSpawned()) {
			this->updateKeysClick("E", sf::Keyboard::E);

			if (this->getKeysClick1("E") && !this->getKeysClick2("E")) {
				this->setKeysClick("E", true);
				this->playerGUI->updateIsShopping();
			}
			this->setKeysClick("E", this->getKeysClick1("E"));

			if (this->playerGUI->updateShop(this->mousePosWindow, this->getMouseClick(), this->soundEngine, this->floatingTextSystem)) {
				this->setMouseClick(true);
			}

		}

		if (this->waveCountdown < 10.f)
		{
			this->waveCountdown += dt;

			if (this->waveCountdown >= 10.f) {
				this->monsterSystem->spawnMonsters(this->player, this->wave);
			}
			else if (this->waveCountdown > 8.f && this->monsterSystem->monsterIDsEmpty()) {
				this->monsterSystem->prepareWave(this->wave, this->sumHP);
				this->playerGUI->updateMonsterCountWave(this->gameSettings->language, this->wave, this->monsterSystem->isBossWave(), this->monsterSystem->monsterIDsSize(), this->soundEngine);
			}
		}
		else {
			if (this->monsterSystem->monstersEmpty()) {
				this->waveCountdown = 0.f;
				this->monsterSystem->monsterIDsClear();
				this->gems += 5;
			}
			else {
				this->player->attackMonster(this->monsterSystem, this->floatingTextSystem, this->soundEngine);
				this->monsterSystem->update(this->player, this->playerGUI, this->projectileSystem, this->dropSystem, this->floatingTextSystem, this->soundEngine, this->paused, dt);
				this->playerGUI->updateBossHP(dt);
			}
		}

		this->projectileSystem->update(this->player, this->playerGUI, this->particleSystem, this->monsterSystem, this->background, this->tileMap, this->floatingTextSystem, this->soundEngine, dt);

		this->dropSystem->update(this->player, this->playerGUI, this->floatingTextSystem, this->soundEngine, dt);

		if (this->player->isDead()) {
			this->paused = true;
			this->player->setIsRegenerating(false);
			this->playerGUI->update_HP();
		}

		this->playerGUI->updating_XP(dt);
		this->playerGUI->updating_HP(this->soundEngine, dt);
		this->playerGUI->updateSprint(dt);

		this->soundEngine->update();
		this->musicEngine->update();
	}

	this->playerGUI->update(this->mousePosView, this->waveCountdown, this->monsterSystem, dt);

	this->particleSystem->update(this->monsterSystem, this->floatingTextSystem, dt);

	this->floatingTextSystem->update(dt);

	this->updateMouseClick();
}
