#pragma once

#include "Entities/Entity.hpp"

class AIComponent {
public:
    AIComponent(sf::VideoMode &vm,
                const std::vector<sf::FloatRect> &obstaclesBounds);
    ~AIComponent();

    void disableAI();
    void update(Entity &e1, Entity &e2,
                const std::vector<sf::Vector2f> &positions, float vel);
    void draw(sf::RenderTarget &target);

private:
    struct Node {
        bool isWall = false;
        bool isMonster = false;
        bool isVisited = false;
        float FCost = 0.f;
        float HCost = 0.f;
        int x = 0;
        int y = 0;
        std::vector<Node *> Neighbours = {};
        Node *parent = nullptr;
    };
    std::vector<Node *> Nodes;
    Node *Start;
    Node *End;
    Node *Current;

    void initNodes();
    void initNeighbours(size_t x, size_t y);
    void resetNodes(Entity &e1, Entity &e2,
                    const std::vector<sf::Vector2f> &positions);
    void resetSingleNode(size_t x, size_t y);
    void AStarAlg();
    void processNextNode(Node *currentNode, std::vector<Node *> &nodesToTest);
    void checkPositions(Entity &e1, Entity &e2,
                        const std::vector<sf::Vector2f> &positions, float vel);

    const size_t CollumsX = 32;
    const size_t CollumsY = 32;

    sf::Vector2f position;
    sf::Vector2f target_position;
    sf::VideoMode &vm;

    bool activateAI;

    sf::Vector2f BlockSize;

    std::vector<sf::RectangleShape> greens;
};
