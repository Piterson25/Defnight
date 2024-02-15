#include "SettingsState.hpp"

SettingsState::SettingsState(float gridSize, sf::RenderWindow &window,
                             GameSettings &gameSettings,
                             SoundEngine &soundEngine, MusicEngine &musicEngine,
                             std::stack<State *> &states)
    : State(gridSize, window, gameSettings, soundEngine, musicEngine, states)
{
    this->page = 0;
    this->id = 0;

    for (int i = sf::VideoMode::getFullscreenModes().size() - 1; i >= 0; --i) {
        const auto &v = sf::VideoMode::getFullscreenModes()[i];
        if (static_cast<float>(v.width) / v.height == 16.f / 9.f) {
            this->videoModes.push_back(v);
        }
    }

    this->fps_id = 0;
    this->fpsLimits.push_back(0);
    this->fpsLimits.push_back(240);
    this->fpsLimits.push_back(120);
    this->fpsLimits.push_back(60);

    this->musicVolume_id = 0;
    for (uint32_t i = 0; i <= 100; i += 10) {
        this->musicVolumes.push_back(i);
    }

    this->soundsVolume_id = 0;
    for (uint32_t i = 0; i <= 100; i += 10) {
        this->soundsVolumes.push_back(i);
    }

    this->initGUI();
}

SettingsState::~SettingsState() = default;

void SettingsState::addSetting(const std::string &t_name, float t_x, float t_y,
                               const std::string &desc,
                               const std::string &change)
{
    this->settings.emplace(
        t_name,
        Setting{
            std::make_unique<gui::ButtonSprite>(
                gui::RECT_SMALL_ARROW, t_x + calcX(416, vm), t_y,
                calcScale(4, vm), gui::GREY, gui::WHITE, false),
            std::make_unique<gui::ButtonSprite>(
                gui::RECT_SMALL_ARROW, t_x + calcX(864, vm), t_y,
                calcScale(4, vm), gui::GREY, gui::WHITE, false),
            std::make_unique<gui::Text>(desc, calcChar(24, vm), t_x,
                                        t_y + calcY(16, vm), gui::WHITE, false),
            std::make_unique<gui::Text>(change, calcChar(24, vm),
                                        t_x + calcX(640, vm),
                                        t_y + calcY(16, vm), gui::WHITE, true),
        });

    this->settings[t_name].rightArrow->flipHorizontal();
}

void SettingsState::addKeybind(const std::string &t_name, float t_x, float t_y,
                               const std::string &desc, const std::string &key)
{
    this->keybindsTexts.emplace(
        t_name,
        Keybind{
            std::make_unique<gui::Text>(desc, calcChar(24, vm), t_x,
                                        t_y + calcY(16, vm), gui::WHITE, false),
            std::make_unique<gui::Text>(key, calcChar(24, vm),
                                        t_x + calcX(640, vm),
                                        t_y + calcY(16, vm), gui::WHITE, false),
        });
}

void SettingsState::initGUI()
{
    this->vm = gameSettings.resolution;

    this->texts["SETTINGS"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["SETTINGS"], calcChar(32, vm), calcX(640, vm),
        calcY(96, vm), gui::WHITE, true);
    this->sprite_buttons["GO_BACK"] = std::make_unique<gui::ButtonSprite>(
        gui::RECT_ARROW, calcX(32, vm), calcY(24, vm), calcX(4, vm), gui::GREY,
        gui::WHITE, false);

    this->text_buttons["GENERAL_SETTINGS"] = std::make_unique<gui::ButtonText>(
        this->gameSettings.lang["GENERAL_SETTINGS"], calcChar(24, vm),
        calcX(640, vm), calcY(300, vm), gui::WHITE, gui::LIGHT_GREY, true);

    this->text_buttons["AUDIO"] = std::make_unique<gui::ButtonText>(
        this->gameSettings.lang["AUDIO"], calcChar(24, vm), calcX(640, vm),
        calcY(372, vm), gui::WHITE, gui::LIGHT_GREY, true);

    this->text_buttons["KEYBINDS"] = std::make_unique<gui::ButtonText>(
        this->gameSettings.lang["KEYBINDS"], calcChar(24, vm), calcX(640, vm),
        calcY(444, vm), gui::WHITE, gui::LIGHT_GREY, true);

    this->mode = vm;

    for (size_t i = 0; i < this->videoModes.size(); ++i) {
        if (this->videoModes[i] == vm) {
            this->id = i;
            addSetting("RESOLUTION", calcX(192, vm), calcY(192, vm),
                       this->lang["RESOLUTION"],
                       std::to_string(this->videoModes[this->id].width) + "x" +
                           std::to_string(this->videoModes[this->id].height));
            break;
        }
    }

    if (this->gameSettings.fullscreen) {
        this->fullscreen = true;
        addSetting("FULLSCREEN", calcX(192, vm), calcY(256, vm),
                   this->lang["FULLSCREEN"], this->lang["YES"]);
    }
    else {
        this->fullscreen = false;
        addSetting("FULLSCREEN", calcX(192, vm), calcY(256, vm),
                   this->lang["FULLSCREEN"], this->lang["NO"]);
    }

    this->fpsLimit = this->gameSettings.fpsLimit;

    for (size_t i = 0; i < this->fpsLimits.size(); ++i) {
        if (this->fpsLimits[i] == this->gameSettings.fpsLimit) {
            this->fps_id = i;
            if (this->fpsLimits[i] == 0) {
                addSetting("FPS_LIMIT", calcX(192, vm), calcY(320, vm),
                           this->lang["FPS_LIMIT"], this->lang["NO_LIMIT"]);
            }
            else {
                addSetting("FPS_LIMIT", calcX(192, vm), calcY(320, vm),
                           this->lang["FPS_LIMIT"],
                           std::to_string(this->fpsLimits[this->fps_id]));
            }
            break;
        }
    }

    if (this->gameSettings.fpsCounterOn) {
        this->fpsCounterOn = true;
        addSetting("FPS_COUNTER", calcX(192, vm), calcY(384, vm),
                   this->lang["FPS_COUNTER"], this->lang["ON"]);
    }
    else {
        this->fpsCounterOn = false;
        addSetting("FPS_COUNTER", calcX(192, vm), calcY(384, vm),
                   this->lang["FPS_COUNTER"], this->lang["OFF"]);
    }

    if (this->gameSettings.language == "english") {
        this->gameSettings.language = "english";
        addSetting("LANGUAGE", calcX(192, vm), calcY(448, vm),
                   this->lang["LANGUAGE"], this->lang["ENGLISH"]);
    }
    else if (this->gameSettings.language == "polish") {
        this->gameSettings.language = "polish";
        addSetting("LANGUAGE", calcX(192, vm), calcY(448, vm),
                   this->lang["LANGUAGE"], this->lang["POLISH"]);
    }

    this->musicVolume = this->gameSettings.musicVolume;

    for (size_t i = 0; i < this->musicVolumes.size(); ++i) {
        if (this->musicVolumes[i] == this->musicVolume) {
            this->musicVolume_id = i;
            addSetting(
                "MUSIC", calcX(192, vm), calcY(256, vm), this->lang["MUSIC"],
                std::to_string(this->musicVolumes[this->musicVolume_id]) + "%");
            break;
        }
    }

    this->soundsVolume = this->gameSettings.soundsVolume;

    for (size_t i = 0; i < this->soundsVolumes.size(); ++i) {
        if (this->soundsVolumes[i] == this->soundsVolume) {
            this->soundsVolume_id = i;
            addSetting(
                "SOUNDS", calcX(192, vm), calcY(320, vm), this->lang["SOUNDS"],
                std::to_string(this->soundsVolumes[this->soundsVolume_id]) +
                    "%");
            break;
        }
    }

    addKeybind("MOVEMENT", calcX(256, vm), calcY(192, vm),
               this->lang["MOVEMENT"], "WSAD");
    addKeybind("LEFT_CLICK", calcX(256, vm), calcY(240, vm),
               this->lang["LEFT_CLICK"], "LMB");
    addKeybind("RIGHT_CLICK", calcX(256, vm), calcY(288, vm),
               this->lang["RIGHT_CLICK"], "RMB");
    addKeybind("RUN", calcX(256, vm), calcY(336, vm), this->lang["RUN"],
               "LShift");
    addKeybind("ESCAPE", calcX(256, vm), calcY(384, vm), this->lang["ESCAPE"],
               "Esc");
    addKeybind("SHOP", calcX(256, vm), calcY(432, vm), this->lang["SHOP"], "Q");
    addKeybind("BUY_ABILITY", calcX(256, vm), calcY(480, vm),
               this->lang["BUY_ABILITY"], "E");

    this->text_buttons["APPLY"] = std::make_unique<gui::ButtonText>(
        this->gameSettings.lang["APPLY"], calcChar(32, vm), calcX(640, vm),
        calcY(576, vm), gui::WHITE, gui::LIGHT_GREY, true);
}

void SettingsState::resetGUI()
{
    vm = gameSettings.resolution;

    std::cout << vm.width << vm.height;

    this->texts["SETTINGS"]->setPosition(calcX(640, vm), calcY(96, vm));
    this->sprite_buttons["GO_BACK"]->setPosition(calcX(32, vm), calcY(24, vm));

    this->text_buttons["GENERAL_SETTINGS"]->setPosition(calcX(640, vm),
                                                        calcY(300, vm));
    this->text_buttons["AUDIO"]->setPosition(calcX(640, vm), calcY(372, vm));
    this->text_buttons["KEYBINDS"]->setPosition(calcX(640, vm), calcY(444, vm));
}

void SettingsState::update(float dt)
{
    this->updateMousePositions();

    if (this->sprite_buttons["GO_BACK"]->isPressed(this->mousePosWindow)) {
        this->soundEngine.addSound("button");
        if (page == 0) {
            this->endState();
        }
        else {
            this->page = 0;
        }
    }

    switch (page) {
        case 0:
            if (this->text_buttons["GENERAL_SETTINGS"]->isPressed(
                    this->mousePosWindow)) {
                this->soundEngine.addSound("button");
                this->page = 1;
            }
            else if (this->text_buttons["AUDIO"]->isPressed(
                         this->mousePosWindow)) {
                this->soundEngine.addSound("button");
                this->page = 2;
            }
            else if (this->text_buttons["KEYBINDS"]->isPressed(
                         this->mousePosWindow)) {
                this->soundEngine.addSound("button");
                this->page = 3;
            }
            break;
        case 1:
            if (this->settings["RESOLUTION"].leftArrow->isPressed(
                    this->mousePosWindow)) {
                this->soundEngine.addSound("button");
                --this->id;
                if (this->id < 0) {
                    this->id = this->videoModes.size() - 1;
                }

                this->settings["RESOLUTION"].change->setText(
                    std::to_string(this->videoModes[this->id].width) + "x" +
                    std::to_string(this->videoModes[this->id].height));
                this->settings["RESOLUTION"].change->center(
                    this->settings["RESOLUTION"].desc->getPosition().x +
                    calcX(640, vm));
                this->mode = this->videoModes[this->id];
            }
            else if (this->settings["RESOLUTION"].rightArrow->isPressed(
                         this->mousePosWindow)) {
                this->soundEngine.addSound("button");

                ++this->id;
                if (this->id == this->videoModes.size()) {
                    this->id = 0;
                }

                this->settings["RESOLUTION"].change->setText(
                    std::to_string(this->videoModes[this->id].width) + "x" +
                    std::to_string(this->videoModes[this->id].height));
                this->settings["RESOLUTION"].change->center(
                    this->settings["RESOLUTION"].desc->getPosition().x +
                    calcX(640, vm));
                this->mode = this->videoModes[this->id];
            }

            if (this->settings["FULLSCREEN"].leftArrow->isPressed(
                    this->mousePosWindow) ||
                this->settings["FULLSCREEN"].rightArrow->isPressed(
                    this->mousePosWindow)) {
                this->soundEngine.addSound("button");
                if (this->fullscreen) {
                    this->fullscreen = false;
                    this->settings["FULLSCREEN"].change->setText(
                        this->gameSettings.lang["NO"]);
                }
                else {
                    this->fullscreen = true;
                    this->settings["FULLSCREEN"].change->setText(
                        this->gameSettings.lang["YES"]);
                }

                this->settings["FULLSCREEN"].change->center(
                    this->settings["FULLSCREEN"].desc->getPosition().x +
                    calcX(640, vm));
            }

            if (this->settings["FPS_LIMIT"].leftArrow->isPressed(
                    this->mousePosWindow)) {
                this->soundEngine.addSound("button");

                ++this->fps_id;
                if (this->fps_id == this->fpsLimits.size()) {
                    this->fps_id = 0;
                }

                if (this->fpsLimits[fps_id] == 0) {
                    this->settings["FPS_LIMIT"].change->setText(
                        this->gameSettings.lang["NO_LIMIT"]);
                }
                else {
                    this->settings["FPS_LIMIT"].change->setText(
                        std::to_string(this->fpsLimits[this->fps_id]));
                }

                this->settings["FPS_LIMIT"].change->center(
                    this->settings["FPS_LIMIT"].desc->getPosition().x +
                    calcX(640, vm));
                this->fpsLimit = this->fpsLimits[this->fps_id];
            }
            else if (this->settings["FPS_LIMIT"].rightArrow->isPressed(
                         this->mousePosWindow)) {
                --this->fps_id;
                if (this->fps_id < 0) {
                    this->fps_id = this->fpsLimits.size() - 1;
                }

                if (this->fpsLimits[fps_id] == 0) {
                    this->settings["FPS_LIMIT"].change->setText(
                        this->gameSettings.lang["NO_LIMIT"]);
                }
                else {
                    this->settings["FPS_LIMIT"].change->setText(
                        std::to_string(this->fpsLimits[this->fps_id]));
                }

                this->settings["FPS_LIMIT"].change->center(
                    this->settings["FPS_LIMIT"].desc->getPosition().x +
                    calcX(640, vm));
                this->fpsLimit = this->fpsLimits[this->fps_id];
            }

            if (this->settings["LANGUAGE"].leftArrow->isPressed(
                    this->mousePosWindow)) {
                this->soundEngine.addSound("button");
                if (this->gameSettings.language == "english") {
                    this->gameSettings.language = "polish";
                    this->settings["LANGUAGE"].change->setText(
                        this->gameSettings.lang["POLISH"]);
                }
                else if (this->gameSettings.language == "polish") {
                    this->gameSettings.language = "english";
                    this->settings["LANGUAGE"].change->setText(
                        this->gameSettings.lang["ENGLISH"]);
                }
                this->settings["LANGUAGE"].change->center(
                    this->settings["LANGUAGE"].desc->getPosition().x +
                    calcX(640, vm));
            }
            else if (this->settings["LANGUAGE"].rightArrow->isPressed(
                         this->mousePosWindow)) {
                this->soundEngine.addSound("button");
                if (this->gameSettings.language == "english") {
                    this->gameSettings.language = "polish";
                    this->settings["LANGUAGE"].change->setText(
                        this->lang["POLISH"]);
                }
                else if (this->gameSettings.language == "polish") {
                    this->gameSettings.language = "english";
                    this->settings["LANGUAGE"].change->setText(
                        this->lang["ENGLISH"]);
                }
                this->settings["LANGUAGE"].change->center(
                    this->settings["LANGUAGE"].desc->getPosition().x +
                    calcX(640, vm));
            }

            if (this->settings["FPS_COUNTER"].leftArrow->isPressed(
                    this->mousePosWindow)) {
                this->soundEngine.addSound("button");

                if (this->fpsCounterOn) {
                    this->fpsCounterOn = false;
                    this->settings["FPS_COUNTER"].change->setText(
                        this->lang["OFF"]);
                }
                else {
                    this->fpsCounterOn = true;
                    this->settings["FPS_COUNTER"].change->setText(
                        this->lang["ON"]);
                }
                this->settings["FPS_COUNTER"].change->center(
                    this->settings["FPS_COUNTER"].desc->getPosition().x +
                    calcX(640, vm));
            }
            else if (this->settings["FPS_COUNTER"].rightArrow->isPressed(
                         this->mousePosWindow)) {
                this->soundEngine.addSound("button");

                if (this->fpsCounterOn) {
                    this->fpsCounterOn = false;
                    this->settings["FPS_COUNTER"].change->setText(
                        this->lang["OFF"]);
                }
                else {
                    this->fpsCounterOn = true;
                    this->settings["FPS_COUNTER"].change->setText(
                        this->lang["ON"]);
                }
                this->settings["FPS_COUNTER"].change->center(
                    this->settings["FPS_COUNTER"].desc->getPosition().x +
                    calcX(640, vm));
            }

            if (this->text_buttons["APPLY"]->isPressed(this->mousePosWindow)) {
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
            break;
        case 2:
            if (this->settings["MUSIC"].leftArrow->isPressed(
                    this->mousePosWindow)) {
                this->soundEngine.addSound("button");

                --this->musicVolume_id;
                if (this->musicVolume_id < 0) {
                    this->musicVolume_id = this->musicVolumes.size() - 1;
                }

                this->settings["MUSIC"].change->setText(
                    std::to_string(this->musicVolumes[this->musicVolume_id]) +
                    "%");
                this->settings["MUSIC"].change->center(
                    this->settings["MUSIC"].desc->getPosition().x +
                    calcX(640, vm));
                this->musicVolume = this->musicVolumes[this->musicVolume_id];

                this->musicEngine.setVolume(this->musicVolume);
                this->gameSettings.musicVolume = this->musicVolume;
                this->gameSettings.save();
            }
            else if (this->settings["MUSIC"].rightArrow->isPressed(
                         this->mousePosWindow)) {
                this->soundEngine.addSound("button");

                ++this->musicVolume_id;
                if (this->musicVolume_id == this->musicVolumes.size()) {
                    this->musicVolume_id = 0;
                }

                this->settings["MUSIC"].change->setText(
                    std::to_string(this->musicVolumes[this->musicVolume_id]) +
                    "%");
                this->settings["MUSIC"].change->center(
                    this->settings["MUSIC"].desc->getPosition().x +
                    calcX(640, vm));
                this->musicVolume = this->musicVolumes[this->musicVolume_id];

                this->musicEngine.setVolume(this->musicVolume);
                this->gameSettings.musicVolume = this->musicVolume;
                this->gameSettings.save();
            }

            if (this->settings["SOUNDS"].leftArrow->isPressed(
                    this->mousePosWindow)) {
                this->soundEngine.addSound("button");

                --this->soundsVolume_id;
                if (this->soundsVolume_id < 0) {
                    this->soundsVolume_id = this->soundsVolumes.size() - 1;
                }

                this->settings["SOUNDS"].change->setText(
                    std::to_string(this->soundsVolumes[this->soundsVolume_id]) +
                    "%");
                this->settings["SOUNDS"].change->center(
                    this->settings["SOUNDS"].desc->getPosition().x +
                    calcX(640, vm));
                this->soundsVolume = this->soundsVolumes[this->soundsVolume_id];

                this->soundEngine.setVolume(this->soundsVolume);
                this->gameSettings.soundsVolume = this->soundsVolume;
                this->gameSettings.save();
            }
            else if (this->settings["SOUNDS"].rightArrow->isPressed(
                         this->mousePosWindow)) {
                this->soundEngine.addSound("button");

                ++this->soundsVolume_id;
                if (this->soundsVolume_id == this->soundsVolumes.size()) {
                    this->soundsVolume_id = 0;
                }

                this->settings["SOUNDS"].change->setText(
                    std::to_string(this->soundsVolumes[this->soundsVolume_id]) +
                    "%");
                this->settings["SOUNDS"].change->center(
                    this->settings["SOUNDS"].desc->getPosition().x +
                    calcX(640, vm));
                this->soundsVolume = this->soundsVolumes[this->soundsVolume_id];

                this->soundEngine.setVolume(this->soundsVolume);
                this->gameSettings.soundsVolume = this->soundsVolume;
                this->gameSettings.save();
            }

            break;
    }

    GameInputHandler::updateMouseClick();

    if (GameInputHandler::isKeyPressed("Escape", sf::Keyboard::Escape)) {
        if (page == 0) {
            this->endState();
        }
        else {
            this->page = 0;
        }
    }

    this->musicEngine.update();
}

void SettingsState::draw(sf::RenderTarget *target)
{
    if (!target) {
        target = &this->window;
    }

    this->texts["SETTINGS"]->draw(*target);
    this->sprite_buttons["GO_BACK"]->draw(*target);

    switch (page) {
        case 0:
            this->text_buttons["GENERAL_SETTINGS"]->draw(*target);
            this->text_buttons["AUDIO"]->draw(*target);
            this->text_buttons["KEYBINDS"]->draw(*target);
            break;
        case 1:
            this->settings["RESOLUTION"].draw(*target);
            this->settings["FULLSCREEN"].draw(*target);
            this->settings["FPS_LIMIT"].draw(*target);
            this->settings["LANGUAGE"].draw(*target);
            this->settings["FPS_COUNTER"].draw(*target);
            this->settings["RESOLUTION"].draw(*target);
            this->text_buttons["APPLY"]->draw(*target);
            break;
        case 2:
            this->settings["MUSIC"].draw(*target);
            this->settings["SOUNDS"].draw(*target);
            break;
        case 3:
            this->keybindsTexts["MOVEMENT"].draw(*target);
            this->keybindsTexts["LEFT_CLICK"].draw(*target);
            this->keybindsTexts["RIGHT_CLICK"].draw(*target);
            this->keybindsTexts["RUN"].draw(*target);
            this->keybindsTexts["ESCAPE"].draw(*target);
            this->keybindsTexts["SHOP"].draw(*target);
            this->keybindsTexts["BUY_ABILITY"].draw(*target);
            break;
    }
}
