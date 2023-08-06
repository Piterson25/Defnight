#include "Cyclope.hpp"

Cyclope::Cyclope(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
                 float t_y, float difficulty_mod, float wave_mod,
                 const std::vector<sf::FloatRect> &obstaclesBounds)
    : Monster(t_name, t_vm, t_x, t_y, difficulty_mod, wave_mod, obstaclesBounds)
{
    this->name = "CYCLOPE";
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
Cyclope::hasAttackedPlayer(const std::vector<sf::FloatRect> &obstaclesBounds,
                           Player &player, SoundEngine &soundEngine,
                           FloatingTextSystem &floatingTextSystem)
{
    if (!canAttackPlayer(obstaclesBounds, player)) {
        return false;
    }

    if (this->getAttack() == 0) {
        return false;
    }

    this->attacking = false;

    if (!this->soundPlayed) {
        soundEngine.addSound("punch");
        this->soundPlayed = true;
    }

    return true;
}

const bool
Cyclope::canAttackPlayer(const std::vector<sf::FloatRect> &obstaclesBounds,
                         Player &player)
{
    const float distance = this->attackDistance(player, *this);

    if (distance <= static_cast<float>(this->reach) * 8.f * calcX(64, vm) &&
        !hasLineOfSight(obstaclesBounds,
                        sf::Vector2f(this->getPosition().x + calcX(24, vm),
                                     this->getPosition().y + calcY(36, vm)),
                        player.getCenter())) {
        this->doAttack();
        if (!player.isDead() && !player.isPunched() && this->attacking &&
            this->frame == 80) {
            return true;
        }
    }

    return false;
}
