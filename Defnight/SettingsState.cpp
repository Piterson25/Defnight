#include "stdafx.h"
#include "Functions.h"
#include "SettingsState.h"

SettingsState::SettingsState(const float& gridSize, sf::RenderWindow* window, GameSettings* grap, 
	std::unordered_map<std::string, int>* supportedKeys, sf::Font* font, std::stack<State*>* states)
	: State(gridSize, window, grap, supportedKeys, font, states)
{
	initGUI();
}

SettingsState::~SettingsState()
{

}

void SettingsState::initGUI()
{
	const sf::VideoMode vm = this->gameSettings->resolution;

	this->mode = vm;

	this->id = 0;

	this->keysClick["Escape"].first = false;
	this->keysClick["Escape"].second = false;

	for (const auto& v : sf::VideoMode::getFullscreenModes()) {
		if (static_cast<float>(v.width) / v.height == 16.f / 9.f) this->videoModes.push_back(v);
	}

	this->texts["SETTINGS"] = new gui::Text(&this->font, this->lang["SETTINGS"], calcChar(32, vm), calcX(640, vm), calcY(96, vm), sf::Color(255, 255, 255), true);
	this->sprites["GO_BACK"] = new gui::Sprite("external/assets/go_back.png", calcX(1192, vm), calcY(24, vm), calcX(4, vm), false);
	this->sprite_buttons["GO_BACK"] = new gui::ButtonSprite("external/assets/select_go_back.png", calcX(1192, vm), calcY(24, vm), calcX(4, vm), false);

	for (size_t i = 0; i < this->videoModes.size(); ++i) {
		if (this->videoModes[i] == vm) {
			this->id = i;
			this->text_buttons["RESOLUTION"] = new gui::ButtonText(&this->font, this->lang["RESOLUTION"] + std::to_string(this->videoModes[this->id].width) + "x" + std::to_string(this->videoModes[this->id].height), calcChar(16, vm), calcX(96, vm), calcY(192, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
			break;
		}
	}

	if (this->gameSettings->fullscreen) {
		this->fullscreen = true;
		this->text_buttons["FULLSCREEN"] = new gui::ButtonText(&this->font, this->lang["FULLSCREEN"] + this->lang["YES"], calcChar(16, vm), calcX(96, vm), calcY(242, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
	}
	else {
		this->fullscreen = false;
		this->text_buttons["FULLSCREEN"] = new gui::ButtonText(&this->font, this->lang["FULLSCREEN"] + this->lang["NO"], calcChar(16, vm), calcX(96, vm), calcY(242, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
	}

	this->fpsLimit = this->gameSettings->fpsLimit;

	this->fpsLimits.push_back(60);
	this->fpsLimits.push_back(120);
	this->fpsLimits.push_back(240);
	this->fpsLimits.push_back(0);

	this->fps_id = 0;

	for (size_t i = 0; i < this->fpsLimits.size(); ++i) {
		if (this->fpsLimits[i] == this->gameSettings->fpsLimit) {
			this->fps_id = i;
			if (this->fpsLimits[i] == 0) 
				this->text_buttons["FPS_LIMIT"] = new gui::ButtonText(&this->font, this->lang["FPS_LIMIT"] + this->lang["NO_LIMIT"], calcChar(16, vm), calcX(96, vm), calcY(292, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
			else 
				this->text_buttons["FPS_LIMIT"] = new gui::ButtonText(&this->font, this->lang["FPS_LIMIT"] + std::to_string(this->fpsLimits[this->fps_id]), calcChar(16, vm), calcX(96, vm), calcY(292, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
			break;
		}
	}

	this->musicVolume = this->gameSettings->musicVolume;
	this->musicVolume_id = 0;
	for (uint16_t i = 0; i <= 100; i += 10) {
		this->musicVolumes.push_back(i);
	}

	for (size_t i = 0; i < this->musicVolumes.size(); ++i) {
		if (this->musicVolumes[i] == this->musicVolume) {
			this->musicVolume_id = i;
			this->text_buttons["MUSIC"] = new gui::ButtonText(&this->font, this->lang["MUSIC"] + std::to_string(this->musicVolumes[this->musicVolume_id]) + "%", calcChar(16, vm), calcX(96, vm), calcY(342, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
			break;
		}
	}
	

	this->soundsVolume = this->gameSettings->soundsVolume;
	this->soundsVolume_id = 0;
	for (uint16_t i = 0; i <= 100; i += 10) {
		this->soundsVolumes.push_back(i);
	}

	for (size_t i = 0; i < this->soundsVolumes.size(); ++i) {
		if (this->soundsVolumes[i] == this->soundsVolume) {
			this->soundsVolume_id = i;
			this->text_buttons["SOUNDS"] = new gui::ButtonText(&this->font, this->lang["SOUNDS"] + std::to_string(this->soundsVolumes[this->soundsVolume_id]) + "%", calcChar(16, vm), calcX(96, vm), calcY(392, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
			break;
		}
	}


	if (this->gameSettings->language == "english") {
		this->language = "english";
		this->text_buttons["LANGUAGE"] = new gui::ButtonText(&this->font, this->lang["LANGUAGE"] + this->lang["ENGLISH"], calcChar(16, vm), calcX(96, vm), calcY(442, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
	}
	else if (this->gameSettings->language == "polish") {
		this->language = "polish";
		this->text_buttons["LANGUAGE"] = new gui::ButtonText(&this->font, this->lang["LANGUAGE"] + this->lang["POLISH"], calcChar(16, vm), calcX(96, vm), calcY(442, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
	}

	if (this->gameSettings->fpsCounterOn) {
		this->fpsCounterOn = true;
		this->text_buttons["FPS_COUNTER"] = new gui::ButtonText(&this->font, this->lang["FPS_COUNTER"] + this->lang["ON"], calcChar(16, vm), calcX(96, vm), calcY(492, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
	}
	else {
		this->fpsCounterOn = false;
		this->text_buttons["FPS_COUNTER"] = new gui::ButtonText(&this->font, this->lang["FPS_COUNTER"] + this->lang["OFF"], calcChar(16, vm), calcX(96, vm), calcY(492, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
	}

	this->text_buttons["APPLY"] = new gui::ButtonText(&this->font, this->lang["APPLY"], calcChar(32, vm), calcX(1000, vm), calcY(592, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
	this->text_buttons["APPLY"]->setPosition(sf::Vector2f(calcX(1184, vm) - this->text_buttons["APPLY"]->getWidth(), calcY(592, vm)));
}

void SettingsState::resetGUI()
{
	for (auto it = this->texts.begin(); it != this->texts.end(); ++it)
	{
		delete it->second;
	}
	this->texts.clear();

	for (auto it = this->text_buttons.begin(); it != this->text_buttons.end(); ++it)
	{
		delete it->second;
	}
	this->text_buttons.clear();

	for (auto it = this->sprites.begin(); it != this->sprites.end(); ++it)
	{
		delete it->second;
	}
	this->sprites.clear();

	for (auto it = this->sprite_buttons.begin(); it != this->sprite_buttons.end(); ++it)
	{
		delete it->second;
	}
	this->sprite_buttons.clear();

	initGUI();
}

void SettingsState::update(const float& dt)
{
	this->updateMousePositions();

	this->sprite_buttons["GO_BACK"]->update(this->mousePosWindow);
	if (this->sprite_buttons["GO_BACK"]->isPressed() && !this->getMouseClick()) {
		this->setMouseClick(true);
		this->endState();
	}

	this->text_buttons["RESOLUTION"]->update(this->mousePosWindow);
	if (this->text_buttons["RESOLUTION"]->isPressed() && !this->getMouseClick()) {
		this->setMouseClick(true);
		
		++this->id;
		if (this->id == this->videoModes.size()) {
			this->id = 0;
		}

		this->text_buttons["RESOLUTION"]->setText(this->lang["RESOLUTION"] + std::to_string(this->videoModes[this->id].width) + "x" + std::to_string(this->videoModes[this->id].height));
		this->mode = this->videoModes[this->id];
	}

	this->text_buttons["FULLSCREEN"]->update(this->mousePosWindow);
	if (this->text_buttons["FULLSCREEN"]->isPressed() && !this->getMouseClick()) {
		this->setMouseClick(true);

		if (this->fullscreen) {
			this->fullscreen = false;
			this->text_buttons["FULLSCREEN"]->setText(this->lang["FULLSCREEN"] + this->lang["NO"]);
		}
		else {
			this->fullscreen = true;
			this->text_buttons["FULLSCREEN"]->setText(this->lang["FULLSCREEN"] + this->lang["YES"]);
		}
	}

	this->text_buttons["FPS_LIMIT"]->update(this->mousePosWindow);
	if (this->text_buttons["FPS_LIMIT"]->isPressed() && !this->getMouseClick()) {
		this->setMouseClick(true);

		++this->fps_id;
		if (this->fps_id == this->fpsLimits.size()) {
			this->fps_id = 0;
		}

		if (this->fpsLimits[fps_id] == 0)
			this->text_buttons["FPS_LIMIT"]->setText(this->lang["FPS_LIMIT"] + this->lang["NO_LIMIT"]);
		else
			this->text_buttons["FPS_LIMIT"]->setText(this->lang["FPS_LIMIT"] + std::to_string(this->fpsLimits[this->fps_id]));
		this->fpsLimit = this->fpsLimits[this->fps_id];
	}

	this->text_buttons["MUSIC"]->update(this->mousePosWindow);
	if (this->text_buttons["MUSIC"]->isPressed() && !this->getMouseClick()) {
		this->setMouseClick(true);

		++this->musicVolume_id;
		if (this->musicVolume_id == this->musicVolumes.size()) {
			this->musicVolume_id = 0;
		}

		this->text_buttons["MUSIC"]->setText(this->lang["MUSIC"] + std::to_string(this->musicVolumes[this->musicVolume_id]) + "%");
		this->musicVolume = this->musicVolumes[this->musicVolume_id];
	}

	this->text_buttons["SOUNDS"]->update(this->mousePosWindow);
	if (this->text_buttons["SOUNDS"]->isPressed() && !this->getMouseClick()) {
		this->setMouseClick(true);

		++this->soundsVolume_id;
		if (this->soundsVolume_id == this->soundsVolumes.size()) {
			this->soundsVolume_id = 0;
		}

		this->text_buttons["SOUNDS"]->setText(this->lang["SOUNDS"] + std::to_string(this->soundsVolumes[this->soundsVolume_id]) + "%");
		this->soundsVolume = this->soundsVolumes[this->soundsVolume_id];
	}

	this->text_buttons["LANGUAGE"]->update(this->mousePosWindow);
	if (this->text_buttons["LANGUAGE"]->isPressed() && !this->getMouseClick()) {
		this->setMouseClick(true);

		if (this->language == "english") {
			this->language = "polish";
			this->text_buttons["LANGUAGE"]->setText(this->lang["LANGUAGE"] + this->lang["POLISH"]);
		}
		else if (this->language == "polish") {
			this->language = "english";
			this->text_buttons["LANGUAGE"]->setText(this->lang["LANGUAGE"] + this->lang["ENGLISH"]);
		}
	}

	this->text_buttons["FPS_COUNTER"]->update(this->mousePosWindow);
	if (this->text_buttons["FPS_COUNTER"]->isPressed() && !this->getMouseClick()) {
		this->setMouseClick(true);

		if (this->fpsCounterOn) {
			this->fpsCounterOn = false;
			this->text_buttons["FPS_COUNTER"]->setText(this->lang["FPS_COUNTER"] + this->lang["OFF"]);
		}
		else {
			this->fpsCounterOn = true;
			this->text_buttons["FPS_COUNTER"]->setText(this->lang["FPS_COUNTER"] + this->lang["ON"]);
		}
	}

	this->text_buttons["APPLY"]->update(this->mousePosWindow);
	if (this->text_buttons["APPLY"]->isPressed() && !this->getMouseClick()) {
		this->setMouseClick(true);
		this->gameSettings->resolution = this->mode;
		this->gameSettings->fullscreen = this->fullscreen;
		this->gameSettings->fpsLimit = this->fpsLimit;
		this->gameSettings->language = this->language;
		this->gameSettings->musicVolume = this->musicVolume;
		this->gameSettings->soundsVolume = this->soundsVolume;
		this->gameSettings->fpsCounterOn = this->fpsCounterOn;
		this->gameSettings->save();

		this->reseted = true;
	}

	this->updateMouseClick();

	this->updateKeysClick("Escape", sf::Keyboard::Escape);

	if (this->getKeysClick1("Escape") && !this->getKeysClick2("Escape")) {
		this->setKeysClick("Escape", true);
		this->endState();
	}
	this->setKeysClick("Escape", this->getKeysClick1("Escape"));
}

void SettingsState::draw(sf::RenderTarget* target)
{
	if (!target) target = this->window;

	this->texts["SETTINGS"]->draw(*target);
	this->sprites["GO_BACK"]->draw(*target);
	this->sprite_buttons["GO_BACK"]->draw(*target);
	this->text_buttons["RESOLUTION"]->draw(*target);
	this->text_buttons["FULLSCREEN"]->draw(*target);
	this->text_buttons["FPS_LIMIT"]->draw(*target);
	this->text_buttons["MUSIC"]->draw(*target);
	this->text_buttons["SOUNDS"]->draw(*target);
	this->text_buttons["LANGUAGE"]->draw(*target);
	this->text_buttons["FPS_COUNTER"]->draw(*target);
	this->text_buttons["APPLY"]->draw(*target);
}
