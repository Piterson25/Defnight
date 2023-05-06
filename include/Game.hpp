#ifndef GAME_HPP
#define GAME_HPP

#include "MainMenuState.hpp"

class Game {
public:
    Game();
    virtual ~Game();

    void init();

    void checkEvents();
    void close();
    void draw();
    void update();
    void run();

private:
    sf::RenderWindow *window;
    GameSettings gameSettings;
    sf::Event sfEvent;
    std::unordered_map<std::string, int> supportedKeys;

    sf::Clock dtClock;
    float dt;

    sf::Font font;
    gui::Text *fpsCounter;
    uint32_t fps;
    float fpsTimer;

    std::stack<State *> states;
    SoundEngine *soundEngine;
    MusicEngine *musicEngine;

    float gridSize;
};

#endif