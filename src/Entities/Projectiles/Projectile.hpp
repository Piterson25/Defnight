#pragma once

#include "Entities/Monsters/Monster.hpp"
#include "Systems/FloatingTextSystem.hpp"

class Projectile : public Entity {
public:
    Projectile(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
               float t_y, float difficulty_mod, const sf::Vector2f &coords,
               float coordsOffset);
    virtual ~Projectile();

    const bool getCollided() const;
    const bool getCollidedPlayer() const;
    const bool getCollidedMonster() const;
    const float getTimeExisting() const;
    const bool getExploded() const;
    const bool isBomb() const;

    void calculateVelocity(const sf::Vector2f &coords);
    void wallCollision(const std::vector<sf::FloatRect> &obstacles);
    void checkWallCollision(const sf::FloatRect &obstacleBounds);
    virtual void playerCollision(Player &player) = 0;
    virtual void monsterCollision(Monster &monster, Player &player,
                                  FloatingTextSystem &floatingTextSystem) = 0;
    void update(float dt);

protected:
    float timeExisting;
    bool collided;
    bool collidedPlayer;
    bool collidedMonster;
    float angle;

    bool bouncing;
};
