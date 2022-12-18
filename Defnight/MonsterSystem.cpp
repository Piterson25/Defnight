#include "stdafx.h"
#include "Functions.h"
#include "MonsterSystem.h"

MonsterSystem::MonsterSystem(const sf::VideoMode& vm, TileMap* tileMap, const float& gridSize, const float& difficulty_mod)
	:vm(vm), gridSize(gridSize), difficulty_mod(difficulty_mod)
{
	this->tileMap = tileMap;
	this->bossWave = false;
	this->shadow_texture.loadFromFile("external/assets/entity_shadow.png");

	sf::Texture texture;
	texture.loadFromFile("external/assets/monsters/goblin.png");
	this->textures["GOBLIN"] = texture;
	texture.loadFromFile("external/assets/monsters/spider.png");
	this->textures["SPIDER"] = texture;
	texture.loadFromFile("external/assets/monsters/orc.png");
	this->textures["ORC"] = texture;
	texture.loadFromFile("external/assets/monsters/cyclope.png");
	this->textures["CYCLOPE"] = texture;
	texture.loadFromFile("external/assets/monsters/minotaur.png");
	this->textures["MINOTAUR"] = texture;
}

MonsterSystem::~MonsterSystem()
{
	this->monsters.clear();
}

const bool MonsterSystem::monsterIDsEmpty() const
{
	return this->monsterIDs.empty();
}

const size_t MonsterSystem::monsterIDsSize() const
{
	return this->monsterIDs.size();
}

const bool MonsterSystem::monstersEmpty() const
{
	return this->monsters.empty();
}

const bool MonsterSystem::isBossWave() const
{
	return this->bossWave;
}

const float MonsterSystem::bossHP() const
{
	for (const auto& monster : monsters) {
		if (monster->getName() == "minotaur") {
			return static_cast<float>(static_cast<float>(monster->getHP()) / static_cast<float>(monster->getMaxHP()));
		}
	}
	return 0.f;
}

void MonsterSystem::monsterIDsClear()
{
	this->monsterIDs.clear();
}

void MonsterSystem::addMonster(const std::string& monster_name, sf::Texture& texture, const float& x, const float& y, const float& wave_mod)
{
	this->monsters.emplace_back(new Monster(this->vm, monster_name, texture, this->shadow_texture, this->tileMap, x, y, this->difficulty_mod, wave_mod));
}

void MonsterSystem::playerAttack(Player* player, FloatingTextSystem* floatingTextSystem, SoundEngine* soundEngine, bool& playedSound)
{
	for (const auto& monster : monsters) {
		const float distance = player->attackDistance(&(*monster), player);

		if ((player->hasVelocity() && distance <= player->getReach() * calcX(32, vm)) || (!player->hasVelocity() && distance <= player->getReach() * calcX(48, vm))) {

			if (!monster->isDead() && !monster->getPunched() && monster->getSpawned() && player->getIsAttacking() && player->getFrame() == 80) {
				if ((uint32_t(Random::Float() * 100.f) + 1) <= player->getCriticalChance()) {
					const int attack = 2 * player->getAttack();
					floatingTextSystem->addFloatingText(std::to_string(-attack), calcChar(16, vm), monster->getPosition().x + calcX(32, vm), monster->getPosition().y + calcY(32, vm), sf::Color(233, 134, 39), false);
					if (static_cast<int>(monster->getHP() - attack) < 0) monster->setHP(0);
					else monster->setHP(monster->getHP() - attack);
				}
				else {
					const int attack = player->getAttack();
					floatingTextSystem->addFloatingText(std::to_string(-attack), calcChar(16, vm), monster->getPosition().x + calcX(32, vm), monster->getPosition().y + calcY(32, vm), sf::Color(255, 255, 255), false);
					if (static_cast<int>(monster->getHP() - attack) < 0) monster->setHP(0);
					else monster->setHP(monster->getHP() - attack);
				}

				if (!playedSound) {
					soundEngine->addSound("whoosh_hit");
					playedSound = true;
				}

				monster->punch();
				break;
			}

		}
	}
}

void MonsterSystem::explosionAttack(Particle* particle, FloatingTextSystem* floatingTextSystem)
{
	for (const auto& monster : monsters) {
		if (monster->getGlobalBounds().intersects(particle->getGlobalBounds())) {
			if (!monster->isDead() && !monster->getPunched() && monster->getSpawned()) {
				const int attack = particle->getAttack();
				floatingTextSystem->addFloatingText(std::to_string(-attack), calcChar(16, vm), monster->getPosition().x + calcX(32, vm), monster->getPosition().y + calcY(32, vm), sf::Color(255, 255, 255), false);
				if (static_cast<int>(monster->getHP() - attack) < 0) monster->setHP(0);
				else monster->setHP(monster->getHP() - attack);

				monster->punch();
			}
		}
	}
}

void MonsterSystem::projectileCollision(Projectile* proj, Player* player, FloatingTextSystem* floatingTextSystem)
{
	for (const auto& monster : monsters) {
		proj->monsterCollision(&(*monster), player, floatingTextSystem);
		if (proj->getCollidedMonster()) break;
	}
}

void MonsterSystem::monsterCollision(Monster* mob)
{
	for (const auto& monster : monsters) {
		if (vectorDistance(mob->getPosition(), monster->getPosition()) < 2 * monster->getGlobalBounds().width) {
			sf::FloatRect mobBounds = mob->getGlobalBounds();
			sf::FloatRect monsterBounds = monster->getGlobalBounds();
		
			sf::FloatRect nextPos = mobBounds;
			nextPos.left += mob->getVelocity().x;
			nextPos.top += mob->getVelocity().y;
		
			if (monsterBounds.intersects(nextPos))
			{
				//Dolna kolizja
				if (mobBounds.top < monsterBounds.top
					&& mobBounds.top + mobBounds.height < monsterBounds.top + monsterBounds.height
					&& mobBounds.left < monsterBounds.left + monsterBounds.width
					&& mobBounds.left + mobBounds.width > monsterBounds.left)
				{
					mob->setVeloctiy(sf::Vector2f(mob->getVelocity().x, 0.f));
					mob->setPosition(mobBounds.left, monsterBounds.top - mobBounds.height);
				}
				//Gorna kolizja
				else if (mobBounds.top > monsterBounds.top
					&& mobBounds.top + mobBounds.height > monsterBounds.top + monsterBounds.height
					&& mobBounds.left < monsterBounds.left + monsterBounds.width
					&& mobBounds.left + mobBounds.width > monsterBounds.left)
				{
					mob->setVeloctiy(sf::Vector2f(mob->getVelocity().x, 0.f));
					mob->setPosition(mobBounds.left, monsterBounds.top + monsterBounds.height);
				}
		
				//Prawa kolizja
				else if (mobBounds.left < monsterBounds.left
					&& mobBounds.left + mobBounds.width < monsterBounds.left + monsterBounds.width
					&& mobBounds.top < monsterBounds.top + monsterBounds.height
					&& mobBounds.top + mobBounds.height > monsterBounds.top)
				{
					mob->setVeloctiy(sf::Vector2f(0.f, mob->getVelocity().y));
					mob->setPosition(monsterBounds.left - mobBounds.width, mobBounds.top);
				}
				//Lewa kolizja
				else if (mobBounds.left > monsterBounds.left
					&& mobBounds.left + mobBounds.width > monsterBounds.left + monsterBounds.width
					&& mobBounds.top < monsterBounds.top + monsterBounds.height
					&& mobBounds.top + mobBounds.height > monsterBounds.top)
				{
					mob->setVeloctiy(sf::Vector2f(0.f, mob->getVelocity().y));
					mob->setPosition(monsterBounds.left + monsterBounds.width, mobBounds.top);
				}
			}
		}
	}
}

void MonsterSystem::spawnMonsters(Player* player, const uint32_t& wave)
{
	const float wave_mod = 1.f + static_cast<uint32_t>(wave / 10.f);

	for (const auto& id : this->monsterIDs) {
		uint32_t rx = static_cast<uint32_t>(Random::Float() * 32.f), ry = static_cast<uint32_t>(Random::Float() * 32.f);
		for (size_t i = 0; i < this->tileMap->getSize(); ++i) {
			while ((this->tileMap->getPosition(i).x == calcX(this->gridSize * rx, this->vm) && this->tileMap->getPosition(i).y == calcY(this->gridSize * ry, vm)) ||
				vectorDistance(calcX(this->gridSize * rx, this->vm), calcY(this->gridSize * ry, this->vm), player->getPosition().x, player->getPosition().y) <= calcX(3.f * this->gridSize, this->vm))
			{
				rx = static_cast<uint32_t>(Random::Float() * 30.f) + 1;
				ry = static_cast<uint32_t>(Random::Float() * 30.f) + 1;
				i = 0;
			}
			for (const auto& mob : this->monsters) {
				if (mob->getPosition().x == calcX(this->gridSize * rx, this->vm) && mob->getPosition().y == calcY(this->gridSize * ry, this->vm)) {
					rx = static_cast<uint32_t>(Random::Float() * 30.f) + 1;
					ry = static_cast<uint32_t>(Random::Float() * 30.f) + 1;
					i = 0;
				}
			}
		}

		switch (id)
		{
		case 0:
			this->addMonster("goblin", this->textures["GOBLIN"], calcX(this->gridSize * rx, this->vm), calcY(this->gridSize * ry, this->vm), wave_mod);
			break;
		case 1:
			this->addMonster("spider", this->textures["SPIDER"], calcX(this->gridSize * rx, this->vm), calcY(this->gridSize * ry, this->vm), wave_mod);
			break;
		case 2:
			this->addMonster("orc", this->textures["ORC"], calcX(this->gridSize * rx, this->vm), calcY(this->gridSize * ry, this->vm), wave_mod);
			break;
		case 3:
			this->addMonster("cyclope", this->textures["CYCLOPE"], calcX(this->gridSize * rx, this->vm), calcY(this->gridSize * ry, this->vm), wave_mod);
			break;
		case 4:
			this->addMonster("minotaur", this->textures["MINOTAUR"], calcX(this->gridSize * 16, this->vm), calcY(this->gridSize * 16, this->vm), wave_mod);
			break;
		default:
			break;
		}
	}
}

void MonsterSystem::prepareWave(uint32_t& wave, uint32_t& sumHP)
{
	wave++;
	const uint32_t wave_mod = 1 + static_cast<uint32_t>(wave / 10.f);
	if (wave % 10 != 0) sumHP += static_cast<uint32_t>((2 - ((1 + sqrtf(5)) / 2.f)) * sumHP);
	uint32_t monstersHP = sumHP;
	short t = 0;
	this->bossWave = false;

	if (wave == 10) {
		t = 4;
		monstersHP = 0;
		this->bossWave = true;
	}
	else if (wave == 7) {
		t = 3;
		if (monstersHP >= 18 * wave_mod) monstersHP -= 18 * wave_mod;
	}
	else if (wave == 5) {
		t = 2;
		if (monstersHP >= 12 * wave_mod) monstersHP -= 12 * wave_mod;
	}
	else if (wave == 3) {
		t = 1;
		if (monstersHP >= 7 * wave_mod) monstersHP -= 7 * wave_mod;
	}
	else if (wave == 1) {
		t = 0;
		if (monstersHP >= 3 * wave_mod) monstersHP -= 3 * wave_mod;
	}

	this->monsterIDs.push_back(t);

	while (monstersHP > 0) {
		if (monstersHP >= 69 * wave_mod && wave % 10 == 0) {
			t = 4;
			this->bossWave = true;
		}
		else if (monstersHP >= 18 * wave_mod && wave >= 7)
			t = static_cast<short>(Random::Float() * 4.f);
		else if (monstersHP >= 12 * wave_mod && wave >= 5)
			t = static_cast<short>(Random::Float() * 3.f);
		else if (monstersHP >= 7 * wave_mod && wave >= 3)
			t = static_cast<short>(Random::Float() * 2.f);
		else if (monstersHP >= 3 * wave_mod)
			t = 0;
		else break;

		this->monsterIDs.push_back(t);

		switch (t) {
		case 0:
			monstersHP -= 3 * wave_mod;
			break;
		case 1:
			monstersHP -= 7 * wave_mod;
			break;
		case 2:
			monstersHP -= 12 * wave_mod;
			break;
		case 3:
			monstersHP -= 18 * wave_mod;
			break;
		case 4:
			monstersHP -= 69 * wave_mod;
			break;
		}
	}
}

void MonsterSystem::update(Player* player, PlayerGUI* playerGUI, ProjectileSystem* projectileSystem, DropSystem* dropSystem, FloatingTextSystem* floatingTextSystem, SoundEngine* soundEngine, bool& paused, const float& dt)
{
	for (const auto& monster : this->monsters) {
		if (monster->getSpawned()) {
			if (monster->isDead()) {
				monster->dying(dt);
			}
			else if (monster->getPunched()) {
				monster->smashed(dt);
			}
			else {
				monster->AI(this->tileMap, player, dt);
				if (monster->hasVelocity()) {
					monsterCollision(&(*monster));
					monster->obstacleCollision(this->tileMap);
					monster->move();
					monster->update(dt);
				}
				monster->loadAttack(dt);
				if (monster->attackPlayer(player, this->tileMap, projectileSystem, floatingTextSystem, soundEngine)) {
					playerGUI->update_HP();
				}
				monster->animation(dt);
			}
		}
		else monster->spawn(dt);
	}
	for (auto monster = this->monsters.begin(); monster != this->monsters.end();) {
		if ((*monster)->getDeadCountdown()) {
			if (player->addXP((*monster)->getXP())) {
				playerGUI->update_level(soundEngine);
				paused = true;
			}
			playerGUI->update_XP();
			dropSystem->addDrop("coin", (*monster)->getPosition().x + calcX(16, vm), (*monster)->getPosition().y + calcY(16, vm), (*monster)->getGold());
			if (const uint8_t t = uint8_t(Random::Float() * 4); (this->difficulty_mod == 0.75f && t < 2) || t == 0) {
				dropSystem->addDrop("heart", (*monster)->getPosition().x + calcX(16, vm), (*monster)->getPosition().y, 1);
			}
			monster = this->monsters.erase(monster);

			player->setKills(player->getKills() + 1);
			playerGUI->updateKills();
			if (this->monsters.size() > 0)
				playerGUI->updateMonsterCount(this->monsters.size());
		}
		else ++monster;
	}
}

void MonsterSystem::draw(sf::RenderTarget& target)
{
	for (const auto& monster : this->monsters) {
		monster->draw(target);
	}
}

void MonsterSystem::drawShadow(sf::RenderTarget& target)
{
	for (const auto& monster : this->monsters) {
		monster->drawShadow(target);
	}
}
