#include "MonsterSystem.hpp"

MonsterSystem::MonsterSystem(sf::VideoMode &vm, Player &player,
                             const std::vector<sf::FloatRect> &obstaclesBounds,
                             float gridSize, float difficulty_mod)
    : vm(vm), player(player), gridSize(gridSize), difficulty_mod(difficulty_mod)
{
    this->bossWave = false;
    freePositions.assign(obstaclesBounds.begin(), obstaclesBounds.end());
}

MonsterSystem::~MonsterSystem()
{
    this->monsters.clear();
}

const bool MonsterSystem::isMonsterIDsEmpty() const
{
    return this->monsterIDs.empty();
}

const size_t MonsterSystem::monsterIDsSize() const
{
    return this->monsterIDs.size();
}

const bool MonsterSystem::isMonstersEmpty() const
{
    return this->monsters.empty();
}

const bool MonsterSystem::isBossWave() const
{
    return this->bossWave;
}

const std::vector<sf::Vector2f> MonsterSystem::monstersPositions() const
{
    std::vector<sf::Vector2f> positions;
    for (const auto &monster : monsters) {
        positions.push_back(monster->getPosition());
    }
    return positions;
}

const float MonsterSystem::bossHP() const
{
    for (const auto &monster : monsters) {
        auto boss = dynamic_cast<Boss *>(monster.get());
        if (boss) {
            return static_cast<float>(static_cast<float>(monster->getHP()) /
                                      static_cast<float>(monster->getMaxHP()));
        }
    }
    return 0.f;
}

void MonsterSystem::monsterIDsClear()
{
    this->monsterIDs.clear();
}

void MonsterSystem::playerAttack(FloatingTextSystem &floatingTextSystem,
                                 SoundEngine &soundEngine)
{
    uint32_t limit = player.getAttackLimit();
    for (const auto &monster : monsters) {
        const float distance = player.attackDistance(*monster, player);

        if (limit > 0 && distance <= player.getReach() * calcX(48, vm)) {

            if (!monster->isDead() && !monster->isPunched() &&
                monster->hasSpawned() && player.isAttacking() &&
                player.getFrame() == 80) {
                if ((static_cast<uint32_t>(Random::Float() * 100.f) + 1) <=
                    player.getCriticalChance()) {
                    const int attack = 2 * player.getAttack();
                    floatingTextSystem.addFloatingText(
                        gui::ORANGE, std::to_string(-attack), calcChar(16, vm),
                        monster->getPosition().x + calcX(32, vm),
                        monster->getPosition().y + calcY(32, vm), false);
                    if (static_cast<int>(monster->getHP() - attack) < 0) {
                        monster->setHP(0);
                    }
                    else {
                        monster->setHP(monster->getHP() - attack);
                    }
                }
                else {
                    const int attack = player.getAttack();
                    floatingTextSystem.addFloatingText(
                        gui::WHITE, std::to_string(-attack), calcChar(16, vm),
                        monster->getPosition().x + calcX(32, vm),
                        monster->getPosition().y + calcY(32, vm), false);
                    if (static_cast<int>(monster->getHP() - attack) < 0) {
                        monster->setHP(0);
                    }
                    else {
                        monster->setHP(monster->getHP() - attack);
                    }
                }

                if (!player.isSoundPlayed()) {
                    soundEngine.addSound("whoosh_hit");
                    player.setPlayedSound(true);
                }

                monster->punch();
                --limit;
                if (limit == 0) {
                    break;
                }
            }
        }
    }
}

void MonsterSystem::explosionAttack(
    const std::vector<sf::FloatRect> &particlesBounds,
    FloatingTextSystem &floatingTextSystem)
{
    for (const auto &bounds : particlesBounds) {
        for (const auto &monster : monsters) {
            if (monster->getGlobalBounds().intersects(bounds)) {
                if (!monster->isDead() && !monster->isPunched() &&
                    monster->hasSpawned()) {
                    const int attack = player.getProjectileAttack();
                    floatingTextSystem.addFloatingText(
                        gui::WHITE, std::to_string(-attack), calcChar(16, vm),
                        monster->getPosition().x + calcX(32, vm),
                        monster->getPosition().y + calcY(32, vm), false);
                    if (static_cast<int>(monster->getHP() - attack) < 0) {
                        monster->setHP(0);
                    }
                    else {
                        monster->setHP(monster->getHP() - attack);
                    }

                    monster->punch();
                }
            }
        }
    }
}

void MonsterSystem::projectileCollision(Projectile &proj,
                                        FloatingTextSystem &floatingTextSystem)
{
    for (const auto &monster : monsters) {
        proj.monsterCollision(*monster, player, floatingTextSystem);
        if (proj.getPiercing() == 0) {
            break;
        }
    }
}

void MonsterSystem::monsterCollision(Monster &mob)
{
    for (const auto &monster : monsters) {
        if (vectorDistance(mob.getPosition(), monster->getPosition()) <
            2 * monster->getGlobalBounds().width) {
            sf::FloatRect mobBounds = mob.getGlobalBounds();
            sf::FloatRect monsterBounds = monster->getGlobalBounds();

            sf::FloatRect nextPos = mobBounds;
            nextPos.left += mob.getVelocity().x;
            nextPos.top += mob.getVelocity().y;

            if (monsterBounds.intersects(nextPos)) {
                if (mob.hasCollidedBottom(mobBounds, monsterBounds)) {
                    mob.setVeloctiy(sf::Vector2f(mob.getVelocity().x, 0.f));
                    mob.setPosition(mobBounds.left,
                                    monsterBounds.top - mobBounds.height);
                }
                else if (mob.hasCollidedTop(mobBounds, monsterBounds)) {
                    mob.setVeloctiy(sf::Vector2f(mob.getVelocity().x, 0.f));
                    mob.setPosition(mobBounds.left,
                                    monsterBounds.top + monsterBounds.height);
                }

                if (mob.hasCollidedRight(mobBounds, monsterBounds)) {
                    mob.setVeloctiy(sf::Vector2f(0.f, mob.getVelocity().y));
                    mob.setPosition(monsterBounds.left - mobBounds.width,
                                    mobBounds.top);
                }
                else if (mob.hasCollidedLeft(mobBounds, monsterBounds)) {
                    mob.setVeloctiy(sf::Vector2f(0.f, mob.getVelocity().y));
                    mob.setPosition(monsterBounds.left + monsterBounds.width,
                                    mobBounds.top);
                }
            }
        }
    }
}

void MonsterSystem::spawnMonsters(
    const std::vector<sf::FloatRect> &obstaclesBounds, uint32_t wave)
{
    float wave_mod = 1.f + static_cast<uint32_t>(wave / 10.f) * 2.f;
    const float minSpawnDistance = calcX(3.f * this->gridSize, this->vm);

    freePositions.assign(obstaclesBounds.begin(), obstaclesBounds.end());

    for (const auto &id : this->monsterIDs) {
        if (freePositions.empty()) {
            break;
        }

        uint32_t rx = 0, ry = 0;
        sf::FloatRect monsterBounds;
        auto positionIt = freePositions.begin();

        bool canSpawnMonster = false;

        while (!canSpawnMonster && positionIt != freePositions.end()) {
            rx = static_cast<uint32_t>(Random::Float() * 30.f) + 1;
            ry = static_cast<uint32_t>(Random::Float() * 30.f) + 1;

            const float monsterWidth =
                (id == 4) ? calcX(128, vm) : calcX(64, vm);
            const float monsterHeight =
                (id == 4) ? calcX(128, vm) : calcX(64, vm);
            monsterBounds = sf::FloatRect(calcX(this->gridSize * rx, this->vm),
                                          calcY(this->gridSize * ry, this->vm),
                                          monsterWidth, monsterHeight);

            canSpawnMonster = true;

            if (monsterBounds.intersects(player.getGlobalBounds()) ||
                vectorDistance(monsterBounds.left, monsterBounds.top,
                               player.getPosition().x,
                               player.getPosition().y) <= minSpawnDistance) {
                canSpawnMonster = false;
                continue;
            }

            for (const auto &obstacle : obstaclesBounds) {
                if (monsterBounds.intersects(obstacle)) {
                    canSpawnMonster = false;
                    break;
                }
            }

            if (canSpawnMonster) {
                for (const auto &mob : this->monsters) {
                    sf::FloatRect mobBounds = mob->getGlobalBounds();
                    if (monsterBounds.intersects(mobBounds)) {
                        canSpawnMonster = false;
                        break;
                    }
                }
            }

            if (id == 4 && canSpawnMonster) {
                sf::FloatRect monsterBounds2(
                    monsterBounds.left + this->gridSize, monsterBounds.top,
                    monsterWidth, monsterHeight);
                sf::FloatRect monsterBounds3(monsterBounds.left,
                                             monsterBounds.top + this->gridSize,
                                             monsterWidth, monsterHeight);
                sf::FloatRect monsterBounds4(monsterBounds.left +
                                                 this->gridSize,
                                             monsterBounds.top + this->gridSize,
                                             monsterWidth, monsterHeight);

                for (const auto &obstacle : obstaclesBounds) {
                    if (monsterBounds2.intersects(obstacle) ||
                        monsterBounds3.intersects(obstacle) ||
                        monsterBounds4.intersects(obstacle)) {
                        canSpawnMonster = false;
                        break;
                    }
                }

                if (canSpawnMonster) {
                    for (const auto &mob : this->monsters) {
                        sf::FloatRect mobBounds = mob->getGlobalBounds();
                        if (monsterBounds2.intersects(mobBounds) ||
                            monsterBounds3.intersects(mobBounds) ||
                            monsterBounds4.intersects(mobBounds)) {
                            canSpawnMonster = false;
                            break;
                        }
                    }
                }
            }

            if (!canSpawnMonster) {
                ++positionIt;
            }
        }

        if (positionIt == freePositions.end()) {
            break;
        }

        switch (id) {
            case 0:
                this->monsters.emplace_back(std::make_unique<Goblin>(
                    "GOBLIN", this->vm, calcX(this->gridSize * rx, this->vm),
                    calcY(this->gridSize * ry, this->vm), this->difficulty_mod,
                    wave_mod, obstaclesBounds));
                break;
            case 1:
                this->monsters.emplace_back(std::make_unique<Spider>(
                    "SPIDER", this->vm, calcX(this->gridSize * rx, this->vm),
                    calcY(this->gridSize * ry, this->vm), this->difficulty_mod,
                    wave_mod, obstaclesBounds));
                break;
            case 2:
                this->monsters.emplace_back(std::make_unique<Orc>(
                    "ORC", this->vm, calcX(this->gridSize * rx, this->vm),
                    calcY(this->gridSize * ry, this->vm), this->difficulty_mod,
                    wave_mod, obstaclesBounds));
                break;
            case 3:
                this->monsters.emplace_back(std::make_unique<Cyclope>(
                    "CYCLOPE", this->vm, calcX(this->gridSize * rx, this->vm),
                    calcY(this->gridSize * ry, this->vm), this->difficulty_mod,
                    wave_mod, obstaclesBounds));
                break;
            case 4:
                this->monsters.emplace_back(std::make_unique<Minotaur>(
                    "MINOTAUR", this->vm, calcX(this->gridSize * rx, this->vm),
                    calcY(this->gridSize * ry, this->vm), this->difficulty_mod,
                    wave_mod, obstaclesBounds));
                break;
            default:
                break;
        }

        if (id == 4) {
            positionIt = freePositions.erase(positionIt);
            for (int i = 1; i < 4; ++i) {
                ++positionIt;
                positionIt = freePositions.erase(positionIt);
            }
        }
        else {
            freePositions.erase(positionIt);
        }
    }
}

void MonsterSystem::prepareWave(uint32_t &wave, uint32_t &sumHP)
{
    wave++;
    this->player.setWave(wave);
    const uint32_t wave_mod = 1 + static_cast<uint32_t>(wave / 10.f);
    if (wave % 10 != 0) {
        sumHP += static_cast<uint32_t>((2 - ((1 + sqrtf(5)) / 2.f)) * sumHP);
    }
    uint32_t monstersHP = sumHP;
    short t = 0;
    this->bossWave = false;

    if (wave % 10 == 0) {
        t = 4;
        monstersHP = 0;
        this->bossWave = true;
    }
    else if (wave == 7) {
        t = 3;
        if (monstersHP >= 30 * wave_mod) {
            monstersHP -= 30 * wave_mod;
        }
    }
    else if (wave == 5) {
        t = 2;
        if (monstersHP >= 12 * wave_mod) {
            monstersHP -= 12 * wave_mod;
        }
    }
    else if (wave == 3) {
        t = 1;
        if (monstersHP >= 7 * wave_mod) {
            monstersHP -= 7 * wave_mod;
        }
    }
    else if (wave == 1) {
        t = 0;
        if (monstersHP >= 3 * wave_mod) {
            monstersHP -= 3 * wave_mod;
        }
    }

    this->monsterIDs.push_back(t);

    while (monstersHP > 0) {
        if (monstersHP >= 69 * wave_mod && wave % 10 == 0) {
            t = 4;
            this->bossWave = true;
        }
        else if (monstersHP >= 30 * wave_mod && wave >= 7) {
            t = static_cast<short>(Random::Float() * 4.f);
        }
        else if (monstersHP >= 12 * wave_mod && wave >= 5) {
            t = static_cast<short>(Random::Float() * 3.f);
        }
        else if (monstersHP >= 7 * wave_mod && wave >= 3) {
            t = static_cast<short>(Random::Float() * 2.f);
        }
        else if (monstersHP >= 3 * wave_mod) {
            t = 0;
        }
        else {
            break;
        }

        if (this->monsterIDs.size() < 100) {
            this->monsterIDs.push_back(t);
        }
        else {
            break;
        }

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
                monstersHP -= 30 * wave_mod;
                break;
            case 4:
                monstersHP -= 69 * wave_mod;
                break;
        }
    }
}

void MonsterSystem::update(PlayerGUI &playerGUI,
                           ProjectileSystem &projectileSystem,
                           DropSystem &dropSystem,
                           FloatingTextSystem &floatingTextSystem,
                           SoundEngine &soundEngine,
                           const std::vector<sf::FloatRect> &obstaclesBounds,
                           bool &paused, float dt)
{
    float slowedDt = dt;

    if (player.getTimeSlowdown() > 0.f && player.isAbilityActive()) {
        slowedDt -= dt * player.getTimeSlowdown();
    }
    for (const auto &monster : this->monsters) {
        if (monster->hasSpawned()) {
            if (monster->isDead()) {
                monster->dyingAnimation(dt);
            }
            else if (monster->isPunched()) {
                monster->smashed(dt);
            }
            else {
                monster->calculateAI(obstaclesBounds, player,
                                     this->monstersPositions(), slowedDt);
                if (monster->hasVelocity()) {
                    monsterCollision(*monster);
                    monster->obstacleCollision(obstaclesBounds);
                    monster->move();
                }
                monster->update(slowedDt);
                monster->loadAttack(slowedDt);
                auto boss = dynamic_cast<Boss *>(monster.get());
                if (boss) {
                    boss->specialAttack(soundEngine, slowedDt);
                    projectileSystem.bossSpecialAttack(*boss);
                }
                if (monster->hasAttackedPlayer(obstaclesBounds, player,
                                               soundEngine,
                                               floatingTextSystem)) {
                    auto cyclope = dynamic_cast<Cyclope *>(monster.get());
                    if (cyclope) {
                        projectileSystem.addProjectile(
                            "STONE", monster->getPosition().x + calcX(24, vm),
                            monster->getPosition().y + calcY(36, vm),
                            monster->getDifficultyMod(), player.getCenter(), 0);
                    }
                    else {
                        floatingTextSystem.addFloatingText(
                            gui::FLAMINGO,
                            std::to_string(static_cast<int>(
                                -round(monster->getAttack() -
                                       (monster->getAttack() *
                                        player.getArmor() * 0.05f)))),
                            calcChar(16, vm),
                            player.getPosition().x + calcX(48, vm),
                            player.getPosition().y + calcY(32, vm), false);
                    }

                    playerGUI.updateHP();
                }
                monster->animation(slowedDt);
            }
        }
        else {
            monster->spawn(dt);
        }
    }
    for (auto monster = this->monsters.begin();
         monster != this->monsters.end();) {
        if ((*monster)->hasDeadCountdownExpired()) {
            player.setPendingXP(player.getPendingXP() + (*monster)->getXP());
            player.setLeveling(true);
            dropSystem.addDrop("COIN",
                               (*monster)->getPosition().x + calcX(16, vm),
                               (*monster)->getPosition().y + calcY(16, vm),
                               (*monster)->getGold() + player.getGoldReward());
            if (const uint8_t t = uint8_t(Random::Float() * 4);
                (this->difficulty_mod == 0.75f && t < 2) || t == 0) {
                dropSystem.addDrop("HEART",
                                   (*monster)->getPosition().x + calcX(16, vm),
                                   (*monster)->getPosition().y, 1);
            }
            monster = this->monsters.erase(monster);

            player.setKills(player.getKills() + 1);
            playerGUI.updateKills();
            if (this->monsters.size() > 0) {
                playerGUI.updateMonsterCount(this->monsters.size());
            }
        }
        else {
            ++monster;
        }
    }
}

void MonsterSystem::draw(sf::RenderTarget &target)
{
    for (const auto &monster : this->monsters) {
        monster->draw(target);
    }
}

void MonsterSystem::drawShadow(sf::RenderTarget &target)
{
    for (const auto &monster : this->monsters) {
        monster->drawShadow(target);
    }
}
