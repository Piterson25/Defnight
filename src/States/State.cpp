#include "State.hpp"

State::State(float gridSize, sf::RenderWindow &window,
             GameSettings &gameSettings, SoundEngine &soundEngine,
             MusicEngine &musicEngine, std::stack<State *> &states)
    : vm(gameSettings.resolution), lang(gameSettings.lang), gridSize(gridSize),
      window(window), gameSettings(gameSettings), soundEngine(soundEngine),
      musicEngine(musicEngine), states(states)
{
    this->quit = false;
    this->paused = false;
    this->reseted = false;
}

State::~State() = default;

const bool State::isReseted() const
{
    return this->reseted;
}

const bool State::isQuitted() const
{
    return this->quit;
}

void State::unReset()
{
    this->reseted = false;
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
