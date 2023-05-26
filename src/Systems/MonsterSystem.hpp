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
    MonsterSystem(sf::VideoMode &vm,
                  const std::vector<sf::FloatRect> &obstaclesBounds,
                  float gridSize, float difficulty_mod);
    ~MonsterSystem();

    const bool monsterIDsEmpty() const;
    const size_t monsterIDsSize() const;
    const bool monstersEmpty() const;
    const bool isBossWave() const;
    const std::vector<sf::Vector2f> monstersPositions() const;
    const float bossHP() const;

    void monsterIDsClear();

    void playerAttack(Player &player, FloatingTextSystem &floatingTextSystem,
                      SoundEngine &soundEngine);
    void explosionAttack(const std::vector<sf::FloatRect> &particlesBounds,
                         FloatingTextSystem &floatingTextSystem);

    void projectileCollision(Projectile &proj, Player &player,
                             FloatingTextSystem &floatingTextSystem);

    void monsterCollision(Monster &mob);
    void spawnMonsters(Player &player,
                       const std::vector<sf::FloatRect> &obstaclesBounds,
                       uint32_t wave);
    void prepareWave(uint32_t &wave, uint32_t &sumHP);

    void update(Player &player, PlayerGUI &playerGUI,
                ProjectileSystem &projectileSystem, DropSystem &dropSystem,
                FloatingTextSystem &floatingTextSystem,
                SoundEngine &soundEngine,
                const std::vector<sf::FloatRect> &obstaclesBounds, bool &paused,
                float dt);
    void draw(sf::RenderTarget &target);
    void drawShadow(sf::RenderTarget &target);

private:
    sf::VideoMode &vm;
    std::list<std::unique_ptr<Monster>> monsters;
    std::unordered_map<std::string, sf::Texture> textures;
    sf::Texture shadow_texture;
    std::vector<short> monsterIDs;
    float gridSize;
    float difficulty_mod;
    bool bossWave;
};
