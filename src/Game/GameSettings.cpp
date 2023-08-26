#include "GameSettings.hpp"

GameSettings::GameSettings()
{
    this->resolution = sf::VideoMode::getDesktopMode();
    this->fullscreen = false;
    this->fpsLimit = 60;
    this->fpsCounterOn = true;
    this->musicVolume = 50;
    this->soundsVolume = 50;
    this->language = "english";
    this->videoModes = sf::VideoMode::getFullscreenModes();
}

GameSettings::~GameSettings()
{
    this->videoModes.clear();
}

void GameSettings::save()
{
    std::ofstream ofs;

    ofs.open("config/game_settings.ini");

    if (ofs.is_open()) {
        ofs << this->resolution.width << " " << this->resolution.height << '\n';
        ofs << this->fullscreen << '\n';
        ofs << this->fpsLimit << '\n';
        ofs << this->fpsCounterOn << '\n';
        ofs << this->musicVolume << '\n';
        ofs << this->soundsVolume << '\n';
        ofs << this->language;
    }
    ofs.close();
}

void GameSettings::load()
{
    std::ifstream ifs;

    ifs.open("config/game_settings.ini");

    if (ifs.is_open()) {
        ifs >> this->resolution.width >> this->resolution.height;
        ifs >> this->fullscreen;
        ifs >> this->fpsLimit;
        ifs >> this->fpsCounterOn;
        ifs >> this->musicVolume;
        ifs >> this->soundsVolume;
        ifs >> this->language;
    }
    ifs.close();

    if (this->language == "polish") {
        ifs.open("assets/languages/polish.ini");
    }
    else {
        ifs.open("assets/languages/english.ini");
    }
    while (!ifs.eof()) {
        std::string key;
        std::string value;
        ifs >> key;
        std::getline(ifs, value);
        value.erase(0, 1);
        lang[key] = sf::String::fromUtf8(value.begin(), value.end());
    }
    ifs.close();

    GameInputHandler::loadKeybinds();
}
