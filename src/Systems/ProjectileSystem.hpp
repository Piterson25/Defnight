#pragma once

#include "Entities/Bosses/Boss.hpp"
#include "Entities/Projectiles/Bomb.hpp"
#include "Entities/Projectiles/GroundWave.hpp"
#include "Entities/Projectiles/Projectile.hpp"
#include "Entities/Projectiles/Shuriken.hpp"
#include "Entities/Projectiles/Stone.hpp"
#include "GUI/PlayerGUI.hpp"
#include "MonsterSystem.hpp"
#include "ParticleSystem.hpp"

class MonsterSystem;
class PlayerGUI;

class ProjectileSystem {
public:
    ProjectileSystem(sf::VideoMode &vm);
    ~ProjectileSystem();

    void addProjectile(const std::string &name, float x, float y,
                       float difficulty_mod, const sf::Vector2f &coords,
                       float coordsOffset);
    void playerAbility(const sf::Vector2f &coords, Player &player);
    void bossSpecialAttack(Boss &boss);
    void update(Player &player, PlayerGUI &playerGui,
                ParticleSystem &particleSystem, MonsterSystem &monsterSystem,
                const sf::FloatRect &mapBounds,
                const std::vector<sf::FloatRect> &obstaclesBounds,
                FloatingTextSystem &floatingTextSystem,
                SoundEngine &soundEngine, float dt);
    void draw(sf::RenderTarget &target);

private:
    std::list<std::unique_ptr<Projectile>> projectiles;
    sf::VideoMode &vm;
    sf::Texture textures;
};
