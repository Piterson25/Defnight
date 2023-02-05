#ifndef PLAYER_H
#define PLAYER_H

#include "MonsterSystem.h"
#include "Entity.h"
#include "ProjectileSystem.h"
#include "FloatingTextSystem.h"
#include "SoundEngine.h"

class MonsterSystem;
class FloatingTextSystem;
class ProjectileSystem;
class SoundEngine;

class Player: 
	public Entity
{
public:
	Player(const sf::VideoMode& vm, const std::string& hero_name, const float& x, const float& y);
	virtual ~Player();

	virtual const uint32_t getGold() const;
	virtual const uint32_t getArmor() const;
	virtual const uint32_t getReg() const;
	virtual const uint32_t getLevel() const;
	virtual const uint32_t getMaxXP() const;
	virtual const float getSprint() const;
	virtual const uint32_t getMaxSprint() const;
	virtual const uint32_t getLastMaxXP() const;
	virtual const uint32_t getCriticalChance() const;
	virtual const uint32_t getKills() const;
	virtual const bool getSprinting() const;
	virtual const bool getRegenerating() const;
	virtual const bool getLeveling() const;
	virtual const bool getSpawned() const;
	virtual const bool getAbilityActive() const;
	virtual const float getAbilityCooldown() const;
	virtual const float getAbilityTime() const;
	virtual const float getAbilityMaxTime() const;

	void setGold(const uint32_t& gold);
	void setArmor(const uint32_t& armor);
	void setReg(const uint32_t& reg);
	void setSprint(const float& sprint);
	void setMaxSprint(const uint32_t& maxSprint);
	void setCriticalChance(const uint32_t& criticalChance);
	void setKills(const uint32_t& kills);
	void setIsSprinting(const bool& isSprinting);
	void setIsRegenerating(const bool& isRegenerating);
	void setIsLeveling(const bool& isLeveling);
	void setAbilityCooldown(const float& abilityCooldown);
	void setAbilityTime(const float& abilityTime);
	void setAbilityMaxTime(const float& abilityMaxTime);

	void attackMonster(MonsterSystem* monsterSystem, FloatingTextSystem* floatingTextSystem, SoundEngine* soundEngine);
	const bool addXP(const uint32_t& monsterXP);
	void spawn(const float& dt);
	void controls(const std::unordered_map<std::string, int>& keybinds, const float& dt);
	const bool regeneration(const float& dt);
	void abilityCounter(const float& dt);
	const bool checkIfAbility();
	void doAbility(const sf::Vector2f& coords, ProjectileSystem* projectileSystem, SoundEngine* soundEngine);
	void whooshSound(SoundEngine* soundEngine);

	void updateSprint(const float& dt);
	void update(const float& dt);
	void draw(sf::RenderTarget& target);
	void drawShadow(sf::RenderTarget& target);
private:
	sf::Sprite shadow;
	sf::Texture shadow_texture;
	sf::Sprite ability;
	sf::Texture ability_texture;

	uint32_t gold;
	uint32_t armor;
	uint32_t reg;
	uint32_t level;
	uint32_t maxXP;
	uint32_t lastMaxXP;
	float sprint;
	uint32_t maxSprint;
	uint32_t criticalChance;
	uint32_t kills;
	bool isRegenerating;
	bool isLeveling;
	bool isSprinting;
	bool spawned;
	float regCooldown;
	float spawnCountdown;

	bool abilityActive;
	float abilityCooldown;
	float abilityTime;
	float abilityMaxTime;
	
	bool playedSound;
};

#endif