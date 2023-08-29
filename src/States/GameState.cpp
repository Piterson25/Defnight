#include "GameState.hpp"

GameState::GameState(float gridSize, sf::RenderWindow &window,
                     GameSettings &gameSettings, SoundEngine &soundEngine,
                     MusicEngine &musicEngine, std::stack<State *> &states,
                     const std::string &mapName, const std::string &playerName,
                     const std::string &difficultyName)
    : State(gridSize, window, gameSettings, soundEngine, musicEngine, states),
      mapName(mapName), difficultyName(difficultyName)
{
    this->musicEngine.clearMusic();
    this->musicEngine.addMusic("battle1.ogg");
    this->musicEngine.addMusic("battle2.ogg");
    this->musicEngine.addMusic("battle3.ogg");
    this->musicEngine.addMusic("battle4.ogg");
    this->musicEngine.addMusic("battle5.ogg");
    this->musicEngine.addMusic("battle6.ogg");

    this->floatingTextSystem = new FloatingTextSystem(this->vm);
    this->tileMap = new TileMap(vm, mapName);

    if (playerName == "WARRIOR") {
        this->player = new Warrior(
            playerName, vm, this->tileMap->getMapSize().x / 2 - calcX(32, vm),
            this->tileMap->getMapSize().y / 2 - calcY(32, vm));
    }

    this->playerGUI =
        new PlayerGUI(this->vm, *this->player, *this->floatingTextSystem,
                      difficultyName, this->gameSettings.lang);

    this->view.setSize(sf::Vector2f(static_cast<float>(vm.width),
                                    static_cast<float>(vm.height)));
    this->view.setCenter(this->player->getCenter());

    this->viewHUD.setSize(sf::Vector2f(static_cast<float>(vm.width),
                                       static_cast<float>(vm.height)));
    this->viewHUD.setCenter(sf::Vector2f(static_cast<float>(vm.width) / 2.f,
                                         static_cast<float>(vm.height) / 2.f));

    Random::Init();

    float modifier = 1.f;

    if (difficultyName == "EASY") {
        modifier = 0.75f;
    }
    else if (difficultyName == "HARD") {
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

    this->tileMap->drawMap(*target);

    this->monsterSystem->drawShadow(*target);

    this->player->drawShadow(*target);

    this->particleSystem->drawSmallParticles(*target);

    this->tileMap->drawObstacles(*target);

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

    if (this->playerGUI->hasClickedMenu(this->mousePosWindow, this->paused)) {
        if (this->paused) {
            this->musicEngine.pauseMusic();
            this->soundEngine.stopSounds();
        }
        else {
            this->musicEngine.playMusic();
        }
    }

    if (this->playerGUI->hasClickedButtons(this->mousePosWindow,
                                           this->soundEngine)) {
        this->unpauseState();
    }

    if (this->player->isDead()) {
        const uint16_t result =
            this->playerGUI->updateDeathScreenButtons(this->mousePosWindow);
        if (result == 1) {
            this->soundEngine.addSound("button");
            this->endState();
        }
        else if (result == 2) {
            this->soundEngine.addSound("button");
            this->window.close();
        }
    }

    if (!this->playerGUI->isLeveling() && !this->playerGUI->isUpgrading()) {

        if (GameInputHandler::isKeyPressed("Escape", sf::Keyboard::Escape)) {
            this->playerGUI->updatePaused(this->paused);
            if (this->paused) {
                this->musicEngine.pauseMusic();
                this->soundEngine.stopSounds();
            }
            else {
                this->musicEngine.playMusic();
            }
        }
    }

    if (this->playerGUI->isEscape() &&
        (!this->playerGUI->isLeveling() && !this->playerGUI->isUpgrading())) {
        const uint8_t result =
            this->playerGUI->updateEscapeButton(this->mousePosWindow);
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
            this->player->controls(GameInputHandler::keybinds, dt);
            if (this->player->isSprinting() &&
                this->player->isParticleCooldown(dt)) {
                this->particleSystem->addSmallParticle(
                    this->player->getDownCenter(),
                    sf::Vector2f(calcX(8, vm), calcY(8, vm)), gui::WHITE);
            }

            if (this->player->isPunched()) {
                this->player->smashed(dt);
            }

            if (this->player->hasVelocity()) {
                this->player->obstacleCollision(
                    this->tileMap->getTilesGlobalBounds());

                this->player->move();

                this->player->update(dt);

                const bool osY =
                    this->player->getPosition().y <= calcY(200, this->vm) ||
                    this->player->getPosition().y >=
                        this->tileMap->getMapSize().y - calcY(392, this->vm);
                const bool osX =
                    this->player->getPosition().x <= calcX(608, this->vm) ||
                    this->player->getPosition().x >=
                        this->tileMap->getMapSize().x - calcX(672, this->vm);

                if (osX && osY) {
                    this->view.setCenter(this->view.getCenter());
                }
                else if (osX) {
                    this->view.setCenter(sf::Vector2f(
                        this->view.getCenter().x, this->player->getCenter().y));
                }
                else if (osY) {
                    this->view.setCenter(sf::Vector2f(
                        this->player->getCenter().x, view.getCenter().y));
                }
                else {
                    this->view.setCenter(this->player->getCenter());
                }
            }

            this->player->loadAttack(dt);
            this->player->whooshSound(this->soundEngine);

            if (this->mousePosView.y > calcY(128, this->vm)) {
                if (!this->playerGUI->isShopping() &&
                    !this->playerGUI->isBuyingAbility()) {
                    if (GameInputHandler::isMouseClick()) {
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

            if (GameInputHandler::isKeyPressed("Q", sf::Keyboard::Q)) {
                this->playerGUI->updateIsShopping();
            }

            playerGUI->updateShopBuy(this->mousePosWindow, this->soundEngine);

            if (this->player->isUpgraded()) {

                if (GameInputHandler::isKeyPressed("E", sf::Keyboard::E)) {
                    this->playerGUI->updateIsBuyingAbility();
                }

                playerGUI->updateAbilityBuy(this->mousePosWindow,
                                            this->soundEngine);
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
            *this->monsterSystem, this->tileMap->getBounds(),
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

    GameInputHandler::updateMouseClick();
}
