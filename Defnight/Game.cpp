#include "stdafx.h"
#include "Functions.h"
#include "Game.h"

Game::Game()
{
	init();
}

Game::~Game()
{
	while (!this->states.empty()) {
		delete this->states.top();
		this->states.pop();
	}

	delete this->window;
	delete this->musicEngine;
	delete this->soundEngine;
}

void Game::init()
{
	this->window = NULL;
	this->dt = 0.f;
	this->gridSize = 64.f;
	this->sfEvent.type = sf::Event::GainedFocus;

	this->gameSettings.load();

	if (this->gameSettings.fullscreen)
		this->window = new sf::RenderWindow(this->gameSettings.resolution, "Defnight", sf::Style::Fullscreen);
	else
		this->window = new sf::RenderWindow(this->gameSettings.resolution, "Defnight", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(this->gameSettings.fpsLimit);

	sf::Image icon;
	icon.loadFromFile("external/assets/logo.png");
	this->window->setIcon(32, 32, icon.getPixelsPtr());

	std::ifstream ifs("external/config/supported_keys.ini");
	if (ifs.is_open()) {
		std::string key = "";
		int key_value = 0;

		while (ifs >> key >> key_value) {
			this->supportedKeys[key] = key_value;
		}
	}
	ifs.close();

	if (!this->font.loadFromFile("external/font/PressStart2P-vaV7.ttf")) {
		throw("ERROR - COULDN'T FIND FONT");
	}

	const sf::VideoMode vm = this->gameSettings.resolution;

	this->fps = 0;
	this->fpsTimer = 0.f;
	this->fpsCounter = new gui::Text(&this->font, "", calcChar(16, vm), calcX(4, vm), calcY(4, vm), sf::Color(255, 255, 255), false);

	this->soundEngine = new SoundEngine(this->gameSettings.soundsVolume);
	this->musicEngine = new MusicEngine(this->gameSettings.musicVolume);

	this->states.push(new MainMenuState(this->gridSize, this->window, &this->gameSettings, &this->supportedKeys, &this->font, this->soundEngine, this->musicEngine, &this->states));
}

void Game::checkEvents()
{
	while (this->window->pollEvent(this->sfEvent)) {
		if (this->sfEvent.type == sf::Event::Closed) {
			this->close();
		}
	}
}

void Game::close()
{
	this->window->close();
}

void Game::draw()
{
	this->window->clear();
	if (!this->states.empty()) this->states.top()->draw();
	if (this->gameSettings.fpsCounterOn) this->fpsCounter->draw(*this->window);
	this->window->display();
}

void Game::update()
{
	this->dt = this->dtClock.restart().asSeconds();

	if (this->gameSettings.fpsCounterOn) {
		++this->fps;
		this->fpsTimer += dt;

		if (this->fpsTimer >= 1.f) {
			this->fpsCounter->setText(std::to_string(this->fps) + " FPS");
			this->fps = 0;
			this->fpsTimer = 0.f;
		}
	}

	this->checkEvents();
	if (!this->states.empty()) {
		if (this->window->hasFocus()) {
			this->states.top()->update(this->dt);

			if (this->states.top()->getQuit()) {
				this->states.top()->endState();
				delete this->states.top();
				this->states.pop();
			}
			else if (this->states.top()->getReseted()) {
				while (!this->states.empty()) {
					delete this->states.top();
					this->states.pop();
				}
				this->musicEngine->stopMusic();
				this->close();
				this->init();
			}
		}
	}
	else this->close();
}

void Game::run()
{
	while (this->window->isOpen()) {
		this->update();
		this->draw();
	}
}
