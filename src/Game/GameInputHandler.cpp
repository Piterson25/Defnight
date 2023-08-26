#include "GameInputHandler.hpp"

std::unordered_map<std::string, int> GameInputHandler::keybinds;
std::unordered_map<std::string, int> GameInputHandler::supportedKeys;
std::unordered_map<std::string, std::pair<bool, bool>>
    GameInputHandler::keysClick;
bool GameInputHandler::mouseClick = false;

void GameInputHandler::loadKeybinds()
{
    std::ifstream ifs;

    ifs.open("config/supported_keys.ini");
    if (ifs.is_open()) {
        std::string key = "";
        int key_value = 0;

        while (ifs >> key >> key_value) {
            supportedKeys[key] = key_value;
        }
    }
    ifs.close();

    ifs.open("config/game_keybinds.ini");
    if (ifs.is_open()) {
        std::string key = "";
        std::string key2 = "";
        while (ifs >> key >> key2) {
            keybinds[key] = supportedKeys[key2];
        }
    }
    ifs.close();
}

bool GameInputHandler::isMouseClick()
{
    return mouseClick;
}

void GameInputHandler::setMouseClick(bool click)
{
    mouseClick = click;
}

const bool GameInputHandler::isKeyClicked1(const std::string &keyName)
{
    return keysClick[keyName].first;
}

const bool GameInputHandler::isKeyClicked2(const std::string &keyName)
{
    return keysClick[keyName].second;
}

void GameInputHandler::setKeysClick(const std::string &keyName, bool click)
{
    keysClick[keyName].second = click;
}

void GameInputHandler::updateMouseClick()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        mouseClick = true;
    }
    else {
        mouseClick = false;
    }
}

void GameInputHandler::updateKeysClick(const std::string &keyName,
                                       sf::Keyboard::Key key)
{
    if (sf::Keyboard::isKeyPressed(key)) {
        keysClick[keyName].first = true;
    }
    else {
        keysClick[keyName].first = false;
    }
}

const bool GameInputHandler::isKeyPressed(const std::string &keyName,
                                          sf::Keyboard::Key key)
{
    updateKeysClick(keyName, key);
    const bool hmm = isKeyClicked1(keyName) && !isKeyClicked2(keyName);
    setKeysClick(keyName, isKeyClicked1(keyName));
    return hmm;
}
