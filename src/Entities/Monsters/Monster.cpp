#include "Monster.hpp"

Monster::Monster(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
                 float t_y, float difficulty_mod, float wave_mod,
                 const std::vector<sf::FloatRect> &obstaclesBounds)
    : Entity(t_name, t_vm, t_x, t_y), difficultyMod(difficulty_mod), gold(0),
      spawned(false), spawnCountdown(0.f), deadCountdown(0.f),
      playedSound(false)
{
    this->texture.loadFromFile("assets/textures/monsters/" + t_name + ".png");
    this->sprite.setTexture(this->texture);
    this->entitySize =
        static_cast<uint32_t>(this->sprite.getGlobalBounds().width / 128);
    this->sprite.setTextureRect(sf::IntRect(0, 32 * this->entitySize,
                                            16 * this->entitySize,
                                            16 * this->entitySize));
    this->sprite.setScale(calcScale(4, vm), calcScale(4, vm));
    this->sprite.setColor(sf::Color(255, 255, 255, 0));

    this->shadow_texture.loadFromFile("assets/textures/entity_shadow.png");
    this->shadow.setTexture(this->shadow_texture);
    this->shadow.setScale(
        calcScale(static_cast<float>(4 * this->entitySize), vm),
        calcScale(static_cast<float>(4 * this->entitySize), vm));
    this->shadow.setPosition(
        this->sprite.getPosition().x,
        this->sprite.getPosition().y +
            calcY(static_cast<float>(52 * this->entitySize), vm));
    this->shadow.setColor(sf::Color(255, 255, 255, 0));

    this->AI = std::make_unique<AIComponent>(this->vm, obstaclesBounds);
}

Monster::~Monster() = default;

const float Monster::getDifficultyMod() const
{
    return this->difficultyMod;
}

const uint32_t Monster::getGold() const
{
    return this->gold;
}

const bool Monster::getSpawned() const
{
    return this->spawned;
}

const bool Monster::getDeadCountdown() const
{
    return this->deadCountdown >= 0.25f;
}

void Monster::setGold(uint32_t gold)
{
    this->gold = gold;
}

const bool
Monster::attackPlayer(const std::vector<sf::FloatRect> &obstaclesBounds,
                      Player &player, SoundEngine &soundEngine,
                      FloatingTextSystem &floatingTextSystem)
{
    const float distance = this->attackDistance(player, *this);

    if (distance <=
        this->reach * calcX(static_cast<float>(32 * this->entitySize), vm)) {
        this->doAttack();
        if (!player.isDead() && !player.getPunched() && this->isAttacking &&
            this->frame == 80 * this->entitySize) {
            int Lattack = static_cast<int>(round(
                this->attack - (this->attack * player.getArmor() * 0.05f)));

            if (Lattack > 0) {
                floatingTextSystem.addFloatingText(
                    std::to_string(-Lattack), calcChar(16, vm),
                    player.getPosition().x + calcX(48, vm),
                    player.getPosition().y + calcY(32, vm),
                    sf::Color(228, 92, 95), false);
                if (static_cast<int>(player.getHP() - Lattack) < 0) {
                    player.setHP(0);
                }
                else {
                    player.setHP(player.getHP() - Lattack);
                }

                if (!this->playedSound) {
                    soundEngine.addSound("punch");
                    this->playedSound = true;
                }

                player.punch();

                return true;
            }
        }
    }

    return false;
}

const bool
Monster::sightCollision(const std::vector<sf::FloatRect> &obstaclesBounds,
                        const sf::Vector2f &a_p1, const sf::Vector2f &a_p2)
{
    for (const auto &obstacle : obstaclesBounds) {
        if (vectorDistance(this->sprite.getPosition(),
                           sf::Vector2f(obstacle.left, obstacle.top)) <=
                20.f * calcX(32, vm) &&
            sight(obstacle, a_p1, a_p2)) {
            return true;
        }
    }
    return false;
}

void Monster::spawn(float dt)
{
    if (this->spawnCountdown < 0.5f) {
        this->sprite.setColor(
            sf::Color(255, 255, 255,
                      static_cast<sf::Uint8>(this->spawnCountdown * 510.f)));
        this->shadow.setColor(
            sf::Color(255, 255, 255,
                      static_cast<sf::Uint8>(this->spawnCountdown * 510.f)));
        this->spawnCountdown += dt;
    }
    if (this->spawnCountdown >= 0.5f) {
        this->spawned = true;
        this->sprite.setColor(sf::Color::White);
        this->shadow.setColor(sf::Color::White);
    }
}

const bool Monster::dyingAnimation(float dt)
{
    if (this->deadCountdown < 0.25f) {
        this->sprite.setColor(sf::Color(
            255, 255, 255,
            static_cast<sf::Uint8>(255.f - (this->deadCountdown * 1020.f))));
        this->shadow.setColor(sf::Color(
            255, 255, 255,
            static_cast<sf::Uint8>(255.f - (this->deadCountdown * 1020.f))));
        this->deadCountdown += dt;
    }
    if (this->deadCountdown >= 0.25f) {
        this->sprite.setColor(sf::Color(255, 255, 255, 0));
        this->shadow.setColor(sf::Color(255, 255, 255, 0));
        return true;
    }
    return false;
}

void Monster::calculateAI(const std::vector<sf::FloatRect> &obstaclesBounds,
                          Player &player,
                          const std::vector<sf::Vector2f> &positions, float dt)
{
    this->velocity = sf::Vector2f(0.f, 0.f);
    const float vel = ((this->speed * 0.2f + 0.8f) * 2 *
                       this->sprite.getGlobalBounds().width) *
                      dt;

    if (!sightCollision(obstaclesBounds, this->getCenter(),
                        player.getCenter())) {

        if (player.getCenter().x > this->getCenter().x) {
            this->velocity.x += vel;
        }
        else if (player.getCenter().x < this->getCenter().x) {
            this->velocity.x += -(vel);
        }
        if (player.getCenter().y > this->getCenter().y) {
            this->velocity.y += vel;
        }
        else if (player.getCenter().y < this->getCenter().y) {
            this->velocity.y += -(vel);
        }

        this->AI->disableAI();
    }
    else if (this->name != "minotaur") {
        this->AI->update(*this, player, positions, vel);
    }

    if (hasVelocity()) {
        this->velocity.x /= 1.44f;
        this->velocity.y /= 1.44f;
    }
}

void Monster::update(float dt)
{
    this->shadow.setPosition(
        this->sprite.getPosition().x,
        this->sprite.getPosition().y +
            calcY(static_cast<float>(52 * this->entitySize), this->vm));
    if (this->playedSound && this->frame != 80) {
        this->playedSound = false;
    }
}

void Monster::draw(sf::RenderTarget &target)
{
    target.draw(this->sprite);
}

void Monster::drawShadow(sf::RenderTarget &target)
{
    target.draw(this->shadow);
}

const bool
Monster::checkAttack(const std::vector<sf::FloatRect> &obstaclesBounds,
                     Player &player)
{
    const float distance = this->attackDistance(player, *this);

    if (distance <= this->getReach() *
                        calcX(static_cast<float>(32 * this->entitySize), vm)) {
        this->doAttack();
        if (!player.isDead() && !player.getPunched() && this->IsAttacking() &&
            this->getFrame() == 80 * this->entitySize) {
            return true;
        }
    }

    return false;
}
