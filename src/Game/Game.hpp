#pragma once

#include "States/MainMenuState.hpp"

class Game {
public:
    Game();
    ~Game();

    void createWindow();
    void setIcon();
    void initGUI();
    void checkEvents();
    void close();
    void draw();
    void update();
    void run();

private:
    sf::RenderWindow *window;
    GameSettings *gameSettings;
    sf::Event sfEvent;

    sf::Clock dtClock;
    float dt;

    std::unique_ptr<gui::Text> fpsCounter;
    uint32_t fps;
    float fpsTimer;

    std::stack<State *> states;
    SoundEngine *soundEngine;
    MusicEngine *musicEngine;

    float gridSize;
};
