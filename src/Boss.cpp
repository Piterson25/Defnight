#include "Boss.hpp"
#include "Functions.hpp"

Boss::Boss(const sf::VideoMode &vm, const std::string &monster_name, sf::Texture &texture,
           sf::Texture &shadow_texture, TileMap *tileMap, const float &x, const float &y,
           const float &difficulty_mod, const float &wave_mod, const uint32_t &monsterSize)
    : Monster(vm, monster_name, texture, shadow_texture, tileMap, x, y, difficulty_mod, wave_mod)
{
    this->vm = vm;
    this->name = monster_name;
    this->texture = texture;
    this->shadow_texture = shadow_texture;

    this->monsterSize = monsterSize;

    this->sprite.setTexture(this->texture);
    this->sprite.setTextureRect(
        sf::IntRect(0, 32 * this->monsterSize, 16 * this->monsterSize, 16 * this->monsterSize));
    this->sprite.setScale(calcScale(4, vm), calcScale(4, vm));
    this->sprite.setPosition(x, y);
    this->sprite.setColor(sf::Color(255, 255, 255, 0));

    this->shadow.setTexture(this->shadow_texture);
    this->shadow.setScale(calcScale(static_cast<float>(4 * this->monsterSize), vm),
                          calcScale(static_cast<float>(4 * this->monsterSize), vm));
    this->shadow.setPosition(this->sprite.getPosition().x,
                             this->sprite.getPosition().y +
                                 calcY(static_cast<float>(52 * this->monsterSize), vm));
    this->shadow.setColor(sf::Color(255, 255, 255, 0));

    this->reach = 1;
    this->spawnCountdown = 0.f;
    this->deadCountdown = 0.f;
    this->spawned = false;
    this->specialAttackCountdown = 0.f;

    this->playedSound = false;

    this->AI = new AIComponent(tileMap, this->vm);

    if (this->name == "minotaur") {
        this->attack = static_cast<uint32_t>(7 * difficulty_mod);
        this->attackSpeed = 2;
        this->HP = static_cast<uint32_t>(69 * difficulty_mod);
        this->maxHP = static_cast<uint32_t>(69 * difficulty_mod);
        this->speed = 1;
        this->gold = static_cast<uint32_t>(50 * wave_mod);
        this->XP = static_cast<uint32_t>(200 * wave_mod);
    }
}

Boss::~Boss()
{
    delete this->AI;
}

void Boss::specialAttack(const float &dt, ProjectileSystem *projectileSystem,
                         SoundEngine *soundEngine)
{
    this->specialAttackCountdown += dt;
    if (this->specialAttackCountdown >= 5.f) {
        this->specialAttackCountdown = 0.f;
        projectileSystem->addProjectile("groundWave", this->getCenter().x, this->getCenter().y,
                                        this->getAttack(), 1, 2, sf::Vector2f(this->getUpCenter()),
                                        0, false);
        projectileSystem->addProjectile(
            "groundWave", this->getCenter().x, this->getCenter().y, this->getAttack(), 1, 2,
            sf::Vector2f(this->getRightCenter().x, this->getUpCenter().y), 0, false);
        projectileSystem->addProjectile("groundWave", this->getCenter().x, this->getCenter().y,
                                        this->getAttack(), 1, 2,
                                        sf::Vector2f(this->getRightCenter()), 0, false);
        projectileSystem->addProjectile(
            "groundWave", this->getCenter().x, this->getCenter().y, this->getAttack(), 1, 2,
            sf::Vector2f(this->getRightCenter().x, this->getDownCenter().y), 0, false);
        projectileSystem->addProjectile("groundWave", this->getCenter().x, this->getCenter().y,
                                        this->getAttack(), 1, 2,
                                        sf::Vector2f(this->getDownCenter()), 0, false);
        projectileSystem->addProjectile(
            "groundWave", this->getCenter().x, this->getCenter().y, this->getAttack(), 1, 2,
            sf::Vector2f(this->getLeftCenter().x, this->getDownCenter().y), 0, false);
        projectileSystem->addProjectile("groundWave", this->getCenter().x, this->getCenter().y,
                                        this->getAttack(), 1, 2,
                                        sf::Vector2f(this->getLeftCenter()), 0, false);
        projectileSystem->addProjectile(
            "groundWave", this->getCenter().x, this->getCenter().y, this->getAttack(), 1, 2,
            sf::Vector2f(this->getLeftCenter().x, this->getUpCenter().y), 0, false);
        if (!this->playedSound) {
            soundEngine->addSound("punch");
            this->playedSound = true;
        }
    }
}

void Boss::update(const float &dt, ProjectileSystem *projectileSystem, SoundEngine *soundEngine)
{
    this->shadow.setPosition(this->sprite.getPosition().x,
                             this->sprite.getPosition().y +
                                 calcY(static_cast<float>(52 * this->monsterSize), this->vm));
    if (this->playedSound && this->frame != 80) {
        this->playedSound = false;
    }

    this->specialAttack(dt, projectileSystem, soundEngine);
}

void Boss::draw(sf::RenderTarget &target)
{
    target.draw(this->sprite);
}

void Boss::drawShadow(sf::RenderTarget &target)
{
    target.draw(this->shadow);
}
