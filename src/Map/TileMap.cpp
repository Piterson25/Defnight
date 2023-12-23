#include "TileMap.hpp"

TileMap::TileMap(sf::VideoMode &t_vm, const std::string &mapName) : vm(t_vm)
{
    backgroundTexture.loadFromFile("assets/textures/maps/" + mapName + ".png");
    background.setTexture(backgroundTexture);
    background.setScale(calcScale(4, vm), calcScale(4, vm));

    tilesTexture.loadFromFile("assets/textures/tiles.png");
    vertexArray.setPrimitiveType(sf::Quads);
    vertexArray.resize(static_cast<size_t>(calcX(64 * 64 * 4, vm)));
    std::ifstream map("assets/maps/" + mapName + ".txt");

    float offsetY = 0.f;
    if (mapName == "desolation") {
        offsetY = 48.f;
    }
    else if (mapName == "permafrost") {
        offsetY = 96.f;
    }

    const auto tile = sf::Vector2f(calcX(64, vm), calcY(64, vm));
    if (map.is_open()) {
        float x = 0.f, y = 0.f;
        const float pos = calcX(64, vm);
        size_t t = 0;
        std::string temp;
        while (std::getline(map, temp)) {
            for (size_t i = 0; i < temp.size(); ++i) {
                if (temp[i] == '#') {
                    addTile("wall", tile, x, y);
                    sf::Vertex *quad = &vertexArray[t * 4];

                    quad[0].position = sf::Vector2f(x, y);
                    quad[1].position = sf::Vector2f(x + calcX(64, vm), y);
                    quad[2].position =
                        sf::Vector2f(x + calcX(64, vm), y + calcY(64, vm));
                    quad[3].position = sf::Vector2f(x, y + calcY(64, vm));

                    quad[0].texCoords = sf::Vector2f(48, 16 + offsetY);
                    quad[1].texCoords = sf::Vector2f(64, 16 + offsetY);
                    quad[2].texCoords = sf::Vector2f(64, 32 + offsetY);
                    quad[3].texCoords = sf::Vector2f(48, 32 + offsetY);
                }
                else if (temp[i] == '@') {
                    addTile("wall", tile, x, y);
                    sf::Vertex *quad = &vertexArray[t * 4];

                    quad[0].position = sf::Vector2f(x, y);
                    quad[1].position = sf::Vector2f(x + calcX(64, vm), y);
                    quad[2].position =
                        sf::Vector2f(x + calcX(64, vm), y + calcY(64, vm));
                    quad[3].position = sf::Vector2f(x, y + calcY(64, vm));

                    quad[0].texCoords = sf::Vector2f(64, 16 + offsetY);
                    quad[1].texCoords = sf::Vector2f(80, 16 + offsetY);
                    quad[2].texCoords = sf::Vector2f(80, 32 + offsetY);
                    quad[3].texCoords = sf::Vector2f(64, 32 + offsetY);
                }
                x += pos;
                t++;
            }
            x = 0.f;
            y += pos;
        }
    }
    map.close();
}

TileMap::~TileMap()
{
    tiles.clear();
}

const sf::FloatRect TileMap::getBounds() const
{
    return background.getGlobalBounds();
}

const sf::Vector2f TileMap::getMapSize() const
{
    return sf::Vector2f(vertexArray.getBounds().width,
                        vertexArray.getBounds().height);
}

const size_t TileMap::getSize() const
{
    return tiles.size();
}

const sf::Vector2f TileMap::getPosition(const size_t &index) const
{
    return tiles[index]->getPosition();
}

const sf::FloatRect TileMap::getGlobalBounds(const size_t &index) const
{
    return tiles[index]->getGlobalBounds();
}

const std::vector<sf::FloatRect> TileMap::getTilesGlobalBounds() const
{
    return tilesGlobalBounds;
}

void TileMap::addTile(const std::string &name, const sf::Vector2f &size,
                      float x, float y)
{
    tiles.emplace_back(std::make_unique<Tile>(name, size, x, y));
    tilesGlobalBounds.emplace_back(sf::FloatRect(sf::Vector2f(x, y), size));
}

void TileMap::drawMap(sf::RenderTarget &target)
{
    target.draw(background);
}

void TileMap::drawObstacles(sf::RenderTarget &target)
{
    target.draw(this->vertexArray, &this->tilesTexture);
}
