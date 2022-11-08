#ifndef PLAYER_H
#define PLAYER_H

#include "Monster.h"
#include "Entity.h"
#include "FloatingText.h"
#include "SoundEngine.h"

class Monster;
class FloatingText;
class Projectile;
class SoundEngine;

class Player: 
	public Entity
{
public:
	Player(const float& x, const float& y, const sf::VideoMode& vm, const std::string& hero_name);
	virtual ~Player();

	virtual const uint16_t getGold() const;
	virtual const uint16_t getAttack() const;
	virtual const uint16_t getKills() const;
	virtual const bool getSpawned() const;
	virtual const uint16_t getArmor() const;
	virtual const unsigned getReg() const;
	virtual const unsigned getMaxXP() const;
	virtual const unsigned getLastMaxXP() const;
	virtual const unsigned getCriticalChance() const;
	virtual const unsigned getLevel() const;
	virtual const bool getRegenerating() const;
	virtual const bool getLeveling() const;
	virtual const float getAbilityCooldown() const;
	virtual const float getAbilityMaxTime() const;
	virtual const float getAbilityTime() const;
	virtual const bool getAbilityActive() const;

	void setGold(const uint16_t& gold);
	void setAttack(const uint16_t& attack);
	void setKills(const uint16_t& kills);
	void setCriticalChance(const unsigned& CriticalChance);
	void setReg(const unsigned& reg);
	void setArmor(const uint16_t& armor);
	void setIsRegenerating(const bool& isRegenerating);
	void setIsLeveling(const bool& isLeveling);
	void setAbilityMaxTime(const float& abilityMaxTime);
	void setAbilityTime(const float& abilityTime);
	void setAbilityCooldown(const float& abilityCooldown);

	void attackMonster(sf::Font* font, const std::list<Monster*>& monsters, std::list<FloatingText*>& floatingTexts, SoundEngine* soundEngine);
	const bool addXP(const unsigned& monsterXP);
	void spawn(const float& dt);
	void controls(const std::unordered_map<std::string, int>& keybinds, const float& dt);
	const bool regeneration(const float& dt);
	void abilityCounter(const float& dt);
	const bool checkIfAbility();
	void doAbility(const sf::Vector2f& coords, std::list<Projectile*>& projectiles);
	void swipeSound(SoundEngine* soundEngine);

	void update(const float& dt);
	void draw(sf::RenderTarget& target);
	void drawShadow(sf::RenderTarget& target);
private:
	sf::Sprite shadow;
	sf::Texture shadow_texture;
	sf::Sprite ability;
	sf::Texture ability_texture;

	uint16_t kills;
	unsigned level;
	unsigned criticalChance;
	unsigned maxXP;
	unsigned lastMaxXP;
	unsigned reg;
	uint16_t armor;
	bool isRegenerating;
	bool isLeveling;
	bool spawned;
	float regCooldown;
	float spawnCountdown;

	uint16_t attack;
	uint16_t gold;

	bool abilityActive;
	float abilityCooldown;
	float abilityTime;
	float abilityMaxTime;
	
	bool playedSound;
};

#endif