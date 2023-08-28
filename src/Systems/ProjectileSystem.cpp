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
        std::make_unique<Bomb>("BOMB", this->vm, t_position, difficulty_mod,
                               coords, coordsOffset, player));
}

void ProjectileSystem::addShuriken(const sf::Vector2f &t_position,
                                   float difficulty_mod,
                                   const sf::Vector2f &coords,
                                   float coordsOffset, Player &player)
{
    this->projectiles.emplace_back(std::make_unique<Shuriken>(
        "SHURIKEN", this->vm, t_position, difficulty_mod, coords, coordsOffset,
        player));
}

void ProjectileSystem::addProjectile(const std::string &name, float x, float y,
                                     float difficulty_mod,
                                     const sf::Vector2f &coords,
                                     float coordsOffset)
{
    if (name == "STONE") {
        this->projectiles.emplace_back(std::make_unique<Stone>(
            name, this->vm, x, y, difficulty_mod, coords, coordsOffset));
    }
    else if (name == "GROUNDWAVE") {
        this->projectiles.emplace_back(std::make_unique<GroundWave>(
            name, this->vm, x, y, difficulty_mod, coords, coordsOffset));
    }
}

void ProjectileSystem::playerAbility(const sf::Vector2f &coords, Player &player)
{
    if (player.getName() == "NINJA") {
        addShuriken(player.getCenter(), 0, coords, 0, player);
    }
    else if (player.getName() == "SENSEI") {
        addShuriken(player.getCenter(), 0, coords, 0, player);
        addShuriken(player.getCenter(), 0, coords, -45.f, player);
        addShuriken(player.getCenter(), 0, coords, 45.f, player);
    }
    else if (player.getName() == "BOMBER") {
        addBomb(player.getCenter(), 0, coords, 0, player);
    }
}

void ProjectileSystem::bossSpecialAttack(Boss &boss)
{
    if (boss.getName() == "MINOTAUR") {
        addProjectile("GROUNDWAVE", boss.getCenter().x, boss.getCenter().y,
                      boss.getDifficultyMod(), sf::Vector2f(boss.getUpCenter()),
                      0);
        addProjectile(
            "GROUNDWAVE", boss.getCenter().x, boss.getCenter().y,
            boss.getDifficultyMod(),
            sf::Vector2f(boss.getRightCenter().x +
                             (boss.getRightCenter().x - boss.getUpCenter().x) /
                                 2,
                         boss.getUpCenter().y),
            0);
        addProjectile(
            "GROUNDWAVE", boss.getCenter().x, boss.getCenter().y,
            boss.getDifficultyMod(),
            sf::Vector2f(boss.getRightCenter().x, boss.getUpCenter().y), 0);
        addProjectile(
            "GROUNDWAVE", boss.getCenter().x, boss.getCenter().y,
            boss.getDifficultyMod(),
            sf::Vector2f(boss.getRightCenter().x,
                         boss.getRightCenter().y +
                             (boss.getRightCenter().y - boss.getUpCenter().y) /
                                 2),
            0);
        addProjectile("GROUNDWAVE", boss.getCenter().x, boss.getCenter().y,
                      boss.getDifficultyMod(),
                      sf::Vector2f(boss.getRightCenter()), 0);
        addProjectile(
            "GROUNDWAVE", boss.getCenter().x, boss.getCenter().y,
            boss.getDifficultyMod(),
            sf::Vector2f(
                boss.getRightCenter().x,
                boss.getDownCenter().y +
                    (boss.getDownCenter().y - boss.getRightCenter().y) / 2),
            0);
        addProjectile(
            "GROUNDWAVE", boss.getCenter().x, boss.getCenter().y,
            boss.getDifficultyMod(),
            sf::Vector2f(boss.getRightCenter().x, boss.getDownCenter().y), 0);
        addProjectile(
            "GROUNDWAVE", boss.getCenter().x, boss.getCenter().y,
            boss.getDifficultyMod(),
            sf::Vector2f(
                boss.getRightCenter().x +
                    (boss.getRightCenter().x - boss.getDownCenter().x) / 2,
                boss.getDownCenter().y),
            0);
        addProjectile("GROUNDWAVE", boss.getCenter().x, boss.getCenter().y,
                      boss.getDifficultyMod(),
                      sf::Vector2f(boss.getDownCenter()), 0);
        addProjectile(
            "GROUNDWAVE", boss.getCenter().x, boss.getCenter().y,
            boss.getDifficultyMod(),
            sf::Vector2f(boss.getLeftCenter().x +
                             (boss.getLeftCenter().x - boss.getDownCenter().x) /
                                 2,
                         boss.getDownCenter().y),
            0);
        addProjectile(
            "GROUNDWAVE", boss.getCenter().x, boss.getCenter().y,
            boss.getDifficultyMod(),
            sf::Vector2f(boss.getLeftCenter().x, boss.getDownCenter().y), 0);
        addProjectile(
            "GROUNDWAVE", boss.getCenter().x, boss.getCenter().y,
            boss.getDifficultyMod(),
            sf::Vector2f(boss.getLeftCenter().x,
                         boss.getDownCenter().y +
                             (boss.getDownCenter().y - boss.getLeftCenter().y) /
                                 2),
            0);
        addProjectile("GROUNDWAVE", boss.getCenter().x, boss.getCenter().y,
                      boss.getDifficultyMod(),
                      sf::Vector2f(boss.getLeftCenter()), 0);
        addProjectile(
            "GROUNDWAVE", boss.getCenter().x, boss.getCenter().y,
            boss.getDifficultyMod(),
            sf::Vector2f(boss.getLeftCenter().x,
                         boss.getUpCenter().y +
                             (boss.getUpCenter().y - boss.getLeftCenter().y) /
                                 2),
            0);
        addProjectile(
            "GROUNDWAVE", boss.getCenter().x, boss.getCenter().y,
            boss.getDifficultyMod(),
            sf::Vector2f(boss.getLeftCenter().x, boss.getUpCenter().y), 0);
        addProjectile(
            "GROUNDWAVE", boss.getCenter().x, boss.getCenter().y,
            boss.getDifficultyMod(),
            sf::Vector2f(boss.getUpCenter().x +
                             (boss.getUpCenter().x - boss.getLeftCenter().x) /
                                 2,
                         boss.getUpCenter().y),
            0);
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
    float slowedDt = dt;

    if (player.getTimeSlowdown() > 0.f && player.isAbilityActive()) {
        slowedDt -= dt * player.getTimeSlowdown();
    }

    for (auto proj = this->projectiles.begin();
         proj != this->projectiles.end();) {
        const Stone *const stone = dynamic_cast<const Stone *>((*proj).get());
        const GroundWave *const groundWave =
            dynamic_cast<const GroundWave *>((*proj).get());
        if (stone || groundWave) {
            (*proj)->update(slowedDt);
            if (groundWave && (*proj)->isParticleCooldown(slowedDt)) {
                particleSystem.addSmallParticle(
                    (*proj)->getCenter(),
                    sf::Vector2f(calcX(8, vm), calcY(8, vm)), gui::BROWN);
            }
        }
        else {
            (*proj)->update(dt);
        }

        if (!groundWave) {
            (*proj)->wallCollision(obstaclesBounds);
        }
        (*proj)->playerCollision(player);
        monsterSystem.projectileCollision(**proj);
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
        else if ((*proj)->getHP() == 0 || (*proj)->getPosition().x < 0 ||
                 (*proj)->getPosition().y < 0 ||
                 (*proj)->getPosition().x > calcX(32 * 64, vm) ||
                 (*proj)->getPosition().y > calcY(32 * 64, vm)) {
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

                    player.setDamageTaken(player.getDamageTaken() + attack);

                    player.punch();

                    floatingTextSystem.addFloatingText(
                        gui::FLAMINGO, "-" + std::to_string(attack),
                        calcChar(16, vm),
                        player.getPosition().x + calcX(32, vm),
                        player.getPosition().y + calcY(32, vm), false);
                    playerGui.updateHP();
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
