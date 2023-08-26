#pragma once

#include "Utils/Functions.hpp"

class GameInputHandler {
public:
    static void loadKeybinds();

    static std::unordered_map<std::string, int> keybinds;
    static std::unordered_map<std::string, int> supportedKeys;

    static bool isMouseClick();
    static void setMouseClick(bool click);

    static const bool isKeyClicked1(const std::string &keyName);
    static const bool isKeyClicked2(const std::string &keyName);

    static void setKeysClick(const std::string &keyName, bool click);

    static void updateMouseClick();
    static void updateKeysClick(const std::string &keyName,
                                sf::Keyboard::Key key);

    static const bool isKeyPressed(const std::string &keyName,
                                   sf::Keyboard::Key key);

    static std::unordered_map<std::string, std::pair<bool, bool>> keysClick;
    static bool mouseClick;
};
