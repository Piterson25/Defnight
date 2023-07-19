#include "ProjectileSystem.hpp"

ProjectileSystem::ProjectileSystem(sf::VideoMode &vm) : vm(vm)
{
    this->textures.loadFromFile("assets/textures/projectiles.png");
}

ProjectileSystem::~ProjectileSystem()
{
    this->projectiles.clear();
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
    else if (name == "shuriken") {
        this->projectiles.emplace_back(std::make_unique<Shuriken>(
            name, this->vm, x, y, difficulty_mod, coords, coordsOffset));
    }
    else if (name == "bomb") {
        this->projectiles.emplace_back(std::make_unique<Bomb>(
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
        addProjectile("shuriken", player.getPosition().x + calcX(32, vm),
                      player.getPosition().y + calcY(32, vm), 0, coords, 0);
        this->projectiles.back()->setAttack(player.getProjectileAttack());
    }
    else if (player.getName() == "master") {
        addProjectile("shuriken", player.getPosition().x + calcX(32, vm),
                      player.getPosition().y + calcY(32, vm), 0, coords, 0);
        this->projectiles.back()->setAttack(player.getProjectileAttack());
        addProjectile("shuriken", player.getPosition().x + calcX(32, vm),
                      player.getPosition().y + calcY(32, vm), 0, coords, -45.f);
        this->projectiles.back()->setAttack(player.getProjectileAttack());
        addProjectile("shuriken", player.getPosition().x + calcX(32, vm),
                      player.getPosition().y + calcY(32, vm), 0, coords, 45.f);
        this->projectiles.back()->setAttack(player.getProjectileAttack());
    }
    else if (player.getName() == "bomber") {
        addProjectile("bomb", player.getPosition().x + calcX(32, vm),
                      player.getPosition().y + calcY(32, vm), 0, coords, 0);
        this->projectiles.back()->setAttack(player.getProjectileAttack());
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
                (*proj)->getPosition().y, (*proj)->getAttack());
            soundEngine.addSound("explosion");
            proj = this->projectiles.erase(proj);
        }
        else if ((*proj)->hasExploded()) {
            particleSystem.addParticle(
                (*proj)->getName(), (*proj)->getPosition().x,
                (*proj)->getPosition().y, (*proj)->getAttack());
            soundEngine.addSound("explosion");
            proj = this->projectiles.erase(proj);
        }
        else if ((*proj)->getHP() == 0) {
            proj = this->projectiles.erase(proj);
        }
        else if ((*proj)->hasCollidedPlayer()) {
            if (!(player.isAbilityActive() && (player.isIncreasedArmor()))) {
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
                        "-" + std::to_string(attack), calcChar(16, vm),
                        player.getPosition().x + calcX(32, vm),
                        player.getPosition().y + calcY(32, vm),
                        sf::Color(228, 92, 95), false);
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
