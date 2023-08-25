#include "GameState.hpp"

GameState::GameState(float gridSize, sf::RenderWindow &window,
                     GameSettings &gameSettings, SoundEngine &soundEngine,
                     MusicEngine &musicEngine, std::stack<State *> &states,
                     const std::string &map_name, const std::string &hero_name,
                     const std::string &difficulty_name)
    : State(gridSize, window, gameSettings, soundEngine, musicEngine, states),
      vm(gameSettings.resolution)
{
    this->musicEngine.clearMusic();
    this->musicEngine.addMusic("battle1.ogg");
    this->musicEngine.addMusic("battle2.ogg");
    this->musicEngine.addMusic("battle3.ogg");
    this->musicEngine.addMusic("battle4.ogg");
    this->musicEngine.addMusic("battle5.ogg");
    this->musicEngine.addMusic("battle6.ogg");

    this->floatingTextSystem = new FloatingTextSystem(this->vm);
    this->tileMap = new TileMap();
    this->mapName = map_name;

    this->background_texture.loadFromFile("assets/textures/maps/" + map_name +
                                          ".png");
    this->background.setTexture(this->background_texture);
    this->background.setScale(calcScale(4, vm), calcScale(4, vm));

    this->tiles_texture.loadFromFile("assets/textures/tiles.png");
    this->vertexArray.setPrimitiveType(sf::Quads);
    this->vertexArray.resize(static_cast<size_t>(calcX(64 * 64 * 4, vm)));
    std::ifstream map("assets/maps/" + map_name + ".txt");
    float x = 0.f, y = 0.f, pos = calcX(this->gridSize, vm);

    float offsetY = 0.f;
    if (map_name == "desolation") {
        offsetY = 48.f;
    }
    else if (map_name == "permafrost") {
        offsetY = 96.f;
    }
    size_t t = 0;
    const sf::Vector2f tile = sf::Vector2f(calcX(64, vm), calcY(64, vm));
    if (map.is_open()) {
        std::string temp;
        while (std::getline(map, temp)) {
            for (size_t i = 0; i < temp.size(); ++i) {
                if (temp[i] == '#') {
                    this->tileMap->addTile("wall", tile, x, y);
                    sf::Vertex *quad = &this->vertexArray[t * 4];

                    quad[0].position = sf::Vector2f(x, y);
                    quad[1].position = sf::Vector2f(x + calcX(64, vm), y);
                    quad[2].position =
                        sf::Vector2f(x + calcX(64, vm), y + calcY(64, vm));
                    quad[3].position = sf::Vector2f(x, y + calcY(64, vm));

                    quad[0].texCoords = sf::Vector2f(48, 16 + offsetY);
                    quad[1].texCoords = sf::Vector2f(64, 16 + offsetY);
                    quad[2].texCoords = sf::Vector2f(64, 32 + offsetY);
                    quad[3].texCoords = sf::Vector2f(48, 32 + offsetY);
                }
                else if (temp[i] == '@') {
                    this->tileMap->addTile("wall", tile, x, y);
                    sf::Vertex *quad = &this->vertexArray[t * 4];

                    quad[0].position = sf::Vector2f(x, y);
                    quad[1].position = sf::Vector2f(x + calcX(64, vm), y);
                    quad[2].position =
                        sf::Vector2f(x + calcX(64, vm), y + calcY(64, vm));
                    quad[3].position = sf::Vector2f(x, y + calcY(64, vm));

                    quad[0].texCoords = sf::Vector2f(64, 16 + offsetY);
                    quad[1].texCoords = sf::Vector2f(80, 16 + offsetY);
                    quad[2].texCoords = sf::Vector2f(80, 32 + offsetY);
                    quad[3].texCoords = sf::Vector2f(64, 32 + offsetY);
                }
                x += pos;
                t++;
            }
            x = 0.f;
            y += pos;
        }
    }
    map.close();

    if (hero_name == "WARRIOR") {
        this->player = new Warrior(
            hero_name, vm,
            this->vertexArray.getBounds().width / 2 - calcX(32, vm),
            this->vertexArray.getBounds().height / 2 - calcY(32, vm));
    }

    this->difficultyName = difficulty_name;
    this->playerGUI =
        new PlayerGUI(this->vm, *this->player, *this->floatingTextSystem,
                      difficulty_name, this->gameSettings.lang);

    this->view.setSize(sf::Vector2f(static_cast<float>(vm.width),
                                    static_cast<float>(vm.height)));
    this->view.setCenter(this->player->getCenter());

    this->viewHUD.setSize(sf::Vector2f(static_cast<float>(vm.width),
                                       static_cast<float>(vm.height)));
    this->viewHUD.setCenter(sf::Vector2f(static_cast<float>(vm.width) / 2.f,
                                         static_cast<float>(vm.height) / 2.f));

    Random::Init();

    this->keysClick["Escape"].first = false;
    this->keysClick["Escape"].second = false;

    float modifier = 1.f;

    if (difficulty_name == "EASY") {
        modifier = 0.75f;
    }
    else if (difficulty_name == "HARD") {
        modifier = 1.25f;
    }

    this->dropSystem = new DropSystem(vm, modifier);

    this->projectileSystem = new ProjectileSystem(vm);
    this->particleSystem = new ParticleSystem(vm);

    this->monsterSystem = new MonsterSystem(
        vm, *player, this->tileMap->getTilesGlobalBounds(), this->gridSize,
        modifier, *this->playerGUI, *this->projectileSystem, *this->dropSystem,
        *this->floatingTextSystem, this->soundEngine);

    this->wave = 0;
    this->sumHP = 15;
    this->waveCountdown = 0.f;
    this->gems = 0;

    this->playerGUI->update_Gold();
}

GameState::~GameState()
{
    this->savePlayerData();
    this->musicEngine.clearMusic();
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
    this->texts.clear();
    this->text_buttons.clear();
    this->sprites.clear();
    this->sprite_buttons.clear();

    initGUI();
}

void GameState::draw(sf::RenderTarget *target)
{
    if (!target) {
        target = &this->window;
    }

    target->setView(this->view);

    target->draw(this->background);

    this->monsterSystem->drawShadow(*target);

    this->player->drawShadow(*target);

    this->particleSystem->drawSmallParticles(*target);

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

void GameState::savePlayerData()
{
    PlayerStats::PlayerData playerData{
        wave,
        player->getXP(),
        player->getLevel(),
        player->getKills(),
        player->getDamageDealt(),
        player->getDamageTaken(),
        static_cast<uint32_t>(player->isDead()),
        player->getGold(),
        player->getBoughtItems(),
        static_cast<uint32_t>(clock.getElapsedTime().asSeconds()) / 60,
        1,
        static_cast<uint32_t>(this->mapName == "ruins"),
        static_cast<uint32_t>(this->mapName == "desolation"),
        static_cast<uint32_t>(this->mapName == "permafrost"),
        static_cast<uint32_t>(this->difficultyName == "EASY"),
        static_cast<uint32_t>(this->difficultyName == "NORMAL"),
        static_cast<uint32_t>(this->difficultyName == "HARD")};
    PlayerStats::saveStats("data/player_stats.dat", playerData);
}

void GameState::update(float dt)
{
    this->updateMousePositions(&this->viewHUD);

    if (this->playerGUI->hasClickedMenu(this->mousePosWindow,
                                        this->isMouseClicked(), this->paused)) {
        this->setKeysClick("Escape", true);
        if (this->paused) {
            this->musicEngine.pauseMusic();
            this->soundEngine.stopSounds();
        }
        else {
            this->musicEngine.playMusic();
        }
    }

    if (this->playerGUI->hasClickedButtons(
            this->mousePosWindow, this->isMouseClicked(), this->soundEngine)) {
        this->setMouseClick(true);
        this->unpauseState();
    }

    if (this->player->isDead()) {
        const uint16_t result = this->playerGUI->updateDeathScreenButtons(
            this->mousePosWindow, this->isMouseClicked());
        if (result == 1) {
            this->setMouseClick(true);
            this->soundEngine.addSound("button");
            this->endState();
        }
        else if (result == 2) {
            this->setMouseClick(true);
            this->soundEngine.addSound("button");
            this->window.close();
        }
    }

    if (!this->playerGUI->isLeveling() && !this->playerGUI->isUpgrading()) {

        this->updateKeysClick("Escape", sf::Keyboard::Escape);

        if (this->isKeyClicked1("Escape") && !this->isKeyClicked2("Escape")) {
            this->setKeysClick("Escape", true);
            this->playerGUI->updatePaused(this->paused);
            if (this->paused) {
                this->musicEngine.pauseMusic();
                this->soundEngine.stopSounds();
            }
            else {
                this->musicEngine.playMusic();
            }
        }
        this->setKeysClick("Escape", this->isKeyClicked1("Escape"));
    }

    if (this->playerGUI->isEscape() &&
        (!this->playerGUI->isLeveling() && !this->playerGUI->isUpgrading())) {
        const uint8_t result = this->playerGUI->updateEscapeButton(
            this->mousePosWindow, this->isMouseClicked());
        if (result == 1) {
            this->soundEngine.addSound("button");
            this->endState();
        }
        else if (result == 2) {
            this->soundEngine.addSound("button");
            this->window.close();
        }
        else if (result == 3) {
            this->paused = false;
            this->musicEngine.playMusic();
            this->soundEngine.playSounds();
            this->soundEngine.addSound("button");
        }
        else if (result == 4) {
            this->soundEngine.addSound("button");
            this->states.push(new SettingsState(
                this->gridSize, this->window, this->gameSettings,
                this->soundEngine, this->musicEngine, this->states));
        }
    }

    if (!this->paused) {
        if (!this->player->hasSpawned()) {
            this->player->spawn(dt);
        }
        else {
            this->player->controls(this->gameSettings.keybinds, dt);
            this->player->updateSprint(dt);

            if (this->player->isPunched()) {
                this->player->smashed(dt);
            }

            if (this->player->hasVelocity()) {
                this->player->obstacleCollision(
                    this->tileMap->getTilesGlobalBounds());

                this->player->move();

                this->player->update(dt);

                const float _32 = calcX(32, this->vm);
                const bool osY =
                    this->player->getPosition().y <= calcY(200, this->vm) ||
                    this->player->getPosition().y >=
                        this->background.getGlobalBounds().height -
                            calcY(392, this->vm);
                const bool osX =
                    this->player->getPosition().x <= calcX(608, this->vm) ||
                    this->player->getPosition().x >=
                        this->background.getGlobalBounds().width -
                            calcX(672, this->vm);

                if (osX || osY) {
                    if (osX && osY) {
                        this->view.setCenter(
                            sf::Vector2f(this->view.getCenter().x,
                                         this->view.getCenter().y));
                    }
                    else {
                        if (osX) {
                            this->view.setCenter(sf::Vector2f(
                                this->view.getCenter().x,
                                this->player->getPosition().y + _32));
                        }
                        else if (osY) {
                            this->view.setCenter(sf::Vector2f(
                                this->player->getPosition().x + _32,
                                view.getCenter().y));
                        }
                    }
                }
                else {
                    this->view.setCenter(this->player->getPosition().x + _32,
                                         this->player->getPosition().y + _32);
                }
            }

            this->player->loadAttack(dt);
            this->player->whooshSound(this->soundEngine);

            if (this->mousePosView.y > calcY(128, this->vm)) {
                if (!this->playerGUI->isShopping() &&
                    !this->playerGUI->isBuyingAbility()) {
                    if (mouseClick) {
                        this->player->doAttack();
                    }
                    if (this->player->isAbilityActivated()) {
                        this->player->doAbility(this->soundEngine);
                        this->projectileSystem->playerAbility(
                            sf::Vector2f(this->window.mapPixelToCoords(
                                sf::Mouse::getPosition(this->window),
                                this->view)),
                            *this->player);
                        this->playerGUI->setAbilityIcon();
                    }
                }
            }
            this->player->spawn(dt);
            this->player->animation(dt);
            this->player->abilityCounter(dt);
            this->playerGUI->updateArmor();
            this->playerGUI->updateAttack();
            this->playerGUI->updateReg();
            this->playerGUI->update_ability(dt);

            this->updateKeysClick("Q", sf::Keyboard::Q);

            if (this->isKeyClicked1("Q") && !this->isKeyClicked2("Q")) {
                this->setKeysClick("Q", true);
                this->playerGUI->updateIsShopping();
            }
            this->setKeysClick("Q", this->isKeyClicked1("Q"));

            if (this->playerGUI->hasClickedShopBuy(this->mousePosWindow,
                                                   this->isMouseClicked(),
                                                   this->soundEngine)) {
                this->setMouseClick(true);
            }

            if (this->player->isUpgraded()) {

                this->updateKeysClick("E", sf::Keyboard::E);

                if (this->isKeyClicked1("E") && !this->isKeyClicked2("E")) {
                    this->setKeysClick("E", true);
                    this->playerGUI->updateIsBuyingAbility();
                }
                this->setKeysClick("E", this->isKeyClicked1("E"));

                if (this->playerGUI->hasClickedAbilityBuy(
                        this->mousePosWindow, this->isMouseClicked(),
                        this->soundEngine)) {
                    this->setMouseClick(true);
                }
            }
        }

        if (this->waveCountdown < 10.f) {
            this->waveCountdown += dt;

            if (this->waveCountdown >= 10.f) {
                this->monsterSystem->spawnMonsters(
                    this->tileMap->getTilesGlobalBounds(), this->wave);
            }
            else if (this->waveCountdown > 8.f &&
                     this->monsterSystem->isMonsterIDsEmpty()) {
                this->monsterSystem->prepareWave(this->wave, this->sumHP);
                this->playerGUI->updateMonsterCountWave(
                    this->gameSettings.language, this->wave,
                    this->monsterSystem->isBossWave(),
                    this->monsterSystem->monsterIDsSize(), this->soundEngine,
                    this->musicEngine);
            }
        }
        else {
            if (this->monsterSystem->isMonstersEmpty()) {
                this->waveCountdown = 0.f;
                this->monsterSystem->monsterIDsClear();
                this->gems += 5;
            }
            else {
                this->monsterSystem->playerAttack();
                this->monsterSystem->update(
                    this->tileMap->getTilesGlobalBounds(), this->paused, dt);
                this->playerGUI->updateBossHP(dt);
            }
        }

        this->projectileSystem->update(
            *this->player, *this->playerGUI, *this->particleSystem,
            *this->monsterSystem, this->background.getGlobalBounds(),
            this->tileMap->getTilesGlobalBounds(), *this->floatingTextSystem,
            this->soundEngine, dt);

        this->dropSystem->update(*this->player, *this->playerGUI,
                                 *this->floatingTextSystem, this->soundEngine,
                                 dt);

        if (this->player->isDead()) {
            this->paused = true;
            this->player->setRegenerating(false);
            this->playerGUI->updateHP();
        }

        this->playerGUI->updatingHP(this->soundEngine, dt);
        this->playerGUI->updatingSprint(dt);

        this->soundEngine.update();
        this->musicEngine.update();
    }

    if (this->player->getPendingXP() > 0) {
        if (!this->playerGUI->isLeveling() && !this->playerGUI->isUpgrading() &&
            this->player->levelUp()) {
            this->player->setLeveling(true);
            this->playerGUI->update_level(soundEngine);
            paused = true;
        }
        this->playerGUI->updateXP();
    }

    this->playerGUI->update(this->mousePosView, this->waveCountdown,
                            this->monsterSystem->bossHP(), dt);

    this->particleSystem->update(dt);
    this->monsterSystem->explosionAttack(
        this->particleSystem->getParticlesGlobalBounds());

    this->particleSystem->clearParticlesGlobalBounds();

    this->floatingTextSystem->update(dt);

    this->updateMouseClick();
}
