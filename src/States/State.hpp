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

    virtual void initGUI() = 0;
    virtual void resetGUI() = 0;

    const bool isReseted() const;
    const bool isQuitted() const;

    void unReset();
    void endState();
    void pauseState();
    void unpauseState();

    virtual void updateMousePositions(sf::View *view = NULL);
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderTarget *target = NULL) = 0;

protected:
    sf::VideoMode &vm;
    std::unordered_map<std::string, std::string> &lang;
    float gridSize;
    sf::RenderWindow &window;
    GameSettings &gameSettings;
    SoundEngine &soundEngine;
    MusicEngine &musicEngine;
    std::stack<State *> &states;

    bool quit;
    bool paused;
    bool reseted;

    std::unordered_map<std::string, sf::Texture> textures;

    sf::Vector2i mousePosScreen;
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;
};
