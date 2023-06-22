#pragma once

#include "Utils/Functions.hpp"

class Entity {
public:
    Entity(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
           float t_y);
    virtual ~Entity();

    const std::string getName() const;
    const sf::Vector2f getPosition() const;
    const sf::FloatRect getGlobalBounds() const;
    const sf::Vector2f getCenter() const;
    const sf::Vector2f getUpCenter() const;
    const sf::Vector2f getRightCenter() const;
    const sf::Vector2f getDownCenter() const;
    const sf::Vector2f getLeftCenter() const;
    const sf::Texture getTexture() const;
    const sf::Vector2f getVelocity() const;
    const uint32_t getAttack() const;
    const uint32_t getAttackSpeed() const;
    const uint32_t getMaxHP() const;
    const uint32_t getHP() const;
    const uint32_t getXP() const;
    const uint32_t getSpeed() const;
    const uint32_t getReach() const;
    const bool isFacingUp() const;
    const bool isFacingRight() const;
    const bool isFacingDown() const;
    const bool isFacingLeft() const;
    const bool isAttacking() const;
    const bool isPunched() const;
    const uint32_t getFrame() const;

    void setName(const std::string &t_name);
    void setPosition(float t_x, float t_y);
    void setTexture(const sf::Texture &t_texture);
    void setTexturePath(const std::string &t_path);
    void setVeloctiy(const sf::Vector2f &t_velocity);
    void setAttack(uint32_t t_attack);
    void setAttackSpeed(uint32_t t_attackSpeed);
    void setMaxHP(uint32_t t_maxHP);
    void setHP(uint32_t t_HP);
    void setXP(uint32_t t_XP);
    void setSpeed(uint32_t t_speed);
    void setReach(uint32_t t_reach);

    const bool hasCollidedTop(const sf::FloatRect &e1Bounds,
                              const sf::FloatRect &e2Bounds) const;
    const bool hasCollidedRight(const sf::FloatRect &e1Bounds,
                                const sf::FloatRect &e2Bounds) const;
    const bool hasCollidedLeft(const sf::FloatRect &e1Bounds,
                               const sf::FloatRect &e2Bounds) const;
    const bool hasCollidedBottom(const sf::FloatRect &e1Bounds,
                                 const sf::FloatRect &e2Bounds) const;
    const float attackDistance(const Entity &e1, const Entity &e2) const;
    const bool hasVelocity() const;
    const bool isDead() const;
    void obstacleCollision(const std::vector<sf::FloatRect> &obstaclesBounds);
    void checkCollision(const sf::FloatRect &obstacleBounds);
    void move();
    void loadAttack(float dt);
    void doAttack();
    void punch();
    void smashed(float dt);
    void animation(float dt);

    virtual void draw(sf::RenderTarget &target);

protected:
    void attackAnimation(int offsetY, float dt);
    void walkAnimation(float dt);
    std::string name;

    sf::VideoMode &vm;
    sf::Sprite sprite;
    sf::Texture texture;

    sf::Vector2f velocity;

    uint32_t attack;
    uint32_t attackSpeed;
    uint32_t maxHP;
    uint32_t HP;
    uint32_t XP;
    uint32_t speed;
    uint32_t reach;

    enum class FaceDirections { UP, RIGHT, DOWN, LEFT };

    FaceDirections faceDirection;

    float attackCooldown;
    bool attacking;
    float punchedCooldown;
    bool punched;

    float animationCooldown;
    uint32_t entitySize;
    uint32_t frame;
};
