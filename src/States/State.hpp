#pragma once

#include "Audio/MusicEngine.hpp"
#include "Audio/SoundEngine.hpp"
#include "Game/GameSettings.hpp"
#include "Utils/Functions.hpp"

class State {
public:
    State(float gridSize, sf::RenderWindow &window, GameSettings &gameSettings,
          SoundEngine &soundEngine, MusicEngine &musicEngine,
          std::stack<State *> &states);
    virtual ~State();

    const bool isReseted() const;
    const bool isQuitted() const;
    const bool isMouseClicked() const;
    const bool isKeyClicked1(const std::string &keyName);
    const bool isKeyClicked2(const std::string &keyName);

    void endState();
    void pauseState();
    void unpauseState();
    void setMouseClick(bool click);
    void setKeysClick(const std::string &keyName, bool click);

    virtual void updateMousePositions(sf::View *view = NULL);
    virtual void updateMouseClick();
    virtual void updateKeysClick(const std::string &keyName,
                                 sf::Keyboard::Key key);
    virtual void updateKeytime(float dt);
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderTarget *target = NULL) = 0;

protected:
    float gridSize;
    sf::RenderWindow &window;
    GameSettings &gameSettings;
    SoundEngine &soundEngine;
    MusicEngine &musicEngine;
    std::stack<State *> &states;

    std::unordered_map<std::string, std::pair<bool, bool>> keysClick;

    float keytime;
    float keytimeMax;
    bool quit;
    bool paused;
    bool mouseClick;
    bool reseted;

    std::unordered_map<std::string, sf::Texture> textures;

    sf::Vector2i mousePosScreen;
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;
};
