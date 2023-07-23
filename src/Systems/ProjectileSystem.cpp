#include "ProjectileSystem.hpp"

ProjectileSystem::ProjectileSystem(sf::VideoMode &vm) : vm(vm)
{
    this->textures.loadFromFile("assets/textures/projectiles.png");
}

ProjectileSystem::~ProjectileSystem()
{
    this->projectiles.clear();
}

void ProjectileSystem::addBomb(const sf::Vector2f &t_position,
                               float difficulty_mod, const sf::Vector2f &coords,
                               float coordsOffset, Player &player)
{
    this->projectiles.emplace_back(
        std::make_unique<Bomb>("bomb", this->vm, t_position, difficulty_mod,
                               coords, coordsOffset, player));
}

void ProjectileSystem::addShuriken(const sf::Vector2f &t_position,
                                   float difficulty_mod,
                                   const sf::Vector2f &coords,
                                   float coordsOffset, Player &player)
{
    this->projectiles.emplace_back(std::make_unique<Shuriken>(
        "shuriken", this->vm, t_position, difficulty_mod, coords, coordsOffset,
        player));
}

void ProjectileSystem::addProjectile(const std::string &name, float x, float y,
                                     float difficulty_mod,
                                     const sf::Vector2f &coords,
                                     float coordsOffset)
{
    if (name == "stone") {
        this->projectiles.emplace_back(std::make_unique<Stone>(
            name, this->vm, x, y, difficulty_mod, coords, coordsOffset));
    }
    else if (name == "groundWave") {
        this->projectiles.emplace_back(std::make_unique<GroundWave>(
            name, this->vm, x, y, difficulty_mod, coords, coordsOffset));
    }
}

void ProjectileSystem::playerAbility(const sf::Vector2f &coords, Player &player)
{
    if (player.getName() == "ninja") {
        addShuriken(player.getCenter(), 0, coords, 0, player);
    }
    else if (player.getName() == "master") {
        addShuriken(player.getCenter(), 0, coords, 0, player);
        addShuriken(player.getCenter(), 0, coords, -45.f, player);
        addShuriken(player.getCenter(), 0, coords, 45.f, player);
    }
    else if (player.getName() == "bomber") {
        addBomb(player.getCenter(), 0, coords, 0, player);
    }
}

void ProjectileSystem::bossSpecialAttack(Boss &boss)
{
    if (boss.isSpecialAttackReady() && boss.getName() == "minotaur") {
        addProjectile("groundWave", boss.getCenter().x, boss.getCenter().y,
                      boss.getDifficultyMod(), sf::Vector2f(boss.getUpCenter()),
                      0);
        addProjectile(
            "groundWave", boss.getCenter().x, boss.getCenter().y,
            boss.getDifficultyMod(),
            sf::Vector2f(boss.getRightCenter().x, boss.getUpCenter().y), 0);
        addProjectile("groundWave", boss.getCenter().x, boss.getCenter().y,
                      boss.getDifficultyMod(),
                      sf::Vector2f(boss.getRightCenter()), 0);
        addProjectile(
            "groundWave", boss.getCenter().x, boss.getCenter().y,
            boss.getDifficultyMod(),
            sf::Vector2f(boss.getRightCenter().x, boss.getDownCenter().y), 0);
        addProjectile("groundWave", boss.getCenter().x, boss.getCenter().y,
                      boss.getDifficultyMod(),
                      sf::Vector2f(boss.getDownCenter()), 0);
        addProjectile(
            "groundWave", boss.getCenter().x, boss.getCenter().y,
            boss.getDifficultyMod(),
            sf::Vector2f(boss.getLeftCenter().x, boss.getDownCenter().y), 0);
        addProjectile("groundWave", boss.getCenter().x, boss.getCenter().y,
                      boss.getDifficultyMod(),
                      sf::Vector2f(boss.getLeftCenter()), 0);
        addProjectile(
            "groundWave", boss.getCenter().x, boss.getCenter().y,
            boss.getDifficultyMod(),
            sf::Vector2f(boss.getLeftCenter().x, boss.getUpCenter().y), 0);
        boss.resetSpecialAttack();
    }
}

void ProjectileSystem::update(Player &player, PlayerGUI &playerGui,
                              ParticleSystem &particleSystem,
                              MonsterSystem &monsterSystem,
                              const sf::FloatRect &mapBounds,
                              const std::vector<sf::FloatRect> &obstaclesBounds,
                              FloatingTextSystem &floatingTextSystem,
                              SoundEngine &soundEngine, float dt)
{
    for (auto proj = this->projectiles.begin();
         proj != this->projectiles.end();) {
        (*proj)->update(dt);
        (*proj)->wallCollision(obstaclesBounds);
        (*proj)->playerCollision(player);
        monsterSystem.projectileCollision(**proj, player, floatingTextSystem);
        (*proj)->move();

        if ((*proj)->isBomb() &&
            ((*proj)->hasCollidedWall() || (*proj)->hasCollidedMonster())) {
            particleSystem.addParticle(
                (*proj)->getName(), (*proj)->getPosition().x,
                (*proj)->getPosition().y, (*proj)->getAttack(),
                player.getProjectileArea());
            soundEngine.addSound("explosion");
            proj = this->projectiles.erase(proj);
        }
        else if ((*proj)->hasExploded()) {
            particleSystem.addParticle(
                (*proj)->getName(), (*proj)->getPosition().x,
                (*proj)->getPosition().y, (*proj)->getAttack(),
                player.getProjectileArea());
            soundEngine.addSound("explosion");
            proj = this->projectiles.erase(proj);
        }
        else if ((*proj)->getHP() == 0) {
            proj = this->projectiles.erase(proj);
        }
        else if ((*proj)->hasCollidedPlayer()) {
            if (!(player.isAbilityActive() &&
                  (player.getArmor() + player.getIncreasedArmor()) > 20)) {
                int attack = static_cast<int>(
                    round((*proj)->getAttack() -
                          ((*proj)->getAttack() * player.getArmor() * 0.05f)));

                if (attack > 0) {
                    if (static_cast<int>(player.getHP() - attack) < 0) {
                        player.setHP(0);
                    }
                    else {
                        player.setHP(player.getHP() - attack);
                    }

                    player.punch();

                    floatingTextSystem.addFloatingText(
                        "flamingo", "-" + std::to_string(attack),
                        calcChar(16, vm),
                        player.getPosition().x + calcX(32, vm),
                        player.getPosition().y + calcY(32, vm), false);
                    playerGui.update_HP();
                }
            }

            proj = this->projectiles.erase(proj);
        }
        else if (!mapBounds.contains((*proj)->getPosition())) {
            proj = this->projectiles.erase(proj);
        }
        else {
            ++proj;
        }
    }
}

void ProjectileSystem::draw(sf::RenderTarget &target)
{
    for (const auto &proj : this->projectiles) {
        proj->draw(target);
    }
}
