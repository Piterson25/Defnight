#include "Entity.hpp"

Entity::Entity(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
               float t_y)
    : name(t_name), vm(t_vm)
{
    this->sprite.setPosition(t_x, t_y);

    this->velocity = sf::Vector2f(0.f, 0.f);

    this->attack = 0;
    this->attackSpeed = 0;
    this->maxHP = 0;
    this->HP = 0;
    this->XP = 0;
    this->speed = 0;
    this->reach = 1;

    this->faceDirection = FaceDirections::LEFT;

    this->attackCooldown = 0.f;
    this->attacking = false;
    this->punchedCooldown = 0.f;
    this->punched = false;

    this->animationCooldown = 0.f;
    this->frame = 0;

    this->entitySize = static_cast<uint32_t>(calcX(64.f, vm));
}

Entity::~Entity() = default;

const std::string Entity::getName() const
{
    return this->name;
}

const sf::Vector2f Entity::getPosition() const
{
    return this->sprite.getPosition();
}

const sf::FloatRect Entity::getGlobalBounds() const
{
    return this->sprite.getGlobalBounds();
}

const sf::Vector2f Entity::getCenter() const
{
    return sf::Vector2f(this->sprite.getPosition().x +
                            (this->sprite.getGlobalBounds().width * 0.5f),
                        this->sprite.getPosition().y +
                            (this->sprite.getGlobalBounds().height * 0.5f));
}

const sf::Vector2f Entity::getUpCenter() const
{
    return sf::Vector2f(this->sprite.getPosition().x +
                            (this->sprite.getGlobalBounds().width * 0.5f),
                        this->sprite.getPosition().y +
                            (this->sprite.getGlobalBounds().height * 0.125f));
}

const sf::Vector2f Entity::getRightCenter() const
{
    return sf::Vector2f(this->sprite.getPosition().x +
                            (this->sprite.getGlobalBounds().width * 0.875f),
                        this->sprite.getPosition().y +
                            (this->sprite.getGlobalBounds().height * 0.5f));
}

const sf::Vector2f Entity::getDownCenter() const
{
    return sf::Vector2f(this->sprite.getPosition().x +
                            (this->sprite.getGlobalBounds().width * 0.5f),
                        this->sprite.getPosition().y +
                            (this->sprite.getGlobalBounds().height * 0.875f));
}

const sf::Vector2f Entity::getLeftCenter() const
{
    return sf::Vector2f(this->sprite.getPosition().x +
                            (this->sprite.getGlobalBounds().width * 0.125f),
                        this->sprite.getPosition().y +
                            (this->sprite.getGlobalBounds().height * 0.5f));
}

const sf::Texture Entity::getTexture() const
{
    return this->texture;
}

const sf::Vector2f Entity::getVelocity() const
{
    return this->velocity;
}

const uint32_t Entity::getAttack() const
{
    return this->attack;
}

const uint32_t Entity::getAttackSpeed() const
{
    return this->attackSpeed;
}

const uint32_t Entity::getMaxHP() const
{
    return this->maxHP;
}

const uint32_t Entity::getHP() const
{
    return this->HP;
}

const uint32_t Entity::getXP() const
{
    return this->XP;
}

const uint32_t Entity::getSpeed() const
{
    return this->speed;
}

const uint32_t Entity::getReach() const
{
    return this->reach;
}

const bool Entity::isFacingUp() const
{
    return this->faceDirection == FaceDirections::UP;
}

const bool Entity::isFacingRight() const
{
    return this->faceDirection == FaceDirections::RIGHT;
}

const bool Entity::isFacingDown() const
{
    return this->faceDirection == FaceDirections::DOWN;
}

const bool Entity::isFacingLeft() const
{
    return this->faceDirection == FaceDirections::LEFT;
}

const bool Entity::isAttacking() const
{
    return this->attacking;
}

const bool Entity::isPunched() const
{
    return this->punched;
}

const uint32_t Entity::getFrame() const
{
    return this->frame;
}

void Entity::setName(const std::string &name)
{
    this->name = name;
}

void Entity::setPosition(float x, float y)
{
    this->sprite.setPosition(x, y);
}

void Entity::setTexture(const sf::Texture &texture)
{
    this->sprite.setTexture(texture);
}

void Entity::setTexturePath(const std::string &path)
{
    this->texture.loadFromFile(path);
    this->sprite.setTexture(this->texture);
}

void Entity::setVeloctiy(const sf::Vector2f &velocity)
{
    this->velocity = velocity;
}

void Entity::setAttack(const uint32_t attack)
{
    this->attack = attack;
}

void Entity::setAttackSpeed(const uint32_t attackSpeed)
{
    this->attackSpeed = attackSpeed;
}

void Entity::setMaxHP(const uint32_t maxHP)
{
    this->maxHP = maxHP;
}

void Entity::setHP(const uint32_t HP)
{
    this->HP = HP;
}

void Entity::setXP(const uint32_t XP)
{
    this->XP = XP;
}

void Entity::setSpeed(const uint32_t speed)
{
    this->speed = speed;
}

void Entity::setReach(const uint32_t reach)
{
    this->reach = reach;
}

const bool Entity::hasCollidedTop(const sf::FloatRect &e1Bounds,
                                  const sf::FloatRect &e2Bounds) const
{
    if (e1Bounds.top > e2Bounds.top &&
        e1Bounds.top + e1Bounds.height > e2Bounds.top + e2Bounds.height &&
        e1Bounds.left < e2Bounds.left + e2Bounds.width &&
        e1Bounds.left + e1Bounds.width > e2Bounds.left) {
        return true;
    }
    return false;
}

const bool Entity::hasCollidedRight(const sf::FloatRect &e1Bounds,
                                    const sf::FloatRect &e2Bounds) const
{
    if (e1Bounds.left < e2Bounds.left &&
        e1Bounds.left + e1Bounds.width < e2Bounds.left + e2Bounds.width &&
        e1Bounds.top < e2Bounds.top + e2Bounds.height &&
        e1Bounds.top + e1Bounds.height > e2Bounds.top) {
        return true;
    }
    return false;
}

const bool Entity::hasCollidedLeft(const sf::FloatRect &e1Bounds,
                                   const sf::FloatRect &e2Bounds) const
{
    if (e1Bounds.left > e2Bounds.left &&
        e1Bounds.left + e1Bounds.width > e2Bounds.left + e2Bounds.width &&
        e1Bounds.top < e2Bounds.top + e2Bounds.height &&
        e1Bounds.top + e1Bounds.height > e2Bounds.top) {
        return true;
    }
    return false;
}

const bool Entity::hasCollidedBottom(const sf::FloatRect &e1Bounds,
                                     const sf::FloatRect &e2Bounds) const
{
    if (e1Bounds.top < e2Bounds.top &&
        e1Bounds.top + e1Bounds.height < e2Bounds.top + e2Bounds.height &&
        e1Bounds.left < e2Bounds.left + e2Bounds.width &&
        e1Bounds.left + e1Bounds.width > e2Bounds.left) {
        return true;
    }
    return false;
}

const float Entity::attackDistance(const Entity &e1, const Entity &e2) const
{
    if (isFacingLeft()) {
        return vectorDistance(e1.getCenter(), e2.getLeftCenter());
    }
    else if (isFacingRight()) {
        return vectorDistance(e1.getCenter(), e2.getRightCenter());
    }
    else if (isFacingUp()) {
        return vectorDistance(e1.getCenter(), e2.getUpCenter());
    }
    else if (isFacingDown()) {
        return vectorDistance(e1.getCenter(), e2.getDownCenter());
    }
    return 1000.f;
}

const bool Entity::hasVelocity() const
{
    return this->velocity.x != 0.f || this->velocity.y != 0.f;
}

const bool Entity::isDead() const
{
    return this->HP == 0;
}

void Entity::obstacleCollision(const std::vector<sf::FloatRect> &obstacles)
{
    const float distance = 2 * obstacles[0].width;

    for (const auto &obstacleBounds : obstacles) {
        if (vectorDistance(this->sprite.getPosition(),
                           sf::Vector2f(obstacleBounds.left,
                                        obstacleBounds.top)) < distance) {
            checkCollision(obstacleBounds);
        }
    }
}

void Entity::checkCollision(const sf::FloatRect &obstacleBounds)
{
    sf::FloatRect spriteBounds = this->sprite.getGlobalBounds();
    sf::FloatRect nextPos = spriteBounds;
    nextPos.left += this->velocity.x;
    nextPos.top += this->velocity.y;

    if (obstacleBounds.intersects(nextPos)) {
        if (hasCollidedBottom(spriteBounds, obstacleBounds)) {
            this->velocity.y = 0.f;
            this->sprite.setPosition(spriteBounds.left,
                                     obstacleBounds.top - spriteBounds.height);
        }
        else if (hasCollidedTop(spriteBounds, obstacleBounds)) {
            this->velocity.y = 0.f;
            this->sprite.setPosition(
                spriteBounds.left, obstacleBounds.top + obstacleBounds.height);
        }

        if (hasCollidedRight(spriteBounds, obstacleBounds)) {
            this->velocity.x = 0.f;
            this->sprite.setPosition(obstacleBounds.left - spriteBounds.width,
                                     spriteBounds.top);
        }
        else if (hasCollidedLeft(spriteBounds, obstacleBounds)) {
            this->velocity.x = 0.f;
            this->sprite.setPosition(obstacleBounds.left + obstacleBounds.width,
                                     spriteBounds.top);
        }
    }
}

void Entity::move()
{
    this->sprite.move(this->velocity);
}

void Entity::loadAttack(float dt)
{
    if (this->attackCooldown < 1.f) {
        this->attackCooldown += (this->attackSpeed * 0.2f + 0.8f) * dt;
    }
}

void Entity::doAttack()
{
    if (!this->attacking && this->attackCooldown >= 1.f) {
        this->attacking = true;
        this->attackCooldown = 0.f;
        this->frame = 0;
    }
}

void Entity::punch()
{
    this->punched = true;
    this->punchedCooldown = 0.f;
    if (this->HP == 0) {
        this->sprite.setColor(sf::Color(182, 60, 53));
    }
}

void Entity::smashed(const float dt)
{
    if (this->punched && !this->isDead()) {
        if (this->punchedCooldown < 1.f) {
            this->punchedCooldown += 5.f * dt;
            this->sprite.setColor(sf::Color(182, 60, 53));
        }
        if (this->punchedCooldown >= 1.f) {
            this->punchedCooldown = 0.f;
            this->sprite.setColor(sf::Color::White);
            this->punched = false;
        }
    }
}

void Entity::animation(float dt)
{
    if (this->velocity.x < 0) {
        this->faceDirection = FaceDirections::LEFT;
    }
    else if (this->velocity.x > 0) {
        this->faceDirection = FaceDirections::RIGHT;
    }
    else {
        if (this->velocity.y < 0) {
            this->faceDirection = FaceDirections::UP;
        }
        else if (this->velocity.y > 0) {
            this->faceDirection = FaceDirections::DOWN;
        }
    }

    if (this->attacking) {
        const int animationOffset = hasVelocity() ? 8 : 4;
        attackAnimation(animationOffset, dt);
    }
    else if (hasVelocity()) {
        walkAnimation(dt);
    }
}

void Entity::draw(sf::RenderTarget &target)
{
    target.draw(this->sprite);
}

void Entity::attackAnimation(int offsetY, float dt)
{
    int y = 0;
    if (isFacingLeft()) {
        y = 2 + offsetY;
    }
    else if (isFacingRight()) {
        y = 3 + offsetY;
    }
    else {
        if (isFacingUp()) {
            y = 0 + offsetY;
        }
        else if (isFacingDown()) {
            y = 1 + offsetY;
        }
    }

    this->animationCooldown += (this->attackSpeed * 0.2f + 0.8f) * 8.f * dt;
    if (this->animationCooldown >= 1.f) {
        this->animationCooldown = 0.f;

        sf::IntRect intRect(this->frame, y * 16 * entitySize, 16 * entitySize,
                            16 * entitySize);
        this->sprite.setTextureRect(intRect);

        if (this->frame == 112 * entitySize) {
            this->frame = 0;
            this->attacking = false;
        }
        else {
            this->frame += 16 * entitySize;
        }
    }
}

void Entity::walkAnimation(float dt)
{
    int y = 0;
    if (this->velocity.x < 0) {
        y = 2;
    }
    else if (this->velocity.x > 0) {
        y = 3;
    }
    else {
        if (this->velocity.y < 0) {
            y = 0;
        }
        else if (this->velocity.y > 0) {
            y = 1;
        }
    }

    this->animationCooldown += (this->speed * 0.2f + 0.8f) * 4.f * dt;
    if (this->animationCooldown >= 1.f) {
        this->animationCooldown = 0.f;

        sf::IntRect intRect(this->frame, y * 16 * entitySize, 16 * entitySize,
                            16 * entitySize);
        this->sprite.setTextureRect(intRect);

        if (this->frame == 112 * entitySize) {
            this->frame = 0;
        }
        else {
            this->frame += 16 * entitySize;
        }
    }
}
