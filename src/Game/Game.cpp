#include "Game.hpp"

Game::Game()
{
    init();
}

Game::~Game()
{
    while (!this->states.empty()) {
        delete this->states.top();
        this->states.pop();
    }

    delete this->window;
    delete this->fpsCounter;
    delete this->gameSettings;
    delete this->soundEngine;
    delete this->musicEngine;
}

void Game::init()
{
    this->window = NULL;
    this->dt = 0.f;
    this->gridSize = 64.f;
    this->sfEvent.type = sf::Event::GainedFocus;

    this->gameSettings = new GameSettings();
    this->gameSettings->load();

    if (!this->font.loadFromFile("assets/font/PressStart2P-vaV7.ttf")) {
        throw std::runtime_error("ERROR - COULDN'T FIND FONT");
    }

    if (this->gameSettings->fullscreen) {
        this->window = new sf::RenderWindow(this->gameSettings->resolution,
                                            "Defnight", sf::Style::Fullscreen);
    }
    else {
        this->window =
            new sf::RenderWindow(this->gameSettings->resolution, "Defnight",
                                 sf::Style::Titlebar | sf::Style::Close);
    }

    this->window->setFramerateLimit(this->gameSettings->fpsLimit);

    sf::Image icon;
    icon.loadFromFile("assets/textures/icon.png");
    this->window->setIcon(32, 32, icon.getPixelsPtr());

    gui::initVM(this->gameSettings->resolution);
    gui::initFont();

    const sf::VideoMode vm = this->gameSettings->resolution;

    this->fps = 0;
    this->fpsTimer = 0.f;
    this->fpsCounter =
        new gui::Text("", calcChar(16, vm), calcX(1150, vm), calcY(4, vm),
                      sf::Color(255, 255, 255), false);

    this->soundEngine = new SoundEngine(this->gameSettings->soundsVolume);
    this->musicEngine = new MusicEngine(this->gameSettings->musicVolume);

    this->states.push(new MainMenuState(this->gridSize, *this->window,
                                        *this->gameSettings, *this->soundEngine,
                                        *this->musicEngine, this->states));
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
        this->musicEngine->stopMusic();
        this->close();
        this->init();
    }
}

void Game::run()
{
    while (this->window->isOpen()) {
        this->update();
        this->draw();
    }
}
