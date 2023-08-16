#include "MainMenuState.hpp"

MainMenuState::MainMenuState(float gridSize, sf::RenderWindow &window,
                             GameSettings &gameSettings,
                             SoundEngine &soundEngine, MusicEngine &musicEngine,
                             std::stack<State *> &states)
    : State(gridSize, window, gameSettings, soundEngine, musicEngine, states)
{
    this->initGUI();
    this->musicEngine.addMusic("main_menu.ogg");
    this->musicEngine.addMusic("main_menu2.ogg");
}

MainMenuState::~MainMenuState() = default;

void MainMenuState::initGUI()
{
    const sf::VideoMode vm = this->gameSettings.resolution;

    this->page = 0;
    this->keysClick["Escape"].first = false;
    this->keysClick["Escape"].second = false;

    this->fading = false;
    this->appearing = false;
    this->dimBackground.setSize(sf::Vector2f(static_cast<float>(vm.width),
                                             static_cast<float>(vm.height)));
    this->dimBackground.setFillColor(sf::Color(0, 0, 0, 0));

    // PAGE 0 - Loading

    this->introCooldown = 0.f;
    this->dimAlpha = 0.f;
    this->sprites["LOGO"] = std::make_unique<gui::Sprite>(
        "assets/textures/icon.png", calcX(640, vm), calcY(232, vm),
        calcScale(8, vm), true);
    this->sprites["LOGO"]->setColor(sf::Color(255, 255, 255, 0));

    // PAGE 1

    Random::Init();

    this->mapVelocity = sf::Vector2f(32.f * cos((3.1415f / 180.f)),
                                     32.f * cos((3.1415f / 180.f)));
    this->mapRotate = Random::Float() * 360.f - 90.f;

    this->mapView.setSize(sf::Vector2f(static_cast<float>(vm.width),
                                       static_cast<float>(vm.height)));
    this->mapView.setCenter(static_cast<float>(vm.width / 2),
                            static_cast<float>(vm.height / 2));

    if (const uint8_t m = static_cast<uint8_t>(Random::Float() * 3.f); m == 0) {
        this->map_texture.loadFromFile("assets/textures/maps/ruins.png");
    }
    else if (m == 1) {
        this->map_texture.loadFromFile("assets/textures/maps/desolation.png");
    }
    else {
        this->map_texture.loadFromFile("assets/textures/maps/permafrost.png");
    }

    this->map.setTexture(this->map_texture);
    this->map.setScale(calcScale(4, vm), calcScale(4, vm));
    this->map.setPosition(256.f, 256.f);
    this->map.setOrigin(256.f, 256.f);
    this->map.setRotation(this->mapRotate);

    this->dimMap.setSize(sf::Vector2f(static_cast<float>(vm.width),
                                      static_cast<float>(vm.height)));
    this->dimMap.setFillColor(sf::Color(0, 0, 0, 192));

    this->quitBackground.setSize(sf::Vector2f(static_cast<float>(vm.width),
                                              static_cast<float>(vm.height)));
    this->quitBackground.setFillColor(sf::Color(0, 0, 0, 192));

    this->sprites["TITLE"] = std::make_unique<gui::Sprite>(
        "assets/textures/title.png", calcX(640, vm), calcY(144, vm),
        calcScale(1, vm), true);

    this->text_buttons["PLAY"] = std::make_unique<gui::ButtonText>(
        this->gameSettings.lang["PLAY"], calcChar(24, vm), calcX(640, vm),
        calcY(300, vm), gui::WHITE, sf::Color(192, 192, 192), true);
    this->text_buttons["STATISTICS"] = std::make_unique<gui::ButtonText>(
        this->gameSettings.lang["STATISTICS"], calcChar(24, vm), calcX(640, vm),
        calcY(372, vm), gui::WHITE, sf::Color(192, 192, 192), true);
    this->text_buttons["SETTINGS"] = std::make_unique<gui::ButtonText>(
        this->gameSettings.lang["SETTINGS"], calcChar(24, vm), calcX(640, vm),
        calcY(444, vm), gui::WHITE, sf::Color(192, 192, 192), true);
    this->text_buttons["CREDITS"] = std::make_unique<gui::ButtonText>(
        this->gameSettings.lang["CREDITS"], calcChar(24, vm), calcX(640, vm),
        calcY(516, vm), gui::WHITE, sf::Color(192, 192, 192), true);
    this->text_buttons["QUIT"] = std::make_unique<gui::ButtonText>(
        this->gameSettings.lang["QUIT"], calcChar(24, vm), calcX(640, vm),
        calcY(588, vm), gui::WHITE, sf::Color(192, 192, 192), true);

    this->texts["VERSION"] =
        std::make_unique<gui::Text>("v0.2.4", calcChar(16, vm), calcX(1272, vm),
                                    calcY(700, vm), gui::WHITE, false);
    this->texts["VERSION"]->setPosition(sf::Vector2f(
        calcX(1272, vm) - this->texts["VERSION"]->getWidth(), calcY(700, vm)));

    this->quitwindow = false;

    this->texts["ARE_YOU_SURE"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["ARE_YOU_SURE"], calcChar(32, vm),
        calcX(640, vm), calcY(250, vm), gui::WHITE, true);
    this->text_buttons["YES"] = std::make_unique<gui::ButtonText>(
        this->gameSettings.lang["YES"], calcChar(32, vm), calcX(488, vm),
        calcY(306, vm), gui::WHITE, sf::Color(192, 192, 192), false);
    this->text_buttons["NO"] = std::make_unique<gui::ButtonText>(
        this->gameSettings.lang["NO"], calcChar(32, vm), calcX(704, vm),
        calcY(306, vm), gui::WHITE, sf::Color(192, 192, 192), false);

    // PAGE 2

    this->sprite_buttons["GO_BACK"] = std::make_unique<gui::ButtonSprite>(
        gui::RECT_ARROW, calcX(32, vm), calcY(24, vm), calcX(4, vm), false);

    this->map_name = "";
    this->texts["CHOOSE_MAP"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["CHOOSE_MAP"], calcChar(32, vm), calcX(640, vm),
        calcY(96, vm), gui::WHITE, true);
    this->sprite_buttons["MAP1"] = std::make_unique<gui::ButtonSprite>(
        gui::RECT_MAP, calcX(24, vm), calcY(248, vm), calcScale(1, vm), false);
    this->sprites["MAP1"] = std::make_unique<gui::Sprite>(
        "assets/textures/maps/ruins.png", calcX(48, vm), calcY(272, vm),
        calcScale(0.5f, vm), false);
    this->texts["RUINS"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["RUINS"], calcChar(32, vm), calcX(176, vm),
        calcY(200, vm), gui::WHITE, true);
    this->sprite_buttons["MAP2"] = std::make_unique<gui::ButtonSprite>(
        gui::RECT_MAP, calcX(472, vm), calcY(248, vm), calcScale(1, vm), false);
    this->sprites["MAP2"] = std::make_unique<gui::Sprite>(
        "assets/textures/maps/desolation.png", calcX(496, vm), calcY(272, vm),
        calcScale(0.5f, vm), false);
    this->texts["DESOLATION"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["DESOLATION"], calcChar(32, vm), calcX(624, vm),
        calcY(200, vm), gui::WHITE, true);
    this->sprite_buttons["MAP3"] = std::make_unique<gui::ButtonSprite>(
        gui::RECT_MAP, calcX(920, vm), calcY(248, vm), calcScale(1, vm), false);
    this->sprites["MAP3"] = std::make_unique<gui::Sprite>(
        "assets/textures/maps/permafrost.png", calcX(944, vm), calcY(272, vm),
        calcScale(0.5f, vm), false);
    this->texts["PERMAFROST"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["PERMAFROST"], calcChar(32, vm),
        calcX(1072, vm), calcY(200, vm), gui::WHITE, true);

    // PAGE 3

    this->hero_name = "";
    this->texts["CHOOSE_HERO"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["CHOOSE_HERO"], calcChar(32, vm),
        calcX(640, vm), calcY(96, vm), gui::WHITE, true);
    this->sprite_buttons["HERO1"] = std::make_unique<gui::ButtonSprite>(
        gui::RECT_BUTTON, calcX(64, vm), calcY(256, vm), calcScale(2, vm),
        false);
    this->sprites["HERO1"] = std::make_unique<gui::Sprite>(
        "assets/textures/upgrades_icons.png", calcX(88, vm), calcY(280, vm),
        calcScale(8, vm), false);
    this->sprites["HERO1"]->setTextureRect(sf::IntRect(0, 0, 16, 16));
    this->texts["WARRIOR"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["WARRIOR"], calcChar(32, vm), calcX(150, vm),
        calcY(200, vm), gui::WHITE, true);

    this->choosing_hero = false;

    this->text_buttons["CHOOSE"] = std::make_unique<gui::ButtonText>(
        this->gameSettings.lang["CHOOSE"], calcChar(32, vm), calcX(240, vm),
        calcY(564, vm), gui::WHITE, sf::Color(192, 192, 192), true);
    this->sprites["HERO_PREVIEW"] = std::make_unique<gui::Sprite>(
        "assets/textures/upgrades_icons.png", calcX(640, vm), calcY(512, vm),
        calcScale(8, vm), true);
    this->sprites["HERO_PREVIEW"]->setTextureRect(sf::IntRect(0, 0, 16, 16));
    this->sprites["HERO_PREVIEW"]->center(calcX(640, vm));
    this->sprites["HP_BAR"] = std::make_unique<gui::Sprite>(
        "assets/textures/bars.png", calcX(860, vm), calcY(526, vm),
        calcScale(1, vm), true);
    this->sprites["HP_BAR"]->setTextureRect(sf::IntRect(0, 20, 256, 20));
    this->sprites["HP_BAR"]->center(calcX(860, vm));
    this->texts["HP"] = std::make_unique<gui::Text>(
        "HP:10/10", calcChar(16, vm), calcX(860, vm), calcY(529, vm),
        gui::WHITE, true);
    this->sprites["SPRINT_BAR"] = std::make_unique<gui::Sprite>(
        "assets/textures/bars.png", calcX(860, vm), calcY(554, vm),
        calcScale(1, vm), true);
    this->sprites["SPRINT_BAR"]->setTextureRect(sf::IntRect(0, 40, 256, 20));
    this->sprites["SPRINT_BAR"]->center(calcX(860, vm));
    this->texts["SPRINT"] =
        std::make_unique<gui::Text>("100/100", calcChar(16, vm), calcX(860, vm),
                                    calcY(557, vm), gui::WHITE, true);

    this->attributes_texture.loadFromFile(
        "assets/textures/attributes_icons.png");
    for (short i = 0; i < 10; ++i) {
        sf::Sprite att;
        att.setTexture(this->attributes_texture);
        sf::IntRect intRect(i * 16, 0, 16, 16);
        att.setTextureRect(intRect);
        att.setScale(calcScale(2, vm), calcScale(2, vm));
        this->attribute_vec.push_back(att);
    }

    this->abilities_texture.loadFromFile("assets/textures/abilities_icons.png");
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            this->abilties.push_back(std::make_unique<gui::Sprite>(
                this->abilities_texture, calcX(432.f + 48.f * x, vm),
                calcY(512.f + 48.f * y, vm), calcScale(2, vm), true,
                sf::IntRect(x * 16, y * 16, 16, 16)));
        }
    }

    this->sprites["ARMOR"] =
        std::make_unique<gui::Sprite>(attribute_vec[1], calcX(728, vm),
                                      calcY(586, vm), calcScale(2, vm), false);
    this->texts["ARMOR"] = std::make_unique<gui::Text>(
        "3", calcChar(16, vm), calcX(744, vm), calcY(640, vm),
        sf::Color(192, 192, 192), true);
    this->sprites["REG"] =
        std::make_unique<gui::Sprite>(attribute_vec[2], calcX(792, vm),
                                      calcY(586, vm), calcScale(2, vm), false);
    this->texts["REG"] = std::make_unique<gui::Text>(
        "1", calcChar(16, vm), calcX(808, vm), calcY(640, vm),
        sf::Color(182, 60, 53), true);
    this->sprites["ATTACK"] =
        std::make_unique<gui::Sprite>(attribute_vec[5], calcX(856, vm),
                                      calcY(586, vm), calcScale(2, vm), false);
    this->texts["ATTACK"] = std::make_unique<gui::Text>(
        "5", calcChar(16, vm), calcX(872, vm), calcY(640, vm),
        sf::Color(192, 192, 192), true);
    this->sprites["ATTACK_SPEED"] =
        std::make_unique<gui::Sprite>(attribute_vec[6], calcX(920, vm),
                                      calcY(586, vm), calcScale(2, vm), false);
    this->texts["ATTACK_SPEED"] = std::make_unique<gui::Text>(
        "4", calcChar(16, vm), calcX(936, vm), calcY(640, vm),
        sf::Color(192, 192, 192), true);
    this->sprites["SPEED"] =
        std::make_unique<gui::Sprite>(attribute_vec[7], calcX(984, vm),
                                      calcY(586, vm), calcScale(2, vm), false);
    this->texts["SPEED"] = std::make_unique<gui::Text>(
        "4", calcChar(16, vm), calcX(1000, vm), calcY(640, vm),
        sf::Color(192, 192, 192), true);
    this->sprites["CRITICAL"] =
        std::make_unique<gui::Sprite>(attribute_vec[8], calcX(1048, vm),
                                      calcY(586, vm), calcScale(2, vm), false);
    this->texts["CRITICAL"] = std::make_unique<gui::Text>(
        "20%", calcChar(16, vm), calcX(1064, vm), calcY(640, vm),
        sf::Color(192, 192, 192), true);

    // PAGE 4

    this->difficulty_name = "";
    this->texts["CHOOSE_DIFFICULTY"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["CHOOSE_DIFFICULTY"], calcChar(32, vm),
        calcX(640, vm), calcY(96, vm), gui::WHITE, true);

    this->sprite_buttons["DIFFICULTY1"] = std::make_unique<gui::ButtonSprite>(
        gui::RECT_DIFFICULTY, calcX(32, vm), calcY(184, vm), calcScale(1, vm),
        false);
    this->sprite_buttons["DIFFICULTY2"] = std::make_unique<gui::ButtonSprite>(
        gui::RECT_DIFFICULTY, calcX(448, vm), calcY(184, vm), calcScale(1, vm),
        false);
    this->sprite_buttons["DIFFICULTY3"] = std::make_unique<gui::ButtonSprite>(
        gui::RECT_DIFFICULTY, calcX(864, vm), calcY(184, vm), calcScale(1, vm),
        false);

    this->sprites["DIFFICULTY1"] = std::make_unique<gui::Sprite>(
        "assets/textures/difficulty_icons.png", calcX(168, vm), calcY(272, vm),
        calcScale(16, vm), false);
    this->sprites["DIFFICULTY1"]->setTextureRect(sf::IntRect(0, 0, 7, 6));
    this->sprites["DIFFICULTY2"] = std::make_unique<gui::Sprite>(
        "assets/textures/difficulty_icons.png", calcX(584, vm), calcY(272, vm),
        calcScale(16, vm), false);
    this->sprites["DIFFICULTY2"]->setTextureRect(sf::IntRect(7, 0, 7, 6));
    this->sprites["DIFFICULTY3"] = std::make_unique<gui::Sprite>(
        "assets/textures/difficulty_icons.png", calcX(1000, vm), calcY(272, vm),
        calcScale(16, vm), false);
    this->sprites["DIFFICULTY3"]->setTextureRect(sf::IntRect(14, 0, 7, 6));

    this->texts["EASY"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["EASY"], calcChar(32, vm), calcX(224, vm),
        calcY(400, vm), gui::WHITE, true);
    this->texts["NORMAL"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["NORMAL"], calcChar(32, vm), calcX(640, vm),
        calcY(400, vm), gui::WHITE, true);
    this->texts["HARD"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["HARD"], calcChar(32, vm), calcX(1056, vm),
        calcY(400, vm), gui::WHITE, true);
    this->texts["EASY_DESC"] = std::make_unique<gui::Text>(

        this->gameSettings.lang["MONSTERS_HAVE"] + "\n\n-25% HP\n\n-25% " +
            this->gameSettings.lang["ATTACK"] + "\n\n50% " +
            this->gameSettings.lang["HEART_DROP"],
        calcChar(16, vm), calcX(224, vm), calcY(460, vm),
        sf::Color(182, 60, 53), true);
    this->texts["NORMAL_DESC"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["NORMAL_DESC"], calcChar(16, vm),
        calcX(640, vm), calcY(460, vm), sf::Color(182, 60, 53), true);
    this->texts["HARD_DESC"] = std::make_unique<gui::Text>(

        this->gameSettings.lang["MONSTERS_HAVE"] + "\n\n+25% HP\n\n+25% " +
            this->gameSettings.lang["ATTACK"] + "\n\n" +
            this->gameSettings.lang["TIME_DROP"],
        calcChar(16, vm), calcX(1056, vm), calcY(460, vm),
        sf::Color(182, 60, 53), true);

    // PAGE 5

    this->texts["CREDITS"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["CREDITS"], calcChar(32, vm), calcX(640, vm),
        calcY(96, vm), gui::WHITE, true);
    this->texts["MAIN_CREATOR"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["MAIN_CREATOR"], calcChar(16, vm),
        calcX(400, vm), calcY(192, vm), gui::WHITE, false);
    this->texts["CREATOR_LIST"] = std::make_unique<gui::Text>(
        "Piterson25", calcChar(16, vm), calcX(722, vm), calcY(192, vm),
        gui::LIME, false);
    this->texts["ARTIST"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["ARTIST"], calcChar(16, vm), calcX(400, vm),
        calcY(292, vm), gui::WHITE, false);
    this->texts["ARTIST_LIST"] = std::make_unique<gui::Text>(
        "Nithorax", calcChar(16, vm), calcX(722, vm), calcY(292, vm), gui::RED,
        false);
    this->texts["TESTERS"] = std::make_unique<gui::Text>(
        this->gameSettings.lang["TESTERS"], calcChar(16, vm), calcX(400, vm),
        calcY(392, vm), gui::WHITE, false);
    this->texts["TESTERS_LIST"] = std::make_unique<gui::Text>(
        "Szmigielko\n\nRaspar\n\nKeku\n\nyouhOrin", calcChar(16, vm),
        calcX(722, vm), calcY(392, vm), gui::LIGHT_BLUE, false);
}

void MainMenuState::resetGUI()
{
    this->texts.clear();
    this->text_buttons.clear();
    this->sprites.clear();
    this->sprite_buttons.clear();

    initGUI();
}

void MainMenuState::update(float dt)
{
    this->updateMousePositions();

    if (this->page >= 1) {
        if (!this->map.getGlobalBounds().intersects(
                this->mapView.getViewport())) {
            this->mapVelocity = sf::Vector2f(32.f * cos((3.1415f / 180.f)),
                                             32.f * cos((3.1415f / 180.f)));
            this->mapRotate = Random::Float() * 360.f - 90.f;
            this->map.setPosition(256.f, 256.f);
            this->map.setRotation(this->mapRotate);
        }
        this->map.move(this->mapVelocity * dt);
    }

    if (this->fading) {
        fadingEffect(dt);
    }
    else {
        switch (this->page) {
            case 0:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                    this->page = 1;
                    this->dimAlpha = 1.f;
                    this->fading = true;
                    this->appearing = true;
                    break;
                }

                this->introCooldown += dt;

                if (this->introCooldown > 1.f) {
                    if (this->introCooldown > 5.f) {
                        this->page = 1;
                        this->dimAlpha = 1.f;
                        this->fading = true;
                        this->appearing = true;
                        break;
                    }
                    else {
                        if (this->introCooldown < 3.f) {
                            if (this->dimAlpha < 1.f) {
                                this->dimAlpha += dt;
                            }
                            if (this->dimAlpha > 1.f) {
                                this->dimAlpha = 1.f;
                            }
                        }
                        else if (this->introCooldown < 5.f &&
                                 this->introCooldown > 3.f) {
                            if (this->dimAlpha > 0.f) {
                                this->dimAlpha -= dt;
                            }
                            if (this->dimAlpha < 0.f) {
                                this->dimAlpha = 0.f;
                            }
                        }
                        this->sprites["LOGO"]->setColor(sf::Color(
                            255, 255, 255,
                            static_cast<sf::Uint8>(this->dimAlpha * 255.f)));
                    }
                }
                break;
            case 1:

                if (!quitwindow) {
                    this->text_buttons["PLAY"]->update(this->mousePosWindow);
                    this->text_buttons["STATISTICS"]->update(
                        this->mousePosWindow);
                    this->text_buttons["SETTINGS"]->update(
                        this->mousePosWindow);
                    this->text_buttons["CREDITS"]->update(this->mousePosWindow);
                    this->text_buttons["QUIT"]->update(this->mousePosWindow);

                    if (this->text_buttons["PLAY"]->isPressed() &&
                        !this->isMouseClicked()) {
                        this->soundEngine.addSound("button");
                        this->setMouseClick(true);
                        this->map_name = "";
                        this->page = 2;
                    }
                    else if (this->text_buttons["SETTINGS"]->isPressed() &&
                             !this->isMouseClicked()) {
                        this->setMouseClick(true);
                        this->soundEngine.addSound("button");
                        this->states.push(new SettingsState(
                            this->gridSize, this->window, this->gameSettings,
                            this->soundEngine, this->musicEngine,
                            this->states));
                    }
                    else if (this->text_buttons["STATISTICS"]->isPressed() &&
                             !this->isMouseClicked()) {
                        this->setMouseClick(true);
                        this->soundEngine.addSound("button");
                        this->states.push(new StatsState(
                            this->gridSize, this->window, this->gameSettings,
                            this->soundEngine, this->musicEngine,
                            this->states));
                    }
                    else if (this->text_buttons["CREDITS"]->isPressed() &&
                             !this->isMouseClicked()) {
                        this->soundEngine.addSound("button");
                        this->setMouseClick(true);
                        this->page = 5;
                    }
                    else if (this->text_buttons["QUIT"]->isPressed() &&
                             !this->isMouseClicked()) {
                        this->setMouseClick(true);
                        this->soundEngine.addSound("button");
                        this->quitwindow = true;
                    }
                }
                else {
                    this->text_buttons["YES"]->update(this->mousePosWindow);
                    this->text_buttons["NO"]->update(this->mousePosWindow);

                    if (this->text_buttons["YES"]->isPressed() &&
                        !this->isMouseClicked()) {
                        this->setMouseClick(true);
                        this->soundEngine.addSound("button");
                        this->endState();
                    }
                    else if (this->text_buttons["NO"]->isPressed() &&
                             !this->isMouseClicked()) {
                        this->setMouseClick(true);
                        this->soundEngine.addSound("button");
                        this->quitwindow = false;
                    }
                }
                break;
            case 2:
                this->sprite_buttons["GO_BACK"]->update(this->mousePosWindow);
                if (this->sprite_buttons["GO_BACK"]->isPressed() &&
                    !this->isMouseClicked()) {
                    this->setMouseClick(true);
                    this->soundEngine.addSound("button");
                    --this->page;
                    this->map_name = "";
                }

                this->sprite_buttons["MAP1"]->update(this->mousePosWindow);
                if (this->sprite_buttons["MAP1"]->isPressed() &&
                    !this->isMouseClicked()) {
                    this->setMouseClick(true);
                    this->soundEngine.addSound("button");
                    this->map_name = "ruins";
                    this->page = 3;
                }

                this->sprite_buttons["MAP2"]->update(this->mousePosWindow);
                if (this->sprite_buttons["MAP2"]->isPressed() &&
                    !this->isMouseClicked()) {
                    this->setMouseClick(true);
                    this->soundEngine.addSound("button");
                    this->map_name = "desolation";
                    this->page = 3;
                }

                this->sprite_buttons["MAP3"]->update(this->mousePosWindow);
                if (this->sprite_buttons["MAP3"]->isPressed() &&
                    !this->isMouseClicked()) {
                    this->setMouseClick(true);
                    this->soundEngine.addSound("button");
                    this->map_name = "permafrost";
                    this->page = 3;
                }
                break;
            case 3:
                this->sprite_buttons["GO_BACK"]->update(this->mousePosWindow);
                if (this->sprite_buttons["GO_BACK"]->isPressed() &&
                    !this->isMouseClicked()) {
                    this->setMouseClick(true);
                    this->soundEngine.addSound("button");
                    --this->page;
                    this->hero_name = "";
                    this->choosing_hero = false;
                }

                this->sprite_buttons["HERO1"]->update(this->mousePosWindow);
                if (this->sprite_buttons["HERO1"]->isPressed() &&
                    !this->isMouseClicked()) {
                    this->setMouseClick(true);
                    this->soundEngine.addSound("button");
                    this->choosing_hero = true;
                    this->hero_name = "WARRIOR";
                }

                if (this->choosing_hero) {
                    this->text_buttons["CHOOSE"]->update(this->mousePosWindow);
                    if (this->text_buttons["CHOOSE"]->isPressed() &&
                        !this->isMouseClicked()) {
                        this->soundEngine.addSound("button");
                        this->page = 4;
                    }
                }
                break;
            case 4:
                this->sprite_buttons["GO_BACK"]->update(this->mousePosWindow);
                if (this->sprite_buttons["GO_BACK"]->isPressed() &&
                    !this->isMouseClicked()) {
                    this->setMouseClick(true);
                    this->soundEngine.addSound("button");
                    --this->page;
                    this->difficulty_name = "";
                    this->choosing_hero = false;
                }

                this->sprite_buttons["DIFFICULTY1"]->update(
                    this->mousePosWindow);
                this->sprite_buttons["DIFFICULTY2"]->update(
                    this->mousePosWindow);
                this->sprite_buttons["DIFFICULTY3"]->update(
                    this->mousePosWindow);
                if (this->sprite_buttons["DIFFICULTY1"]->isPressed() &&
                    !this->isMouseClicked()) {
                    this->setMouseClick(true);
                    this->soundEngine.addSound("button");
                    this->difficulty_name = "EASY";
                    this->page = 1;
                    this->states.push(new GameState(
                        this->gridSize, this->window, this->gameSettings,
                        this->soundEngine, this->musicEngine, this->states,
                        this->map_name, this->hero_name,
                        this->difficulty_name));
                    this->choosing_hero = false;
                }
                else if (this->sprite_buttons["DIFFICULTY2"]->isPressed() &&
                         !this->isMouseClicked()) {
                    this->setMouseClick(true);
                    this->soundEngine.addSound("button");
                    this->difficulty_name = "NORMAL";
                    this->page = 1;
                    this->states.push(new GameState(
                        this->gridSize, this->window, this->gameSettings,
                        this->soundEngine, this->musicEngine, this->states,
                        this->map_name, this->hero_name,
                        this->difficulty_name));
                    this->choosing_hero = false;
                }
                else if (this->sprite_buttons["DIFFICULTY3"]->isPressed() &&
                         !this->isMouseClicked()) {
                    this->setMouseClick(true);
                    this->soundEngine.addSound("button");
                    this->difficulty_name = "HARD";
                    this->page = 1;
                    this->states.push(new GameState(
                        this->gridSize, this->window, this->gameSettings,
                        this->soundEngine, this->musicEngine, this->states,
                        this->map_name, this->hero_name,
                        this->difficulty_name));
                    this->choosing_hero = false;
                }
                break;
            case 5:
                this->sprite_buttons["GO_BACK"]->update(this->mousePosWindow);
                if (this->sprite_buttons["GO_BACK"]->isPressed() &&
                    !this->isMouseClicked()) {
                    this->setMouseClick(true);
                    this->soundEngine.addSound("button");
                    this->page = 1;
                }
                break;
        }
    }

    this->updateMouseClick();

    if (!this->quitwindow && this->page > 0) {
        this->updateKeysClick("Escape", sf::Keyboard::Escape);

        if (this->isKeyClicked1("Escape") && !this->isKeyClicked2("Escape")) {
            this->setKeysClick("Escape", true);
            if (this->page == 1) {
                this->quitwindow = true;
            }
            else {
                --this->page;
            }
        }
        this->setKeysClick("Escape", this->isKeyClicked1("Escape"));
    }

    if (this->musicEngine.isEmpty()) {
        this->musicEngine.addMusic("main_menu.ogg");
        this->musicEngine.addMusic("main_menu2.ogg");
    }

    this->musicEngine.update();
}

void MainMenuState::draw(sf::RenderTarget *target)
{
    if (!target) {
        target = &this->window;
    }

    switch (this->page) {
        case 0:
            this->sprites["LOGO"]->draw(*target);
            break;
        case 1:
            target->draw(this->map);
            target->draw(this->dimMap);

            this->sprites["TITLE"]->draw(*target);

            this->text_buttons["PLAY"]->draw(*target);
            this->text_buttons["STATISTICS"]->draw(*target);
            this->text_buttons["SETTINGS"]->draw(*target);
            this->text_buttons["CREDITS"]->draw(*target);
            this->text_buttons["QUIT"]->draw(*target);

            this->texts["VERSION"]->draw(*target);
            if (this->quitwindow) {
                target->draw(this->quitBackground);
                this->texts["ARE_YOU_SURE"]->draw(*target);

                this->text_buttons["YES"]->draw(*target);
                this->text_buttons["NO"]->draw(*target);
            }
            break;
        case 2:
            this->sprite_buttons["GO_BACK"]->draw(*target);

            this->texts["CHOOSE_MAP"]->draw(*target);
            this->sprites["MAP1"]->draw(*target);
            this->sprite_buttons["MAP1"]->draw(*target);
            this->sprites["MAP2"]->draw(*target);
            this->sprite_buttons["MAP2"]->draw(*target);
            this->sprites["MAP3"]->draw(*target);
            this->sprite_buttons["MAP3"]->draw(*target);

            this->texts["RUINS"]->draw(*target);
            this->texts["DESOLATION"]->draw(*target);
            this->texts["PERMAFROST"]->draw(*target);
            break;
        case 3:
            this->sprite_buttons["GO_BACK"]->draw(*target);

            this->texts["CHOOSE_HERO"]->draw(*target);
            this->sprites["HERO1"]->draw(*target);
            this->sprite_buttons["HERO1"]->draw(*target);

            this->texts["WARRIOR"]->draw(*target);

            if (this->choosing_hero) {
                this->text_buttons["CHOOSE"]->draw(*target);

                for (const auto &ability : abilties) {
                    ability->draw(*target);
                }

                this->sprites["HERO_PREVIEW"]->draw(*target);
                this->sprites["HP_BAR"]->draw(*target);
                this->texts["HP"]->draw(*target);
                this->sprites["SPRINT_BAR"]->draw(*target);
                this->texts["SPRINT"]->draw(*target);

                this->sprites["ARMOR"]->draw(*target);
                this->texts["ARMOR"]->draw(*target);
                this->sprites["REG"]->draw(*target);
                this->texts["REG"]->draw(*target);
                this->sprites["ATTACK"]->draw(*target);
                this->texts["ATTACK"]->draw(*target);
                this->sprites["ATTACK_SPEED"]->draw(*target);
                this->texts["ATTACK_SPEED"]->draw(*target);
                this->sprites["SPEED"]->draw(*target);
                this->texts["SPEED"]->draw(*target);
                this->sprites["CRITICAL"]->draw(*target);
                this->texts["CRITICAL"]->draw(*target);
            }
            break;
        case 4:
            this->sprite_buttons["GO_BACK"]->draw(*target);

            this->texts["CHOOSE_DIFFICULTY"]->draw(*target);

            this->sprites["DIFFICULTY1"]->draw(*target);
            this->sprites["DIFFICULTY2"]->draw(*target);
            this->sprites["DIFFICULTY3"]->draw(*target);
            this->sprite_buttons["DIFFICULTY1"]->draw(*target);
            this->sprite_buttons["DIFFICULTY2"]->draw(*target);
            this->sprite_buttons["DIFFICULTY3"]->draw(*target);

            this->texts["EASY"]->draw(*target);
            this->texts["NORMAL"]->draw(*target);
            this->texts["HARD"]->draw(*target);
            this->texts["EASY_DESC"]->draw(*target);
            this->texts["NORMAL_DESC"]->draw(*target);
            this->texts["HARD_DESC"]->draw(*target);
            break;

        case 5:
            this->sprite_buttons["GO_BACK"]->draw(*target);
            this->texts["CREDITS"]->draw(*target);
            this->texts["MAIN_CREATOR"]->draw(*target);
            this->texts["CREATOR_LIST"]->draw(*target);
            this->texts["ARTIST"]->draw(*target);
            this->texts["ARTIST_LIST"]->draw(*target);
            this->texts["TESTERS"]->draw(*target);
            this->texts["TESTERS_LIST"]->draw(*target);
    }

    target->draw(dimBackground);
}

void MainMenuState::fadingEffect(float dt)
{
    if (this->appearing) {
        if (this->dimAlpha > 0.f) {
            this->dimAlpha -= dt;
            if (this->dimAlpha < 0.f) {
                this->dimAlpha = 0.f;
                this->fading = false;
            }
        }
    }
    else {
        if (this->dimAlpha < 1.f) {
            this->dimAlpha += dt;
            if (this->dimAlpha > 1.f) {
                this->dimAlpha = 1.f;
                this->fading = false;
            }
        }
    }
    this->dimBackground.setFillColor(
        sf::Color(0, 0, 0, static_cast<sf::Uint8>(this->dimAlpha * 255.f)));
}
