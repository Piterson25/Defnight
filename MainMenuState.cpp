#include "Functions.h"
#include "MainMenuState.h"

MainMenuState::MainMenuState()
{
	this->font.loadFromFile("external/font/PressStart2P-vaV7.ttf");
	this->scale = 1.f;
	this->state = 0;
	this->fps = 0;
	this->mycha = false;
	this->select_map_texture.loadFromFile("external/assets/select_map.png");
	this->select_hero_texture.loadFromFile("external/assets/select_hero.png");
	this->select_difficulty_texture.loadFromFile("external/assets/select_difficulty.png");
	this->difficulty_texture.loadFromFile("external/assets/difficulty_icons.png");
}

MainMenuState::~MainMenuState()
{

}

void MainMenuState::init(float& scale, std::vector<sf::Keyboard::Key>& klawisze, const bool& isFullhd, const bool& isFullscreen, const bool& isVsync, const bool& isWsad, const bool& isFps)
{
	this->scale = scale;
	this->klawisze = klawisze;

	// Licznik FPSow
	createText(this->fpsCounter, this->font, 16 * this->scale, "", 4 * this->scale, 4 * this->scale, 255, 255, 255);

	// Tytul
	createSprite(this->title, this->title_texture, "external/assets/title.png", this->scale);
	this->title.setPosition(640 * this->scale, 144 * this->scale);
	center(this->title, 640 * this->scale);

	// Wyjscie
	createRectangle(this->dimBackground, 1280 * this->scale, 720 * this->scale, 0.f, 0.f, 0, 0, 0);
	this->dimBackground.setFillColor(sf::Color(0, 0, 0, 192));
	this->quitwindow = false;
	createText(this->quitText, this->font, 32 * this->scale, "", 250 * this->scale, 250 * this->scale, 255, 255, 255);
	this->quitText.setString(L"Czy na pewno chcesz wyjœæ z gry?");
	this->quitText.setPosition(640 * this->scale - this->quitText.getGlobalBounds().width / 2, this->quitText.getPosition().y);

	createText(this->yes, this->font, 32 * this->scale, "Tak", 488 * this->scale, 306 * this->scale, 255, 255, 255);

	createText(this->no, this->font, 32 * this->scale, "Nie", 704 * this->scale, 306 * this->scale, 255, 255, 255);

	// Przycisk graj
	createText(this->play, this->font, 32 * this->scale, "Graj", 580 * this->scale, 370 * this->scale, 255, 255, 255);
	// Przycisk ustawienia
	createText(this->settings, this->font, 32 * this->scale, "Ustawienia", 482 * this->scale, 466 * this->scale, 255, 255, 255);
	// Przycisk wyjdŸ
	createText(this->quit, this->font, 32 * this->scale, "", 562 * this->scale, 558 * this->scale, 255, 255, 255);
	this->quit.setString(L"WyjdŸ");
	// Wersja
	createText(this->version, this->font, 16 * this->scale, "v0.1.7", 1280 * this->scale, 720 * this->scale, 255, 255, 255);
	this->version.setPosition(1280 * this->scale - this->version.getGlobalBounds().width - 32 * this->scale, 720 * this->scale - this->version.getGlobalBounds().height - 32 * this->scale);

	// Przycisk cofnij
	createSprite(this->go_back, this->go_back_texture, "external/assets/go_back.png", 4 * this->scale);
	this->go_back.setPosition(1192 * this->scale, 24 * this->scale);

	// Ustawienia
	createText(this->settings_text, this->font, 32 * this->scale, "USTAWIENIA", 0, 96 * this->scale, 255, 255, 255);
	this->settings_text.setPosition(float(unsigned(640 * this->scale - this->settings_text.getGlobalBounds().width / 2)), this->settings_text.getPosition().y);

	this->isFullHD = isFullhd;
	this->isFullscreen = isFullscreen;
	this->isVsync = isVsync;
	this->isWsad = isWsad;
	this->isFps = isFps;
	createText(this->fullhd, this->font, 32 * this->scale, "1920x1080", 256 * this->scale, 224 * this->scale, 255, 255, 255);
	createText(this->hd, this->font, 32 * this->scale, "1280x720", 736 * this->scale, 224 * this->scale, 255, 255, 255);
	if (isFullHD) fullhd.setStyle(sf::Text::Underlined);
	else hd.setStyle(sf::Text::Underlined);

	createText(this->yesFullscreen, this->font, 32 * this->scale, "Fullscreen", 256 * this->scale, 280 * this->scale, 255, 255, 255);
	createText(this->noFullscreen, this->font, 32 * this->scale, "W oknie", 736 * this->scale, 280 * this->scale, 255, 255, 255);
	if (isFullscreen) yesFullscreen.setStyle(sf::Text::Underlined);
	else noFullscreen.setStyle(sf::Text::Underlined);

	createText(this->yesVsync, this->font, 32 * this->scale, "Vsync", 256 * this->scale, 344 * this->scale, 255, 255, 255);
	createText(this->noVsync, this->font, 32 * this->scale, "Bez Vsync", 736 * this->scale, 344 * this->scale, 255, 255, 255);
	if (isVsync) yesVsync.setStyle(sf::Text::Underlined);
	else noVsync.setStyle(sf::Text::Underlined);

	createText(this->wsad, this->font, 32 * this->scale, "WSAD", 256 * this->scale, 408 * this->scale, 255, 255, 255);
	createText(this->strzalki, this->font, 32 * this->scale, "Strzalki", 736 * this->scale, 408 * this->scale, 255, 255, 255);
	this->strzalki.setString(L"Strza³ki");
	if (isWsad) wsad.setStyle(sf::Text::Underlined);
	else strzalki.setStyle(sf::Text::Underlined);

	createText(this->fpsOn, this->font, 32 * this->scale, "", 256 * this->scale, 472 * this->scale, 255, 255, 255);
	this->fpsOn.setString(L"W³acz FPS");
	createText(this->fpsOff, this->font, 32 * this->scale, "", 736 * this->scale, 472 * this->scale, 255, 255, 255);
	this->fpsOff.setString(L"Wy³acz FPS");
	if (isFps) fpsOn.setStyle(sf::Text::Underlined);
	else fpsOff.setStyle(sf::Text::Underlined);

	createText(this->apply, this->font, 32 * this->scale, "Zastosuj", 600 * this->scale, 624 * this->scale, 255, 255, 255);
	this->apply.setPosition(640 * this->scale - this->apply.getGlobalBounds().width / 2, this->apply.getPosition().y);

	createText(this->closequit, this->font, 16 * this->scale, "", 600 * this->scale, 200 * this->scale, 182, 60, 53);
	this->closequit.setString(L"Uruchom ponownie gre aby zastosowaæ");
	this->closequit.setPosition(float(unsigned(640 * this->scale - this->closequit.getGlobalBounds().width / 2)), this->closequit.getPosition().y);
	this->applybool = false;

	// Wybór mapy
	createText(this->chooseMap_text, this->font, 32 * this->scale, "WYBIERZ MAPE:", 0, 96 * this->scale, 255, 255, 255);
	center(this->chooseMap_text, 640 * this->scale);

	// Przycisk pierwszej mapy
	createSprite(this->select_map, this->select_map_texture, this->scale);
	this->select_map.setPosition(24 * this->scale, 248 * this->scale);
	
	// Miniaturka pierwszej mapy
	createSprite(this->mapa1, this->background_texture, "external/assets/background.png", 0.5f * this->scale);
	this->mapa1.setPosition(48 * this->scale, 272 * this->scale);
	// Napis pierwszej mapy
	createText(this->mapName, this->font, 32 * this->scale, "Ruiny", 300 * this->scale, 200 * this->scale, 255, 255, 255);
	this->mapName.setPosition(float(unsigned(this->mapa1.getPosition().x + 128 * this->scale - this->mapName.getGlobalBounds().width / 2)), this->mapName.getPosition().y);


	// Wybór postaci
	createText(this->chooseClass_text, this->font, 32 * this->scale, "WYBIERZ POSTAC:", 0, 96 * this->scale, 255, 255, 255);
	center(this->chooseClass_text, 640 * this->scale);

	// Przycisk pierwszej postaci
	createSprite(this->select_hero, this->select_hero_texture, this->scale);
	this->select_hero.setPosition(64 * this->scale, 256 * this->scale);
	// Miniaturka wojownika
	createSprite(this->wojownik, this->player, "external/assets/heroes/wojownik.png", 1.f);
	this->wojownik.setTextureRect(sf::IntRect(0, 32, 16, 16));
	this->wojownik.setScale(8 * this->scale, 8 * this->scale);
	this->wojownik.setPosition(88 * this->scale, 272 * this->scale);
	// Napis wojownika
	createText(this->className, this->font, 32 * this->scale, "Wojownik", 300 * this->scale, 200 * this->scale, 255, 255, 255);
	this->className.setPosition(float(unsigned(this->wojownik.getPosition().x + 64 * this->scale - this->className.getGlobalBounds().width / 2)), this->className.getPosition().y);

	// Poziom trudnoœci
	createText(this->chooseDifficulty_text, this->font, 32 * this->scale, "WYBIERZ TRUDNOSC:", 0, 96 * this->scale, 255, 255, 255);
	center(this->chooseDifficulty_text, 640 * this->scale);

	createSprite(this->easy, this->difficulty_texture, 16 * this->scale, 168 * this->scale, 272 * this->scale);
	this->easy.setTextureRect(sf::IntRect(0, 0, 7, 6));
	createSprite(this->normal, this->difficulty_texture, 16 * this->scale, 584 * this->scale, 272 * this->scale);
	this->normal.setTextureRect(sf::IntRect(7, 0, 7, 6));
	createSprite(this->hard, this->difficulty_texture, 16 * this->scale, 1000 * this->scale, 272 * this->scale);
	this->hard.setTextureRect(sf::IntRect(14, 0, 7, 6));

	createText(this->easy_text, this->font, 32 * this->scale, "LATWY", 224 * this->scale, 400 * this->scale, 255, 255, 255);
	center(this->easy_text, 224 * this->scale);
	createText(this->normal_text, this->font, 32 * this->scale, "NORMALNY", 640 * this->scale, 400 * this->scale, 255, 255, 255);
	center(this->normal_text, 640 * this->scale);
	createText(this->hard_text, this->font, 32 * this->scale, "TRUDNY", 1056 * this->scale, 400 * this->scale, 255, 255, 255);
	center(this->hard_text, 1056 * this->scale);

	createSprite(this->select_easy, this->select_difficulty_texture, this->scale, 32 * this->scale, 184 * this->scale);
	createSprite(this->select_normal, this->select_difficulty_texture, this->scale, 448 * this->scale, 184 * this->scale);
	createSprite(this->select_hard, this->select_difficulty_texture, this->scale, 864 * this->scale, 184 * this->scale);

	createText(this->easy_desc, this->font, 16 * this->scale, "Potwory maja:\n\n-25% HP\n\n-25% atak", 224 * this->scale, 460 * this->scale, 182, 60, 53);
	center(this->easy_desc, 224 * this->scale);
	createText(this->normal_desc, this->font, 16 * this->scale, "Brak zmiany u potworów", 640 * this->scale, 460 * this->scale, 182, 60, 53);
	center(this->normal_desc, 640 * this->scale);
	createText(this->hard_desc, this->font, 16 * this->scale, "Potwory maja:\n\n+25% HP\n\n+25% atak", 1056 * this->scale, 460 * this->scale, 182, 60, 53);
	center(this->hard_desc, 1056 * this->scale);

}

void MainMenuState::update(sf::RenderWindow& window, const float& dt)
{
	if (this->isFps) {

		this->fps++;

		if (this->fpsClock.getElapsedTime().asSeconds() >= 1.f) {
			this->fpsCounter.setString("FPS:" + std::to_string(int(this->fps)));
			this->fps = 0;
			this->fpsClock.restart();
		}
	}

	

	if (this->state == 0) {
		this->state0(window);
	}
	else if (this->state == 1) {
		this->state1(window);
	}
	else if (this->state == 2) {
		this->state2(window);
	}
	else if (this->state == 3) {
		this->state3(window);
	}
	else if (this->state == 4) {
		this->gra.update(window, dt, this->state);
	}
	else if (this->state == 5) {
		this->state5(window);
	}


	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) == false) this->mycha = false;
	else this->mycha = true;
}

void MainMenuState::draw(sf::RenderWindow& window)
{
	if (this->state == 0) {
		window.draw(this->title);
		window.draw(this->play);
		window.draw(this->settings);
		window.draw(this->quit);
		window.draw(this->version);
		if (this->quitwindow) {
			window.draw(this->dimBackground);
			window.draw(this->quitText);
			window.draw(this->yes);
			window.draw(this->no);
		}
	}
	else if (this->state == 1) {
		window.draw(this->chooseMap_text);
		window.draw(this->select_map);
		window.draw(this->mapa1);
		window.draw(this->mapName);
		window.draw(this->go_back);
	}
	else if (this->state == 2) {
		window.draw(this->chooseClass_text);
		window.draw(this->select_hero);
		window.draw(this->wojownik);
		window.draw(this->className);
		window.draw(this->go_back);
		//window.draw(this->test);
	}
	else if (this->state == 3) {
		window.draw(this->chooseDifficulty_text);
		window.draw(this->easy);
		window.draw(this->easy_text);
		window.draw(this->select_easy);
		window.draw(this->easy_desc);
		window.draw(this->normal);
		window.draw(this->normal_text);
		window.draw(this->select_normal);
		window.draw(this->normal_desc);
		window.draw(this->hard);
		window.draw(this->hard_text);
		window.draw(this->select_hard);
		window.draw(this->hard_desc);
		window.draw(this->go_back);
	}
	else if (this->state == 4) {
		this->gra.draw(window);
	}
	else if (this->state == 5) {
		window.draw(this->settings_text);
		window.draw(this->go_back);
		window.draw(this->fullhd);
		window.draw(this->hd);
		window.draw(this->yesFullscreen);
		window.draw(this->noFullscreen);
		window.draw(this->yesVsync);
		window.draw(this->noVsync);
		window.draw(this->wsad);
		window.draw(this->strzalki);
		window.draw(this->fpsOn);
		window.draw(this->fpsOff);
		window.draw(this->apply);
		if (this->applybool)
			window.draw(this->closequit);
		
	}

	if (this->isFps) window.draw(this->fpsCounter);
}

void MainMenuState::buttonText(sf::RenderWindow& window, sf::Text& text, const std::string& word)
{
	if (text.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
		text.setFillColor(sf::Color(192, 192, 192));
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->mycha == false) {
			if (word == "start") this->state = 1;
			else if (word == "settings") this->state = 5;
			else if (word == "zastosuj") {
				//this->saveConfig();
				//this->loadConfig(window);
				this->applybool = true;
			}
			else if (word == "quit") this->quitwindow = true;
			else if (word == "tak") window.close();
			else if (word == "nie") this->quitwindow = false;
			this->mycha = true;
			text.setFillColor(sf::Color(255, 255, 255));
		}
	}
	else {
		text.setFillColor(sf::Color(255, 255, 255));
	}
}

void MainMenuState::buttonTextChoice(sf::RenderWindow& window, sf::Text& text, const std::string& word)
{
	if (text.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
		text.setFillColor(sf::Color(192, 192, 192));
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->mycha == false) {
			if (word == "fullhd") {
				this->isFullHD = true;
				this->hd.setStyle(sf::Text::Regular);
			}
			else if (word == "hd") {
				this->isFullHD = false;
				this->fullhd.setStyle(sf::Text::Regular);
			}
			else if (word == "fullscreen") {
				this->isFullscreen = true;
				this->noFullscreen.setStyle(sf::Text::Regular);
			}
			else if (word == "niefullscreen") {
				this->isFullscreen = false;
				this->yesFullscreen.setStyle(sf::Text::Regular);
			}
			else if (word == "vsync") {
				this->isVsync = true;
				this->noVsync.setStyle(sf::Text::Regular);
			}
			else if (word == "nievsync") {
				this->isVsync = false;
				this->yesVsync.setStyle(sf::Text::Regular);
			}
			else if (word == "wsad") {
				this->isWsad = true;
				this->strzalki.setStyle(sf::Text::Regular);
			}
			else if (word == "strzalki") {
				this->isWsad = false;
				this->wsad.setStyle(sf::Text::Regular);
			}
			else if (word == "fpson") {
				this->isFps = true;
				this->fpsOff.setStyle(sf::Text::Regular);
			}
			else if (word == "fpsoff") {
				this->isFps = false;
				this->fpsOn.setStyle(sf::Text::Regular);
			}
			this->mycha = true;
			text.setStyle(sf::Text::Underlined);
			text.setFillColor(sf::Color(255, 255, 255));
		}
	}
	else {
		text.setFillColor(sf::Color(255, 255, 255));
	}
}

void MainMenuState::buttonSprite(sf::RenderWindow& window, sf::Sprite& spr, const std::string& word)
{
	if (spr.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
		if (word == "powrot") {
			spr.setColor(sf::Color(192, 192, 192));
		}
		else spr.setColor(sf::Color::White);
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->mycha == false) {

			if (word == "mapa") this->state = 2;
			else if (word == "wojownik") {
				this->state = 3;
			}
			else if (word == "latwy") {
				this->state = 4;
				this->gra.init(this->scale, this->klawisze, "Ruiny", "Wojownik", "Latwy");
			}
			else if (word == "normalny") {
				this->state = 4;
				this->gra.init(this->scale, this->klawisze, "Ruiny", "Wojownik", "Normalny");
			}
			else if (word == "trudny") {
				this->state = 4;
				this->gra.init(this->scale, this->klawisze, "Ruiny", "Wojownik", "Trudny");
			}
			else if (word == "powrot") {
				if (this->state == 5)
					this->state = 0;
				else this->state--;
			}
			this->mycha = true;

			if (word == "powrot") {
				spr.setColor(sf::Color::White);
			}
			else spr.setColor(sf::Color::Transparent);
		}
	}
	else {
		if (word == "powrot") {
			spr.setColor(sf::Color::White);
		}
		else spr.setColor(sf::Color::Transparent);
	}
}

void MainMenuState::state0(sf::RenderWindow& window)
{
	if (this->quitwindow == false) {
		buttonText(window, this->play, "start");
		buttonText(window, this->settings, "settings");
		buttonText(window, this->quit, "quit");
	}
	else {
		buttonText(window, this->yes, "tak");
		buttonText(window, this->no, "nie");
	}
}

void MainMenuState::state1(sf::RenderWindow& window)
{
	buttonSprite(window, this->select_map, "mapa");
	buttonSprite(window, this->go_back, "powrot");
}

void MainMenuState::state2(sf::RenderWindow& window)
{
	buttonSprite(window, this->select_hero, "wojownik");
	buttonSprite(window, this->go_back, "powrot");
}

void MainMenuState::state3(sf::RenderWindow& window)
{
	buttonSprite(window, this->select_easy, "latwy");
	buttonSprite(window, this->select_normal, "normalny");
	buttonSprite(window, this->select_hard, "trudny");
	buttonSprite(window, this->go_back, "powrot");
}

void MainMenuState::state5(sf::RenderWindow& window)
{
	buttonSprite(window, this->go_back, "powrot");
	buttonTextChoice(window, this->fullhd, "fullhd");
	buttonTextChoice(window, this->hd, "hd");
	buttonTextChoice(window, this->yesFullscreen, "fullscreen");
	buttonTextChoice(window, this->noFullscreen, "niefullscreen");
	buttonTextChoice(window, this->yesVsync, "vsync");
	buttonTextChoice(window, this->noVsync, "nievsync");
	buttonTextChoice(window, this->wsad, "wsad");
	buttonTextChoice(window, this->strzalki, "strzalki");
	buttonTextChoice(window, this->fpsOn, "fpson");
	buttonTextChoice(window, this->fpsOff, "fpsoff");
	buttonText(window, this->apply, "zastosuj");
}
