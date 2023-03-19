#include "Functions.h"
#include "State.h"

State::State(const float& gridSize, sf::RenderWindow* window, GameSettings* grap, std::unordered_map<std::string, int>* supportedKeys, sf::Font* font, SoundEngine* soundEngine, MusicEngine* musicEngine, std::stack<State*>* states)
	:gridSize(gridSize), window(window), gameSettings(grap), supportedKeys(supportedKeys), font(*font), soundEngine(soundEngine), musicEngine(musicEngine), states(states)
{
	this->quit = false;
	this->paused = false;
	this->mouseClick = false;
	this->keytime = 0.f;
	this->keytimeMax = 10.f;
	this->reseted = false;
	this->font = *font;

	std::ifstream f;
	if (this->gameSettings->language == "polish") {
		f.open(L"external/languages/polish.ini");
	}
	else {
		f.open(L"external/languages/english.ini");
	}
	while (!f.eof()) {
		std::string key;
		std::string value;
		f >> key;
		std::getline(f, value);
		value.erase(0, 1);
		lang[key] = sf::String::fromUtf8(value.begin(), value.end());
	}
	f.close();
}

State::~State()
{

}

const bool& State::getReseted() const
{
	return this->reseted;
}

const bool& State::getQuit() const
{
	return this->quit;
}

const bool State::getKeytime()
{
	if (this->keytime >= this->keytimeMax) {
		this->keytime = 0.f;
		return true;
	}
	return false;
}

const bool& State::getMouseClick() const
{
	return this->mouseClick;
}

bool& State::getKeysClick1(const std::string& keyName)
{
	return this->keysClick[keyName].first;
}

bool& State::getKeysClick2(const std::string& keyName)
{
	return this->keysClick[keyName].second;
}

void State::endState()
{
	this->quit = true;
}

void State::pauseState()
{
	this->paused = true;
}

void State::unpauseState()
{
	this->paused = false;
}

void State::setMouseClick(const bool& click)
{
	this->mouseClick = click;
}

void State::setKeysClick(const std::string& keyName, const bool& click)
{
	this->keysClick[keyName].second = click;
}

void State::updateMousePositions(sf::View* view)
{
	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);

	if (view) this->window->setView(*view);

	this->mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));

	this->window->setView(this->window->getDefaultView());
}

void State::updateMouseClick()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		this->mouseClick = true;
	}
	else this->mouseClick = false;
}

void State::updateKeysClick(const std::string& keyName, sf::Keyboard::Key key)
{
	this->keysClick[keyName].first = false;

	if (sf::Keyboard::isKeyPressed(key)) {
		this->keysClick[keyName].first = true;
	}
}

void State::updateKeytime(const float& dt)
{
	if (this->keytime < this->keytimeMax) {
		this->keytime += 100.f * dt;
	}
}
