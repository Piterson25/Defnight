#ifndef SETTINGSSTATE_H
#define SETTINGSSTATE_H
#include "State.h"
#include "GUI.h"

class SettingsState :
	public State
{
public:
	SettingsState(const float& gridSize, sf::RenderWindow* window, GameSettings* grap,
		std::unordered_map<std::string, int>* supportedKeys, sf::Font* font, SoundEngine* soundEngine, MusicEngine* musicEngine, std::stack<State*>* states);
	virtual ~SettingsState();

	void initGUI();
	void resetGUI();

	void update(const float& dt);
	void draw(sf::RenderTarget* target = NULL);
private:
	std::vector<sf::VideoMode> videoModes;

	sf::VideoMode mode;
	size_t id;
	size_t fps_id;
	bool fullscreen;
	uint16_t fpsLimit;
	std::vector<uint16_t> fpsLimits;
	bool fpsCounterOn;
	float musicVolume;
	size_t musicVolume_id;
	std::vector<uint16_t> musicVolumes;
	float soundsVolume;
	size_t soundsVolume_id;
	std::vector<uint16_t> soundsVolumes;
	std::string language;

	std::unordered_map<std::string, gui::ButtonText*> text_buttons;
	std::unordered_map<std::string, gui::Text*> texts;
	std::unordered_map<std::string, gui::ButtonSprite*> sprite_buttons;
	std::unordered_map<std::string, gui::Sprite*> sprites;
};

#endif