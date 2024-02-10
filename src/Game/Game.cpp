#include "Game.hpp"

Game::Game()
{
    this->window = NULL;
    this->dt = 0.f;
    this->gridSize = 64.f;
    this->sfEvent.type = sf::Event::GainedFocus;
    this->fps = 0;
    this->fpsTimer = 0.f;

    this->gameSettings = new GameSettings();
    this->gameSettings->load();
    this->createWindow();
    this->setIcon();
    this->initGUI();
    this->soundEngine = new SoundEngine(this->gameSettings->soundsVolume);
    this->musicEngine = new MusicEngine(this->gameSettings->musicVolume);

    this->states.push(new MainMenuState(this->gridSize, *this->window,
                                        *this->gameSettings, *this->soundEngine,
                                        *this->musicEngine, this->states));
}

Game::~Game()
{
    while (!this->states.empty()) {
        delete this->states.top();
        this->states.pop();
    }

    delete this->window;
    delete this->gameSettings;
    delete this->soundEngine;
    delete this->musicEngine;
}

void Game::createWindow()
{
    if (this->gameSettings->fullscreen) {
        this->window = new sf::RenderWindow(this->gameSettings->resolution,
                                            "Defnight", sf::Style::Fullscreen);
    }
    else {
        this->window = new sf::RenderWindow(this->gameSettings->resolution,
                                            "Defnight", sf::Style::Close);
    }

    this->window->setFramerateLimit(this->gameSettings->fpsLimit);
}

void Game::setIcon()
{
    sf::Image icon;
    icon.loadFromFile("assets/textures/icon.png");
    this->window->setIcon(32, 32, icon.getPixelsPtr());
}

void Game::initGUI()
{
    const sf::VideoMode vm = this->gameSettings->resolution;

    gui::initVM(vm);
    gui::initFont();
    gui::initTextures();
    this->fpsCounter = std::make_unique<gui::Text>(
        "", calcChar(16, vm), calcX(1150, vm), calcY(4, vm), gui::WHITE, false);
}

void Game::checkEvents()
{
    while (this->window->pollEvent(this->sfEvent)) {
        if (this->sfEvent.type == sf::Event::Closed) {
            this->close();
        }
    }
}

void Game::close()
{
    this->window->close();
}

void Game::draw()
{
    this->window->clear();
    if (!this->states.empty()) {
        this->states.top()->draw();
    }
    if (this->gameSettings->fpsCounterOn) {
        this->fpsCounter->draw(*this->window);
    }
    this->window->display();
}

void Game::update()
{
    this->dt = this->dtClock.restart().asSeconds();

    if (this->gameSettings->fpsCounterOn) {
        ++this->fps;
        this->fpsTimer += dt;

        if (this->fpsTimer >= 1.f) {
            this->fpsCounter->setText(std::to_string(this->fps) + " FPS");
            this->fpsCounter->setPositionX(
                calcX(1276, this->gameSettings->resolution) -
                this->fpsCounter->getWidth());
            this->fps = 0;
            this->fpsTimer = 0.f;
        }
    }

    this->checkEvents();
    if (this->states.empty()) {
        this->close();
        return;
    }

    if (!this->window->hasFocus()) {
        return;
    }

    auto currentState = this->states.top();

    currentState->update(this->dt);

    if (currentState->isQuitted()) {
        delete currentState;
        this->states.pop();
    }
    else if (currentState->isReseted()) {
        while (!this->states.empty()) {
            delete this->states.top();
            this->states.pop();
        }
        this->close();
        this->gameSettings->load();
        this->createWindow();
        this->setIcon();
        this->initGUI();
        this->musicEngine->setVolume(this->gameSettings->musicVolume);
        this->soundEngine->setVolume(this->gameSettings->soundsVolume);

        this->states.push(new MainMenuState(
            this->gridSize, *this->window, *this->gameSettings,
            *this->soundEngine, *this->musicEngine, this->states));
    }
}

void Game::run()
{
    while (this->window->isOpen()) {
        this->update();
        this->draw();
    }
}
