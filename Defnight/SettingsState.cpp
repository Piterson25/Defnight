#include "Functions.h"
#include "SettingsState.h"

SettingsState::SettingsState(const float& gridSize, sf::RenderWindow* window, GraphicsSettings* grap, 
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
	const sf::VideoMode vm = this->graphicsSettings->resolution;

	this->mode = vm;

	this->id = 0;

	this->keysClick["Escape"].first = false;
	this->keysClick["Escape"].second = false;

	for (auto& v : sf::VideoMode::getFullscreenModes()) {
		if (static_cast<float>(v.width) / v.height == 16.f / 9.f) this->videoModes.push_back(v);
	}

	this->texts["SETTINGS"] = new gui::Text(&this->font, this->lang["SETTINGS"], calcChar(32, vm), calcX(640, vm), calcY(96, vm), sf::Color(255, 255, 255), true);
	this->sprites["GO_BACK"] = new gui::Sprite("external/assets/go_back.png", calcX(1192, vm), calcY(24, vm), calcX(4, vm), false);
	this->sprite_buttons["GO_BACK"] = new gui::ButtonSprite("external/assets/select_go_back.png", calcX(1192, vm), calcY(24, vm), calcX(4, vm), false);

	for (size_t i = 0; i < this->videoModes.size(); ++i) {
		if (this->videoModes[i] == vm) {
			this->id = i;
			this->text_buttons["RESOLUTION"] = new gui::ButtonText(&this->font, this->lang["RESOLUTION"] + std::to_string(this->videoModes[this->id].width) + "x" + std::to_string(this->videoModes[this->id].height), calcChar(32, vm), calcX(96, vm), calcY(192, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
			break;
		}
	}

	if (this->graphicsSettings->fullscreen) {
		this->fullscreen = true;
		this->text_buttons["FULLSCREEN"] = new gui::ButtonText(&this->font, this->lang["FULLSCREEN"] + this->lang["YES"], calcChar(32, vm), calcX(96, vm), calcY(292, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
	}
	else {
		this->fullscreen = false;
		this->text_buttons["FULLSCREEN"] = new gui::ButtonText(&this->font, this->lang["FULLSCREEN"] + this->lang["NO"], calcChar(32, vm), calcX(96, vm), calcY(292, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
	}

	this->fpsLimit = this->graphicsSettings->fpsLimit;

	this->fpsLimits.push_back(60);
	this->fpsLimits.push_back(120);
	this->fpsLimits.push_back(240);
	this->fpsLimits.push_back(0);

	this->fps_id = 0;

	for (size_t i = 0; i < this->fpsLimits.size(); ++i) {
		if (this->fpsLimits[i] == this->graphicsSettings->fpsLimit) {
			this->fps_id = i;
			if (this->fpsLimits[i] == 0) 
				this->text_buttons["FPS_LIMIT"] = new gui::ButtonText(&this->font, this->lang["FPS_LIMIT"] + this->lang["NO_LIMIT"], calcChar(32, vm), calcX(96, vm), calcY(392, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
			else 
				this->text_buttons["FPS_LIMIT"] = new gui::ButtonText(&this->font, this->lang["FPS_LIMIT"] + std::to_string(this->fpsLimits[this->fps_id]), calcChar(32, vm), calcX(96, vm), calcY(392, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
			break;
		}
	}

	if (this->graphicsSettings->language == "english") {
		this->language = "english";
		this->text_buttons["LANGUAGE"] = new gui::ButtonText(&this->font, this->lang["LANGUAGE"] + this->lang["ENGLISH"], calcChar(32, vm), calcX(96, vm), calcY(492, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
	}
	else if (this->graphicsSettings->language == "polish") {
		this->language = "polish";
		this->text_buttons["LANGUAGE"] = new gui::ButtonText(&this->font, this->lang["LANGUAGE"] + this->lang["POLISH"], calcChar(32, vm), calcX(96, vm), calcY(492, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), false);
	}

	this->text_buttons["APPLY"] = new gui::ButtonText(&this->font, this->lang["APPLY"], calcChar(32, vm), calcX(640, vm), calcY(592, vm), sf::Color(255, 255, 255), sf::Color(192, 192, 192), true);
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

	this->text_buttons["APPLY"]->update(this->mousePosWindow);
	if (this->text_buttons["APPLY"]->isPressed() && !this->getMouseClick()) {
		this->setMouseClick(true);
		this->graphicsSettings->resolution = this->mode;
		this->graphicsSettings->fullscreen = this->fullscreen;
		this->graphicsSettings->fpsLimit = this->fpsLimit;
		this->graphicsSettings->language = this->language;
		this->graphicsSettings->save();

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
	this->text_buttons["LANGUAGE"]->draw(*target);
	this->text_buttons["APPLY"]->draw(*target);
}
