#include "Projectile.hpp"

Projectile::Projectile(const std::string &t_name, sf::VideoMode &t_vm,
                       float t_x, float t_y, float difficulty_mod,
                       const sf::Vector2f &coords, float coordsOffset)
    : Entity(t_name, t_vm, t_x, t_y)
{

    this->texture.loadFromFile("assets/textures/projectiles.png");
    this->sprite.setTexture(this->texture);
    this->sprite.setScale(calcScale(4, vm), calcScale(4, vm));
    this->sprite.setPosition(t_x, t_y);
    this->sprite.setOrigin(2.f, 2.f);

    this->timeExisting = 0.f;
    this->angle = coordsOffset;
    this->collided = false;
    this->collidedPlayer = false;
    this->collidedMonster = false;
    this->bouncing = false;

    if (this->name == "stone") {
        this->sprite.setTextureRect(sf::IntRect(0, 0, 4, 4));
    }
    else if (this->name == "shuriken") {
        this->sprite.setTextureRect(sf::IntRect(4, 0, 4, 4));
        this->bouncing = true;
    }
    else if (this->name == "bomb") {
        this->sprite.setTextureRect(sf::IntRect(8, 0, 4, 4));
    }
    else if (this->name == "groundWave") {
        this->sprite.setTextureRect(sf::IntRect(12, 0, 8, 8));
    }

    this->calculateVelocity(coords);
}

Projectile::~Projectile() = default;

const bool Projectile::getCollided() const
{
    return this->collided;
}

const bool Projectile::getCollidedPlayer() const
{
    return this->collidedPlayer;
}

const bool Projectile::getCollidedMonster() const
{
    return this->collidedMonster;
}

const float Projectile::getTimeExisting() const
{
    return this->timeExisting;
}

const bool Projectile::getExploded() const
{
    if (this->name == "bomb") {
        return this->timeExisting >= 3.f;
    }
    return false;
}

const bool Projectile::isBomb() const
{
    return this->name == "bomb";
}

void Projectile::calculateVelocity(const sf::Vector2f &coords)
{
    const float addAngle =
        getAngle(this->sprite.getPosition().x, this->sprite.getPosition().y,
                 coords.x, coords.y) +
        90.f;
    if (addAngle + this->angle >= 270.f) {
        this->angle = -90.f + this->angle - (270.f - addAngle);
    }
    else if (addAngle + this->angle <= -90.f) {
        this->angle = 270.f + this->angle + (90.f + addAngle);
    }
    else {
        this->angle += addAngle;
    }
    this->sprite.setRotation(this->angle);
}

void Projectile::wallCollision(const std::vector<sf::FloatRect> &obstacles)
{
    this->collided = false;

    const float distance = 2 * obstacles[0].width;

    for (const auto &obstacleBounds : obstacles) {
        if (vectorDistance(this->sprite.getPosition(),
                           sf::Vector2f(obstacleBounds.left,
                                        obstacleBounds.top)) < distance &&
            !this->collided && !this->collidedPlayer) {

            checkWallCollision(obstacleBounds);
        }
    }
}

void Projectile::checkWallCollision(const sf::FloatRect &obstacleBounds)
{
    sf::FloatRect projectileBounds = this->sprite.getGlobalBounds();
    sf::FloatRect wallBounds = obstacleBounds;

    sf::FloatRect nextPos = projectileBounds;
    nextPos.left += this->velocity.x;
    nextPos.top += this->velocity.y;

    if (!wallBounds.intersects(nextPos)) {
        return;
    }

    if (bottomCollision(projectileBounds, wallBounds)) {
        if (this->bouncing) {
            this->angle *= -1.f;
        }
        else {
            this->sprite.setPosition(projectileBounds.left,
                                     wallBounds.top - projectileBounds.height);
        }
        this->velocity.y = 0.f;
        this->collided = true;
    }
    else if (topCollision(projectileBounds, wallBounds)) {
        if (this->bouncing) {
            this->angle *= -1.f;
        }
        else {
            this->sprite.setPosition(projectileBounds.left,
                                     wallBounds.top + wallBounds.height);
        }
        this->velocity.y = 0.f;
        this->collided = true;
    }

    if (rightCollision(projectileBounds, wallBounds)) {
        if (this->bouncing) {
            this->angle = 180.f - this->angle;
        }
        else {
            this->sprite.setPosition(wallBounds.left - projectileBounds.width,
                                     projectileBounds.top);
        }
        this->velocity.x = 0.f;
        this->collided = true;
    }
    else if (leftCollision(projectileBounds, wallBounds)) {
        if (this->bouncing) {
            this->angle = 180.f - this->angle;
        }
        else {
            this->sprite.setPosition(wallBounds.left + wallBounds.width,
                                     projectileBounds.top);
        }
        this->velocity.x = 0.f;
        this->collided = true;
    }

    if (this->collided) {
        this->HP--;
    }
}

// void Projectile::playerCollision(Player &player)
// {
//     if (this->name == "stone" || this->name == "groundWave") {

//         const float distance = 2 * player->getGlobalBounds().width;

//         if (vectorDistance(this->sprite.getPosition(), player->getPosition())
//         < distance &&
//             !this->collided && !this->collidedPlayer &&
//             !this->collidedMonster) {

//             sf::FloatRect projectileBounds = this->sprite.getGlobalBounds();
//             sf::FloatRect playerBounds = player->getGlobalBounds();

//             sf::FloatRect nextPos = projectileBounds;
//             nextPos.left += this->velocity.x;
//             nextPos.top += this->velocity.y;

//             if (playerBounds.intersects(nextPos)) {
//                 if (bottomCollision(projectileBounds, playerBounds)) {
//                     this->velocity.y = 0.f;
//                     this->sprite.setPosition(projectileBounds.left,
//                                              playerBounds.top -
//                                              projectileBounds.height);
//                     this->collidedPlayer = true;
//                 }
//                 else if (topCollision(projectileBounds, playerBounds)) {
//                     this->velocity.y = 0.f;
//                     this->sprite.setPosition(projectileBounds.left,
//                                              playerBounds.top +
//                                              playerBounds.height);
//                     this->collidedPlayer = true;
//                 }

//                 if (rightCollision(projectileBounds, playerBounds)) {
//                     this->velocity.x = 0.f;
//                     this->sprite.setPosition(playerBounds.left -
//                     projectileBounds.width,
//                                              projectileBounds.top);
//                     this->collidedPlayer = true;
//                 }
//                 else if (leftCollision(projectileBounds, playerBounds)) {
//                     this->velocity.x = 0.f;
//                     this->sprite.setPosition(playerBounds.left +
//                     playerBounds.width,
//                                              projectileBounds.top);
//                     this->collidedPlayer = true;
//                 }
//             }
//         }
//     }
// }

// void Projectile::monsterCollision(Monster &monster, Player &player,
//                                   FloatingTextSystem &floatingTextSystem)
// {
//     if (this->name == "shuriken" || this->name == "bomb") {
//         const float distance = 2 * monster->getGlobalBounds().width;

//         if (vectorDistance(this->sprite.getPosition(),
//         monster->getPosition()) < distance &&
//             !this->collided && !this->collidedPlayer &&
//             !this->collidedMonster) {

//             sf::FloatRect projectileBounds = this->sprite.getGlobalBounds();
//             sf::FloatRect mobBounds = monster->getGlobalBounds();

//             sf::FloatRect nextPos = projectileBounds;
//             nextPos.left += this->velocity.x;
//             nextPos.top += this->velocity.y;

//             if (mobBounds.intersects(nextPos)) {
//                 if (bottomCollision(projectileBounds, mobBounds)) {
//                     this->velocity.y = 0.f;
//                     this->sprite.setPosition(projectileBounds.left,
//                                              mobBounds.top -
//                                              projectileBounds.height);
//                     this->collidedMonster = true;
//                 }
//                 else if (topCollision(projectileBounds, mobBounds)) {
//                     this->velocity.y = 0.f;
//                     this->sprite.setPosition(projectileBounds.left,
//                                              mobBounds.top +
//                                              mobBounds.height);
//                     this->collidedMonster = true;
//                 }

//                 if (rightCollision(projectileBounds, mobBounds)) {
//                     this->velocity.x = 0.f;
//                     this->sprite.setPosition(mobBounds.left -
//                     projectileBounds.width,
//                                              projectileBounds.top);
//                     this->collidedMonster = true;
//                 }
//                 else if (leftCollision(projectileBounds, mobBounds)) {
//                     this->velocity.x = 0.f;
//                     this->sprite.setPosition(mobBounds.left +
//                     mobBounds.width,
//                                              projectileBounds.top);
//                     this->collidedMonster = true;
//                 }
//             }

//             if (this->collidedMonster) {
//                 if ((static_cast<uint32_t>(Random::Float() * 100.f) + 1) <=
//                     player->getCriticalChance()) {
//                     const int attack = 2 * this->attack;
//                     floatingTextSystem->addFloatingText(std::to_string(-attack),
//                     calcChar(16, vm),
//                                                         monster->getPosition().x
//                                                         + calcX(32, vm),
//                                                         monster->getPosition().y
//                                                         + calcY(32, vm),
//                                                         sf::Color(233, 134,
//                                                         39), false);
//                     if (static_cast<int>(monster->getHP() - attack) < 0) {
//                         monster->setHP(0);
//                     }
//                     else {
//                         monster->setHP(monster->getHP() - attack);
//                     }
//                 }
//                 else {
//                     const int attack = this->attack;
//                     floatingTextSystem->addFloatingText(std::to_string(-attack),
//                     calcChar(16, vm),
//                                                         monster->getPosition().x
//                                                         + calcX(32, vm),
//                                                         monster->getPosition().y
//                                                         + calcY(32, vm),
//                                                         sf::Color(255, 255,
//                                                         255), false);
//                     if (static_cast<int>(monster->getHP() - attack) < 0) {
//                         monster->setHP(0);
//                     }
//                     else {
//                         monster->setHP(monster->getHP() - attack);
//                     }
//                 }

//                 monster->punch();

//                 this->HP = 0;
//             }
//         }
//     }
// }

void Projectile::update(float dt)
{
    const float vel = (this->speed * 0.2f + 0.8f) * 16.f *
                      this->sprite.getGlobalBounds().width * dt;

    this->velocity.x = vel * cos((3.1415f / 180.f) * this->angle);
    this->velocity.y = vel * sin((3.1415f / 180.f) * this->angle);
    this->timeExisting += dt;
    if (this->name == "shuriken") {
        this->sprite.rotate(90.f / dt);
    }
}
