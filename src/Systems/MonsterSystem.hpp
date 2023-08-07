#pragma once

#include "DropSystem.hpp"
#include "Entities/Bosses/Minotaur.hpp"
#include "Entities/Monsters/Cyclope.hpp"
#include "Entities/Monsters/Goblin.hpp"
#include "Entities/Monsters/Orc.hpp"
#include "Entities/Monsters/Spider.hpp"
#include "GUI/PlayerGUI.hpp"
#include "ProjectileSystem.hpp"

class ProjectileSystem;

class MonsterSystem {
public:
    MonsterSystem(sf::VideoMode &vm, Player &player,
                  const std::vector<sf::FloatRect> &obstaclesBounds,
                  float gridSize, float difficulty_mod);
    ~MonsterSystem();

    const bool isMonsterIDsEmpty() const;
    const size_t monsterIDsSize() const;
    const bool isMonstersEmpty() const;
    const bool isBossWave() const;
    const std::vector<sf::Vector2f> monstersPositions() const;
    const float bossHP() const;

    void monsterIDsClear();

    void playerAttack(FloatingTextSystem &floatingTextSystem,
                      SoundEngine &soundEngine);
    void explosionAttack(const std::vector<sf::FloatRect> &particlesBounds,
                         FloatingTextSystem &floatingTextSystem);

    void projectileCollision(Projectile &proj,
                             FloatingTextSystem &floatingTextSystem);

    void monsterCollision(Monster &mob);
    void spawnMonsters(const std::vector<sf::FloatRect> &obstaclesBounds,
                       uint32_t wave);
    void prepareWave(uint32_t &wave, uint32_t &sumHP);

    void update(PlayerGUI &playerGUI, ProjectileSystem &projectileSystem,
                DropSystem &dropSystem, FloatingTextSystem &floatingTextSystem,
                SoundEngine &soundEngine,
                const std::vector<sf::FloatRect> &obstaclesBounds, bool &paused,
                float dt);
    void draw(sf::RenderTarget &target);
    void drawShadow(sf::RenderTarget &target);

private:
    sf::VideoMode &vm;
    Player &player;
    std::list<std::unique_ptr<Monster>> monsters;
    std::vector<short> monsterIDs;
    std::list<sf::FloatRect> freePositions;
    float gridSize;
    float difficulty_mod;
    bool bossWave;
};
