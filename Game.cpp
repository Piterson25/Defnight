#include "Functions.h"
#include "Game.h"

Game::Game()
{
	this->dt = 0.f;
	this->graphicsSettings.loadFromFile("external/config/graphics.ini");

	if (this->graphicsSettings.fullscreen)
		this->window.create(this->graphicsSettings.resolution, "Defnight", sf::Style::Fullscreen);
	else
		this->window.create(this->graphicsSettings.resolution, "Defnight", sf::Style::Titlebar | sf::Style::Close);

	this->window.setVerticalSyncEnabled(this->graphicsSettings.verticalSync);
	this->window.setKeyRepeatEnabled(false);

	bool isFullHD = false;
	this->scale = 1.f;
	if (this->graphicsSettings.resolution.width >= 1920 && this->graphicsSettings.resolution.height >= 1080) {
		this->scale = 1.5f;
		isFullHD = true;
	}
	this->gridSize = 64.f * this->scale;


	bool isWsad = false;
	bool isFps = false;

	std::ifstream ifs("external/config/supported_keys.ini");
	if (ifs.is_open()) {
		ifs >> isWsad;
		ifs >> isFps;
	}
	ifs.close();

	sf::Image icon;
	icon.loadFromFile("external/assets/logo.png");
	this->window.setIcon(32, 32, icon.getPixelsPtr());

	if (isWsad) {
		this->klawisze.push_back(sf::Keyboard::Key::W);
		this->klawisze.push_back(sf::Keyboard::Key::S);
		this->klawisze.push_back(sf::Keyboard::Key::A);
		this->klawisze.push_back(sf::Keyboard::Key::D);
	}
	else {
		this->klawisze.push_back(sf::Keyboard::Key::Up);
		this->klawisze.push_back(sf::Keyboard::Key::Down);
		this->klawisze.push_back(sf::Keyboard::Key::Left);
		this->klawisze.push_back(sf::Keyboard::Key::Right);
	}

	this->menu.init(this->scale, this->klawisze, isFullHD, this->graphicsSettings.fullscreen, this->graphicsSettings.verticalSync, isWsad, isFps);
}

Game::~Game()
{
	
}

void Game::check_events()
{
	while (window.pollEvent(this->sfEvent)) {
		if (this->sfEvent.type == sf::Event::Closed) {
			this->close();
		}
	}
}

void Game::close()
{
	this->window.close();
}


void Game::draw()
{
	this->window.clear();
	this->menu.draw(this->window);
	this->window.display();
}

void Game::update()
{
	this->dt = this->dtClock.restart().asSeconds();
	this->check_events();
	this->menu.update(this->window, this->dt);
}

void Game::run()
{
	while (this->window.isOpen()) {
		this->update();
		this->draw();
	}
}
