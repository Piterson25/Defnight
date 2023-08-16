#include "SettingsState.hpp"

SettingsState::SettingsState(float gridSize, sf::RenderWindow &window,
                             GameSettings &gameSettings,
                             SoundEngine &soundEngine, MusicEngine &musicEngine,
                             std::stack<State *> &states)
    : State(gridSize, window, gameSettings, soundEngine, musicEngine, states)
{
    this->initGUI();
}

SettingsState::~SettingsState() = default;

void SettingsState::initGUI()
{
    const sf::VideoMode vm = this->gameSettings.resolution;

    this->mode = vm;

    this->id = 0;

    this->keysClick["Escape"].first = false;
    this->keysClick["Escape"].second = false;

    for (const auto &v : sf::VideoMode::getFullscreenModes()) {
        if (static_cast<float>(v.width) / v.height == 16.f / 9.f) {
            this->videoModes.push_back(v);
        }
    }

    this->texts["SETTINGS"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["SETTINGS"], calcChar(32, vm), calcX(640, vm),
        calcY(96, vm), sf::Color(255, 255, 255), true);
    this->sprite_buttons["GO_BACK"] = std::make_unique<gui::ButtonSprite>(
        gui::RECT_ARROW, calcX(32, vm), calcY(24, vm), calcX(4, vm), false);

    for (size_t i = 0; i < this->videoModes.size(); ++i) {
        if (this->videoModes[i] == vm) {
            this->id = i;
            this->text_buttons["RESOLUTION"] =
                std::make_unique<gui::ButtonText>(
                    this->gameSettings.lang["RESOLUTION"] +
                        std::to_string(this->videoModes[this->id].width) + "x" +
                        std::to_string(this->videoModes[this->id].height),
                    calcChar(16, vm), calcX(96, vm), calcY(192, vm),
                    sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
            break;
        }
    }

    if (this->gameSettings.fullscreen) {
        this->fullscreen = true;
        this->text_buttons["FULLSCREEN"] = std::make_unique<gui::ButtonText>(
            this->gameSettings.lang["FULLSCREEN"] +
                this->gameSettings.lang["YES"],
            calcChar(16, vm), calcX(96, vm), calcY(242, vm),
            sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
    }
    else {
        this->fullscreen = false;
        this->text_buttons["FULLSCREEN"] = std::make_unique<gui::ButtonText>(
            this->gameSettings.lang["FULLSCREEN"] +
                this->gameSettings.lang["NO"],
            calcChar(16, vm), calcX(96, vm), calcY(242, vm),
            sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
    }

    this->fpsLimit = this->gameSettings.fpsLimit;

    this->fpsLimits.push_back(60);
    this->fpsLimits.push_back(120);
    this->fpsLimits.push_back(240);
    this->fpsLimits.push_back(0);

    this->fps_id = 0;

    for (size_t i = 0; i < this->fpsLimits.size(); ++i) {
        if (this->fpsLimits[i] == this->gameSettings.fpsLimit) {
            this->fps_id = i;
            if (this->fpsLimits[i] == 0) {
                this->text_buttons["FPS_LIMIT"] =
                    std::make_unique<gui::ButtonText>(

                        this->gameSettings.lang["FPS_LIMIT"] +
                            this->gameSettings.lang["NO_LIMIT"],
                        calcChar(16, vm), calcX(96, vm), calcY(292, vm),
                        sf::Color(255, 255, 255), sf::Color(192, 192, 192),
                        false);
            }
            else {
                this->text_buttons["FPS_LIMIT"] =
                    std::make_unique<gui::ButtonText>(
                        this->gameSettings.lang["FPS_LIMIT"] +
                            std::to_string(this->fpsLimits[this->fps_id]),
                        calcChar(16, vm), calcX(96, vm), calcY(292, vm),
                        sf::Color(255, 255, 255), sf::Color(192, 192, 192),
                        false);
            }
            break;
        }
    }

    this->musicVolume = this->gameSettings.musicVolume;
    this->musicVolume_id = 0;
    for (uint32_t i = 0; i <= 100; i += 10) {
        this->musicVolumes.push_back(i);
    }

    for (size_t i = 0; i < this->musicVolumes.size(); ++i) {
        if (this->musicVolumes[i] == this->musicVolume) {
            this->musicVolume_id = i;
            this->text_buttons["MUSIC"] = std::make_unique<gui::ButtonText>(

                this->gameSettings.lang["MUSIC"] +
                    std::to_string(this->musicVolumes[this->musicVolume_id]) +
                    "%",
                calcChar(16, vm), calcX(96, vm), calcY(342, vm),
                sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
            break;
        }
    }

    this->soundsVolume = this->gameSettings.soundsVolume;
    this->soundsVolume_id = 0;
    for (uint32_t i = 0; i <= 100; i += 10) {
        this->soundsVolumes.push_back(i);
    }

    for (size_t i = 0; i < this->soundsVolumes.size(); ++i) {
        if (this->soundsVolumes[i] == this->soundsVolume) {
            this->soundsVolume_id = i;
            this->text_buttons["SOUNDS"] = std::make_unique<gui::ButtonText>(

                this->gameSettings.lang["SOUNDS"] +
                    std::to_string(this->soundsVolumes[this->soundsVolume_id]) +
                    "%",
                calcChar(16, vm), calcX(96, vm), calcY(392, vm),
                sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
            break;
        }
    }

    if (this->gameSettings.language == "english") {
        this->gameSettings.language = "english";
        this->text_buttons["LANGUAGE"] = std::make_unique<gui::ButtonText>(
            this->gameSettings.lang["LANGUAGE"] +
                this->gameSettings.lang["ENGLISH"],
            calcChar(16, vm), calcX(96, vm), calcY(442, vm),
            sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
    }
    else if (this->gameSettings.language == "polish") {
        this->gameSettings.language = "polish";
        this->text_buttons["LANGUAGE"] = std::make_unique<gui::ButtonText>(
            this->gameSettings.lang["LANGUAGE"] +
                this->gameSettings.lang["POLISH"],
            calcChar(16, vm), calcX(96, vm), calcY(442, vm),
            sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
    }

    if (this->gameSettings.fpsCounterOn) {
        this->fpsCounterOn = true;
        this->text_buttons["FPS_COUNTER"] = std::make_unique<gui::ButtonText>(
            this->gameSettings.lang["FPS_COUNTER"] +
                this->gameSettings.lang["ON"],
            calcChar(16, vm), calcX(96, vm), calcY(492, vm),
            sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
    }
    else {
        this->fpsCounterOn = false;
        this->text_buttons["FPS_COUNTER"] = std::make_unique<gui::ButtonText>(
            this->gameSettings.lang["FPS_COUNTER"] +
                this->gameSettings.lang["OFF"],
            calcChar(16, vm), calcX(96, vm), calcY(492, vm),
            sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
    }

    this->texts["MOVEMENT"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["MOVEMENT"], calcChar(16, vm), calcX(800, vm),
        calcY(192, vm), sf::Color(255, 255, 255), false);
    this->texts["LEFT_CLICK"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["LEFT_CLICK"], calcChar(16, vm), calcX(800, vm),
        calcY(242, vm), sf::Color(255, 255, 255), false);
    this->texts["RIGHT_CLICK"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["RIGHT_CLICK"], calcChar(16, vm),
        calcX(800, vm), calcY(292, vm), sf::Color(255, 255, 255), false);
    this->texts["ESCAPE"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["ESCAPE"], calcChar(16, vm), calcX(800, vm),
        calcY(342, vm), sf::Color(255, 255, 255), false);
    this->texts["SHOP"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["SHOP"], calcChar(16, vm), calcX(800, vm),
        calcY(392, vm), sf::Color(255, 255, 255), false);
    this->texts["RUN"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["RUN"], calcChar(16, vm), calcX(800, vm),
        calcY(442, vm), sf::Color(255, 255, 255), false);
    this->texts["BUY_ABILITY"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["BUY_ABILITY"], calcChar(16, vm),
        calcX(800, vm), calcY(492, vm), sf::Color(255, 255, 255), false);

    this->text_buttons["APPLY"] = std::make_unique<gui::ButtonText>(
        this->gameSettings.lang["APPLY"], calcChar(32, vm), calcX(1000, vm),
        calcY(592, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192),
        false);
    this->text_buttons["APPLY"]->setPosition(
        sf::Vector2f(calcX(1184, vm) - this->text_buttons["APPLY"]->getWidth(),
                     calcY(592, vm)));
}

void SettingsState::resetGUI()
{
    this->texts.clear();
    this->text_buttons.clear();
    this->sprites.clear();
    this->sprite_buttons.clear();

    initGUI();
}

void SettingsState::update(float dt)
{
    this->updateMousePositions();

    this->sprite_buttons["GO_BACK"]->update(this->mousePosWindow);
    if (this->sprite_buttons["GO_BACK"]->isPressed() &&
        !this->isMouseClicked()) {
        this->setMouseClick(true);
        this->soundEngine.addSound("button");
        this->endState();
    }

    this->text_buttons["RESOLUTION"]->update(this->mousePosWindow);
    if (this->text_buttons["RESOLUTION"]->isPressed() &&
        !this->isMouseClicked()) {
        this->setMouseClick(true);
        this->soundEngine.addSound("button");

        ++this->id;
        if (this->id == this->videoModes.size()) {
            this->id = 0;
        }

        this->text_buttons["RESOLUTION"]->setText(
            this->gameSettings.lang["RESOLUTION"] +
            std::to_string(this->videoModes[this->id].width) + "x" +
            std::to_string(this->videoModes[this->id].height));
        this->mode = this->videoModes[this->id];
    }

    this->text_buttons["FULLSCREEN"]->update(this->mousePosWindow);
    if (this->text_buttons["FULLSCREEN"]->isPressed() &&
        !this->isMouseClicked()) {
        this->setMouseClick(true);
        this->soundEngine.addSound("button");

        if (this->fullscreen) {
            this->fullscreen = false;
            this->text_buttons["FULLSCREEN"]->setText(
                this->gameSettings.lang["FULLSCREEN"] +
                this->gameSettings.lang["NO"]);
        }
        else {
            this->fullscreen = true;
            this->text_buttons["FULLSCREEN"]->setText(
                this->gameSettings.lang["FULLSCREEN"] +
                this->gameSettings.lang["YES"]);
        }
    }

    this->text_buttons["FPS_LIMIT"]->update(this->mousePosWindow);
    if (this->text_buttons["FPS_LIMIT"]->isPressed() &&
        !this->isMouseClicked()) {
        this->setMouseClick(true);
        this->soundEngine.addSound("button");

        ++this->fps_id;
        if (this->fps_id == this->fpsLimits.size()) {
            this->fps_id = 0;
        }

        if (this->fpsLimits[fps_id] == 0) {
            this->text_buttons["FPS_LIMIT"]->setText(
                this->gameSettings.lang["FPS_LIMIT"] +
                this->gameSettings.lang["NO_LIMIT"]);
        }
        else {
            this->text_buttons["FPS_LIMIT"]->setText(
                this->gameSettings.lang["FPS_LIMIT"] +
                std::to_string(this->fpsLimits[this->fps_id]));
        }
        this->fpsLimit = this->fpsLimits[this->fps_id];
    }

    this->text_buttons["MUSIC"]->update(this->mousePosWindow);
    if (this->text_buttons["MUSIC"]->isPressed() && !this->isMouseClicked()) {
        this->setMouseClick(true);
        this->soundEngine.addSound("button");

        ++this->musicVolume_id;
        if (this->musicVolume_id == this->musicVolumes.size()) {
            this->musicVolume_id = 0;
        }

        this->text_buttons["MUSIC"]->setText(
            this->gameSettings.lang["MUSIC"] +
            std::to_string(this->musicVolumes[this->musicVolume_id]) + "%");
        this->musicVolume = this->musicVolumes[this->musicVolume_id];
    }

    this->text_buttons["SOUNDS"]->update(this->mousePosWindow);
    if (this->text_buttons["SOUNDS"]->isPressed() && !this->isMouseClicked()) {
        this->setMouseClick(true);
        this->soundEngine.addSound("button");

        ++this->soundsVolume_id;
        if (this->soundsVolume_id == this->soundsVolumes.size()) {
            this->soundsVolume_id = 0;
        }

        this->text_buttons["SOUNDS"]->setText(
            this->gameSettings.lang["SOUNDS"] +
            std::to_string(this->soundsVolumes[this->soundsVolume_id]) + "%");
        this->soundsVolume = this->soundsVolumes[this->soundsVolume_id];
    }

    this->text_buttons["LANGUAGE"]->update(this->mousePosWindow);
    if (this->text_buttons["LANGUAGE"]->isPressed() &&
        !this->isMouseClicked()) {
        this->setMouseClick(true);
        this->soundEngine.addSound("button");

        if (this->gameSettings.language == "english") {
            this->gameSettings.language = "polish";
            this->text_buttons["LANGUAGE"]->setText(
                this->gameSettings.lang["LANGUAGE"] +
                this->gameSettings.lang["POLISH"]);
        }
        else if (this->gameSettings.language == "polish") {
            this->gameSettings.language = "english";
            this->text_buttons["LANGUAGE"]->setText(
                this->gameSettings.lang["LANGUAGE"] +
                this->gameSettings.lang["ENGLISH"]);
        }
    }

    this->text_buttons["FPS_COUNTER"]->update(this->mousePosWindow);
    if (this->text_buttons["FPS_COUNTER"]->isPressed() &&
        !this->isMouseClicked()) {
        this->setMouseClick(true);
        this->soundEngine.addSound("button");

        if (this->fpsCounterOn) {
            this->fpsCounterOn = false;
            this->text_buttons["FPS_COUNTER"]->setText(
                this->gameSettings.lang["FPS_COUNTER"] +
                this->gameSettings.lang["OFF"]);
        }
        else {
            this->fpsCounterOn = true;
            this->text_buttons["FPS_COUNTER"]->setText(
                this->gameSettings.lang["FPS_COUNTER"] +
                this->gameSettings.lang["ON"]);
        }
    }

    this->text_buttons["APPLY"]->update(this->mousePosWindow);
    if (this->text_buttons["APPLY"]->isPressed() && !this->isMouseClicked()) {
        this->setMouseClick(true);
        this->soundEngine.addSound("button");
        this->gameSettings.resolution = this->mode;
        this->gameSettings.fullscreen = this->fullscreen;
        this->gameSettings.fpsLimit = this->fpsLimit;
        this->gameSettings.language = this->gameSettings.language;
        this->gameSettings.musicVolume = this->musicVolume;
        this->gameSettings.soundsVolume = this->soundsVolume;
        this->gameSettings.fpsCounterOn = this->fpsCounterOn;
        this->gameSettings.save();

        this->reseted = true;
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

void SettingsState::draw(sf::RenderTarget *target)
{
    if (!target) {
        target = &this->window;
    }

    this->texts["SETTINGS"]->draw(*target);
    this->sprite_buttons["GO_BACK"]->draw(*target);
    this->text_buttons["RESOLUTION"]->draw(*target);
    this->text_buttons["FULLSCREEN"]->draw(*target);
    this->text_buttons["FPS_LIMIT"]->draw(*target);
    this->text_buttons["MUSIC"]->draw(*target);
    this->text_buttons["SOUNDS"]->draw(*target);
    this->text_buttons["LANGUAGE"]->draw(*target);
    this->text_buttons["FPS_COUNTER"]->draw(*target);
    this->texts["MOVEMENT"]->draw(*target);
    this->texts["LEFT_CLICK"]->draw(*target);
    this->texts["RIGHT_CLICK"]->draw(*target);
    this->texts["ESCAPE"]->draw(*target);
    this->texts["SHOP"]->draw(*target);
    this->texts["RUN"]->draw(*target);
    this->texts["BUY_ABILITY"]->draw(*target);
    this->text_buttons["APPLY"]->draw(*target);
}
