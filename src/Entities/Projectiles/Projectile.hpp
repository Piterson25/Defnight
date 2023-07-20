#pragma once

#include "Entities/Monsters/Monster.hpp"
#include "Systems/FloatingTextSystem.hpp"

class Projectile : public Entity {
public:
    Projectile(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
               float t_y, float difficulty_mod, const sf::Vector2f &coords,
               float coordsOffset);
    virtual ~Projectile();

    const bool hasCollidedWall() const;
    const bool hasCollidedPlayer() const;
    const bool hasCollidedMonster() const;
    const float getTimeExisting() const;
    const uint32_t getPiercing() const;
    const bool hasExploded() const;
    const bool isBomb() const;

    void setPiercing(uint32_t t_piercing);
    void calculateVelocity(const sf::Vector2f &coords);
    void wallCollision(const std::vector<sf::FloatRect> &obstacles);
    void checkWallCollision(const sf::FloatRect &obstacleBounds);
    virtual void playerCollision(Player &player) = 0;
    virtual void monsterCollision(Monster &monster, Player &player,
                                  FloatingTextSystem &floatingTextSystem) = 0;
    void update(float dt);

protected:
    float timeExisting;
    bool collidedWall;
    bool collidedPlayer;
    bool collidedMonster;
    float angle;

    uint32_t piercing;
    bool bouncing;
};
