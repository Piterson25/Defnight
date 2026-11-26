#include "TileMap.hpp"

TileMap::TileMap(sf::VideoMode &t_vm, const std::string &mapName)
    : vm(t_vm), backgroundTexture("assets/textures/maps/" + toLowerCase(mapName) + ".png"),
      background(backgroundTexture)
{
    background = sf::Sprite(backgroundTexture);
    background.setScale({calcScale(4, vm), calcScale(4, vm)});

    tilesTexture = sf::Texture("assets/textures/tiles.png");
    vertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
    vertexArray.resize(static_cast<size_t>(calcX(64 * 64 * 6, vm)));
    std::ifstream map("assets/maps/" + toLowerCase(mapName) + ".txt");

    float offsetX = 0.f;
    if (mapName == "DESOLATION") {
        offsetX = 48.f;
    }
    else if (mapName == "PERMAFROST") {
        offsetX = 96.f;
    }
    else if (mapName == "VOLCANO") {
        offsetX = 144.f;
    }

    if (!shader.loadFromFile("shaders/light_fragment.glsl", sf::Shader::Type::Fragment)) {
        throw("ERROR - COULDN'T FIND LIGHT SHADER");
    }

    const float LIGHT_RADIUS = calcX(256, vm);
    shader.setUniform("windowSizeY", static_cast<float>(vm.size.y));
    shader.setUniform("lightColor", sf::Glsl::Vec4(sf::Color(255, 240, 43, 127)));

    const auto tile = sf::Vector2f(calcX(64, vm), calcY(64, vm));
    if (map.is_open()) {
        float x = 0.f, y = 0.f;
        const float pos = calcX(64, vm);
        size_t t = 0;
        std::string temp;
        const float tileSize = calcX(64, vm);
        while (std::getline(map, temp)) {
            for (size_t i = 0; i < temp.size(); ++i) {
                if (temp[i] == '#') {
                    addTile("wall", tile, x, y);
                    sf::Vertex *triangles = &vertexArray[t * 6];
                    triangles[0].position = sf::Vector2f(x, y);
                    triangles[1].position = sf::Vector2f(x + tileSize, y);
                    triangles[2].position = sf::Vector2f(x + tileSize, y + tileSize);
                    triangles[3].position = sf::Vector2f(x, y);
                    triangles[4].position = sf::Vector2f(x + tileSize, y + tileSize);
                    triangles[5].position = sf::Vector2f(x, y + tileSize);

                    triangles[0].texCoords = sf::Vector2f(64 + offsetX, 48);
                    triangles[1].texCoords = sf::Vector2f(80 + offsetX, 48);
                    triangles[2].texCoords = sf::Vector2f(80 + offsetX, 64);
                    triangles[3].texCoords = sf::Vector2f(64 + offsetX, 48);
                    triangles[4].texCoords = sf::Vector2f(80 + offsetX, 64);
                    triangles[5].texCoords = sf::Vector2f(64 + offsetX, 64);
                }
                else if (temp[i] == '@') {
                    addTile("wall", tile, x, y);
                    sf::Vertex *triangles = &vertexArray[t * 6];
                    triangles[0].position = sf::Vector2f(x, y);
                    triangles[1].position = sf::Vector2f(x + tileSize, y);
                    triangles[2].position = sf::Vector2f(x + tileSize, y + tileSize);
                    triangles[3].position = sf::Vector2f(x, y);
                    triangles[4].position = sf::Vector2f(x + tileSize, y + tileSize);
                    triangles[5].position = sf::Vector2f(x, y + tileSize);

                    triangles[0].texCoords = sf::Vector2f(64 + offsetX, 64);
                    triangles[1].texCoords = sf::Vector2f(80 + offsetX, 64);
                    triangles[2].texCoords = sf::Vector2f(80 + offsetX, 80);
                    triangles[3].texCoords = sf::Vector2f(64 + offsetX, 64);
                    triangles[4].texCoords = sf::Vector2f(80 + offsetX, 80);
                    triangles[5].texCoords = sf::Vector2f(64 + offsetX, 80);
                }
                else if (temp[i] == 'L') {
                    sf::RectangleShape lightRect;
                    lightRect.setSize({calcX(256, vm), calcY(256, vm)});
                    lightRect.setOrigin({calcX(128, vm), calcY(128, vm)});
                    lightRect.setFillColor(sf::Color::Yellow);
                    lightRect.setPosition({x, y});
                    lightRects.emplace_back(lightRect);
                    addTile("wall", tile, x, y);
                }
                else if (temp[i] == 'T') {
                    sf::RectangleShape lightRect;
                    lightRect.setSize({LIGHT_RADIUS * 2.f, LIGHT_RADIUS * 2.f});
                    lightRect.setOrigin({LIGHT_RADIUS, LIGHT_RADIUS});
                    lightRect.setFillColor(sf::Color::White);
                    lightRect.setPosition({x, y});
                    lightRects.emplace_back(lightRect);
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
    return vertexArray.getBounds().size;
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

const std::vector<sf::FloatRect> &TileMap::getTilesGlobalBounds() const
{
    return tilesGlobalBounds;
}

void TileMap::addTile(const std::string &name, const sf::Vector2f &size, float x, float y)
{
    tiles.emplace_back(std::make_unique<Tile>(name, size, x, y));
    tilesGlobalBounds.emplace_back(sf::FloatRect(sf::Vector2f(x, y), size));
}

void TileMap::drawMap(sf::RenderTarget &target, sf::RenderStates &states) const
{
    target.draw(background, states);
}

void TileMap::drawObstacles(sf::RenderTarget &target, sf::RenderStates &states) const
{
    states.texture = &tilesTexture;
    target.draw(this->vertexArray, states);
}

void TileMap::drawLights(sf::RenderTarget &target)
{
    sf::RenderStates states;
    states.blendMode = sf::BlendAdd;

    for (const auto &light : lightRects) {
        const sf::Vector2i centerPx =
            target.mapCoordsToPixel({light.getPosition().x + calcX(32, vm), light.getPosition().y + calcY(32, vm)});
        shader.setUniform("lightCenter",
                          sf::Glsl::Vec2(static_cast<float>(centerPx.x), static_cast<float>(centerPx.y)));
        shader.setUniform("lightRadius", light.getSize().x * 0.5f);
        states.shader = &shader;
        target.draw(light, states);
    }
}
