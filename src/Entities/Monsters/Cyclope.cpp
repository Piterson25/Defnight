#include "Cyclope.hpp"

Cyclope::Cyclope(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
                 float t_y, float difficulty_mod, float wave_mod,
                 const std::vector<sf::FloatRect> &obstaclesBounds)
    : Monster(t_name, t_vm, t_x, t_y, difficulty_mod, wave_mod, obstaclesBounds)
{
    this->name = "cyclope";
    this->attack = static_cast<uint32_t>(4 * difficulty_mod);
    this->attackSpeed = 1;
    this->HP = static_cast<uint32_t>(18 * difficulty_mod);
    this->maxHP = static_cast<uint32_t>(18 * difficulty_mod);
    this->speed = 1;
    this->gold = static_cast<uint32_t>(4 * wave_mod);
    this->XP = static_cast<uint32_t>(26 * wave_mod);
}

Cyclope::~Cyclope() = default;

const bool
Cyclope::attackPlayer(const std::vector<sf::FloatRect> &obstaclesBounds,
                      Player &player, SoundEngine &soundEngine, FloatingTextSystem &floatingTextSystem)
{
    if (!checkAttack(obstaclesBounds, player)) {
        return false;
    }

    if (this->getAttack() == 0) {
        return false;
    }

    this->isAttacking = false;

    if (!this->playedSound) {
        soundEngine.addSound("punch");
        this->playedSound = true;
    }

    return true;
}

const bool
Cyclope::checkAttack(const std::vector<sf::FloatRect> &obstaclesBounds,
                     Player &player)
{
    const float distance = this->attackDistance(player, *this);

    if (distance <= static_cast<float>(this->reach) * 8.f * calcX(64, vm) &&
        !sightCollision(obstaclesBounds,
                        sf::Vector2f(this->getPosition().x + calcX(24, vm),
                                     this->getPosition().y + calcY(36, vm)),
                        player.getCenter())) {
        this->doAttack();
        if (!player.isDead() && !player.getPunched() && this->isAttacking &&
            this->frame == 80) {
            return true;
        }
    }

    return false;
}
