#include "StatsState.hpp"

StatsState::StatsState(float gridSize, sf::RenderWindow &window,
                       GameSettings &gameSettings, SoundEngine &soundEngine,
                       MusicEngine &musicEngine, std::stack<State *> &states)
    : State(gridSize, window, gameSettings, soundEngine, musicEngine, states)
{
    const sf::VideoMode vm = this->gameSettings.resolution;

    this->texts["STATISTICS"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["STATISTICS"], calcChar(32, vm), calcX(640, vm),
        calcY(96, vm), sf::Color(255, 255, 255), true);
    this->sprite_buttons["GO_BACK"] = std::make_unique<gui::ButtonSprite>(
        gui::RECT_ARROW, calcX(32, vm), calcY(24, vm), calcX(4, vm), false);
}

StatsState::~StatsState() = default;

void StatsState::update(float dt)
{
    this->updateMousePositions();

    this->sprite_buttons["GO_BACK"]->update(this->mousePosWindow);
    if (this->sprite_buttons["GO_BACK"]->isPressed() &&
        !this->isMouseClicked()) {
        this->setMouseClick(true);
        this->soundEngine.addSound("button");
        this->endState();
    }

    this->updateMouseClick();

    this->updateKeysClick("Escape", sf::Keyboard::Escape);

    if (this->isKeyClicked1("Escape") && !this->isKeyClicked2("Escape")) {
        this->setKeysClick("Escape", true);
        this->endState();
    }
    this->setKeysClick("Escape", this->isKeyClicked1("Escape"));

    this->musicEngine.update();
}

void StatsState::draw(sf::RenderTarget *target)
{
    if (!target) {
        target = &this->window;
    }

    this->texts["STATISTICS"]->draw(*target);
    this->sprite_buttons["GO_BACK"]->draw(*target);
}
