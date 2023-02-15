#ifndef BOSS_H
#define BOSS_H

#include "Monster.h"
#include "TileMap.h"

class TileMap;

class Boss
	: public Monster
{
public:
	Boss(const sf::VideoMode& vm, const std::string& monster_name, sf::Texture& texture, sf::Texture& shadow_texture, TileMap* tileMap, 
		const float& x, const float& y, const float& difficulty_mod, const float& wave_mod, const uint32_t& monsterSize);
	virtual ~Boss();

	void update(const float& dt);
	void draw(sf::RenderTarget& target);
	void drawShadow(sf::RenderTarget& target);
private:
	uint32_t monsterSize;
};

#endif