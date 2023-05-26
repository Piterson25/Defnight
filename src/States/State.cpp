#include "State.hpp"

State::State(float gridSize, sf::RenderWindow &window,
             GameSettings &gameSettings, SoundEngine &soundEngine,
             MusicEngine &musicEngine, std::stack<State *> &states)
    : gridSize(gridSize), window(window), gameSettings(gameSettings),
      soundEngine(soundEngine), musicEngine(musicEngine), states(states)
{
    this->quit = false;
    this->paused = false;
    this->mouseClick = false;
    this->keytime = 0.f;
    this->keytimeMax = 10.f;
    this->reseted = false;
}

State::~State() = default;

const bool State::getReseted() const
{
    return this->reseted;
}

const bool State::getQuit() const
{
    return this->quit;
}

const bool State::getKeytime()
{
    if (this->keytime >= this->keytimeMax) {
        this->keytime = 0.f;
        return true;
    }
    return false;
}

const bool State::getMouseClick() const
{
    return this->mouseClick;
}

const bool State::getKeysClick1(const std::string &keyName)
{
    return this->keysClick[keyName].first;
}

const bool State::getKeysClick2(const std::string &keyName)
{
    return this->keysClick[keyName].second;
}

void State::endState()
{
    this->quit = true;
}

void State::pauseState()
{
    this->paused = true;
}

void State::unpauseState()
{
    this->paused = false;
}

void State::setMouseClick(bool click)
{
    this->mouseClick = click;
}

void State::setKeysClick(const std::string &keyName, bool click)
{
    this->keysClick[keyName].second = click;
}

void State::updateMousePositions(sf::View *view)
{
    this->mousePosScreen = sf::Mouse::getPosition();
    this->mousePosWindow = sf::Mouse::getPosition(this->window);

    if (view) {
        this->window.setView(*view);
    }

    this->mousePosView =
        this->window.mapPixelToCoords(sf::Mouse::getPosition(this->window));

    this->window.setView(this->window.getDefaultView());
}

void State::updateMouseClick()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        this->mouseClick = true;
    }
    else {
        this->mouseClick = false;
    }
}

void State::updateKeysClick(const std::string &keyName, sf::Keyboard::Key key)
{
    this->keysClick[keyName].first = false;

    if (sf::Keyboard::isKeyPressed(key)) {
        this->keysClick[keyName].first = true;
    }
}

void State::updateKeytime(float dt)
{
    if (this->keytime < this->keytimeMax) {
        this->keytime += 100.f * dt;
    }
}
