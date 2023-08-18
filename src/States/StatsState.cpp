#include "StatsState.hpp"

StatsState::StatsState(float gridSize, sf::RenderWindow &window,
                       GameSettings &gameSettings, SoundEngine &soundEngine,
                       MusicEngine &musicEngine, std::stack<State *> &states)
    : State(gridSize, window, gameSettings, soundEngine, musicEngine, states),
      vm(gameSettings.resolution)
{
    this->texts["STATISTICS"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["STATISTICS"], calcChar(32, vm), calcX(640, vm),
        calcY(96, vm), sf::Color(255, 255, 255), true);
    this->sprite_buttons["GO_BACK"] = std::make_unique<gui::ButtonSprite>(
        gui::RECT_ARROW, calcX(32, vm), calcY(24, vm), calcX(4, vm), false);

    PlayerStats::PlayerData playerData{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    PlayerStats::loadStats("data/player_stats.dat", playerData);

    this->texts["RECORD_WAVE"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["RECORD_WAVE"] +
            std::to_string(playerData.wave),
        calcChar(24, vm), calcX(640, vm), calcY(192, vm), gui::FLAMINGO, true);

    this->texts["TOTAL_XP"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["TOTAL_XP"] + std::to_string(playerData.xp),
        calcChar(16, vm), calcX(192, vm), calcY(292, vm), gui::LIME, false);

    this->texts["RECORD_LEVEL"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["RECORD_LEVEL"] +
            std::to_string(playerData.maxLevel),
        calcChar(16, vm), calcX(192, vm), calcY(342, vm), gui::LIME, false);

    this->texts["TOTAL_KILLS"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["TOTAL_KILLS"] +
            std::to_string(playerData.kills),
        calcChar(16, vm), calcX(192, vm), calcY(392, vm), gui::RED, false);

    this->texts["DAMAGE_DEALT"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["DAMAGE_DEALT"] +
            std::to_string(playerData.damageDealt),
        calcChar(16, vm), calcX(192, vm), calcY(442, vm), gui::RED, false);

    this->texts["DAMAGE_TAKEN"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["DAMAGE_TAKEN"] +
            std::to_string(playerData.damageTaken),
        calcChar(16, vm), calcX(192, vm), calcY(492, vm), gui::RED, false);

    this->texts["TOTAL_DEATHS"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["TOTAL_DEATHS"] +
            std::to_string(playerData.deaths),
        calcChar(16, vm), calcX(192, vm), calcY(542, vm), gui::RED, false);

    this->texts["TOTAL_GOLD"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["TOTAL_GOLD"] + std::to_string(playerData.gold),
        calcChar(16, vm), calcX(192, vm), calcY(592, vm), gui::GOLD, false);

    this->texts["BOUGHT_ITEMS"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["BOUGHT_ITEMS"] +
            std::to_string(playerData.boughtItems),
        calcChar(16, vm), calcX(192, vm), calcY(642, vm), gui::GOLD, false);

    this->texts["TIME_PLAYED"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["TIME_PLAYED"] +
            std::to_string(playerData.timePlayed) + "m",
        calcChar(16, vm), calcX(800, vm), calcY(292, vm), gui::LIGHT_GREY,
        false);

    this->texts["TOTAL_PLAYED"] =
        std::make_unique<gui::Text>(this->gameSettings.lang["TOTAL_PLAYED"] +
                                        std::to_string(playerData.gamesPlayed),
                                    calcChar(16, vm), calcX(800, vm),
                                    calcY(342, vm), gui::LIGHT_GREY, false);

    this->texts["RUINS"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["RUINS"] + ": " +
            std::to_string(playerData.ruins),
        calcChar(16, vm), calcX(800, vm), calcY(392, vm), gui::WHITE, false);

    this->texts["DESOLATION"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["DESOLATION"] + ": " +
            std::to_string(playerData.desolation),
        calcChar(16, vm), calcX(800, vm), calcY(442, vm), gui::WHITE, false);

    this->texts["PERMAFROST"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["PERMAFROST"] + ": " +
            std::to_string(playerData.permafrost),
        calcChar(16, vm), calcX(800, vm), calcY(492, vm), gui::WHITE, false);

    this->texts["EASY"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["EASY"] + ": " +
            std::to_string(playerData.easy),
        calcChar(16, vm), calcX(800, vm), calcY(542, vm), gui::WHITE, false);

    this->texts["NORMAL"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["NORMAL"] + ": " +
            std::to_string(playerData.normal),
        calcChar(16, vm), calcX(800, vm), calcY(592, vm), gui::WHITE, false);

    this->texts["HARD"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["HARD"] + ": " +
            std::to_string(playerData.hard),
        calcChar(16, vm), calcX(800, vm), calcY(642, vm), gui::WHITE, false);
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
    this->texts["RECORD_WAVE"]->draw(*target);
    this->texts["TOTAL_XP"]->draw(*target);
    this->texts["RECORD_LEVEL"]->draw(*target);
    this->texts["TOTAL_KILLS"]->draw(*target);
    this->texts["DAMAGE_DEALT"]->draw(*target);
    this->texts["DAMAGE_TAKEN"]->draw(*target);
    this->texts["TOTAL_DEATHS"]->draw(*target);
    this->texts["TOTAL_GOLD"]->draw(*target);
    this->texts["BOUGHT_ITEMS"]->draw(*target);
    this->texts["TIME_PLAYED"]->draw(*target);
    this->texts["TOTAL_PLAYED"]->draw(*target);
    this->texts["RUINS"]->draw(*target);
    this->texts["DESOLATION"]->draw(*target);
    this->texts["PERMAFROST"]->draw(*target);
    this->texts["EASY"]->draw(*target);
    this->texts["NORMAL"]->draw(*target);
    this->texts["HARD"]->draw(*target);
}
