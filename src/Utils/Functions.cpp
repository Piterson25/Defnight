#include "Functions.hpp"

const float calcX(float x, const sf::VideoMode &vm)
{
    return std::floor(static_cast<float>(x * (vm.width / 1280.f)));
}

const float calcY(float y, const sf::VideoMode &vm)
{
    return std::floor(static_cast<float>(y * (vm.height / 720.f)));
}

const float calcScale(float x, const sf::VideoMode &vm)
{
    return static_cast<float>(x * (vm.width / 1280.f));
}

const unsigned calcChar(unsigned size, const sf::VideoMode &vm)
{
    return static_cast<unsigned>(
        std::floor(static_cast<float>(size * (vm.height / 720.f))));
}

const float getAngle(float x1, float y1, float x2, float y2)
{
    sf::Vector2f p0(x1, y1);
    sf::Vector2f p1(x2, y2);
    sf::Vector2f q = p1 - p0;

    float q_length = sqrtf(q.x * q.x + q.y * q.y);

    q.x /= q_length;
    q.y /= q_length;

    float scalar_product = q.y;

    float angle = acos(scalar_product) * 180.0f / 3.1415f;

    if (x2 - x1 < 0.0f) {
        return angle;
    }
    return -angle;
}

const float vectorDistance(float x1, float y1, float x2, float y2)
{
    const float dx = x2 - x1;
    const float dy = y2 - y1;

    return sqrtf(dx * dx + dy * dy);
}

const float vectorDistance(const sf::Vector2f &vec1, const sf::Vector2f &vec2)
{
    const sf::Vector2f dVec = vec2 - vec1;

    return sqrtf(dVec.x * dVec.x + dVec.y * dVec.y);
}

void center(sf::Text &text, float x)
{
    text.setPosition(float(int(x - (text.getLocalBounds().width) / 2)),
                     text.getPosition().y);
}

void center(sf::Sprite &sprite, float x)
{
    sprite.setPosition(float(int(x - (sprite.getLocalBounds().width) / 2)),
                       sprite.getPosition().y);
}

std::string toLowerCase(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), tolower);
    return str;
}

const bool isPointVisible(const sf::FloatRect &rect, const sf::Vector2f &a_p1,
                          const sf::Vector2f &a_p2)
{
    float minX = std::min(a_p1.x, a_p2.x);
    float maxX = std::max(a_p1.x, a_p2.x);

    if (maxX > rect.left + rect.width) {
        maxX = rect.left + rect.width;
    }

    if (minX < rect.left) {
        minX = rect.left;
    }

    if (minX > maxX) {
        return false;
    }

    float minY = a_p1.y;
    float maxY = a_p2.y;

    float dx = a_p2.x - a_p1.x;
    if (std::abs(dx) > 0.f) {
        auto k = (a_p2.y - a_p1.y) / dx;
        auto b = a_p1.y - k * a_p1.x;
        minY = k * minX + b;
        maxY = k * maxX + b;
    }

    if (minY > maxY) {
        std::swap(minY, maxY);
    }

    if (maxY > rect.top + rect.height) {
        maxY = rect.top + rect.height;
    }

    if (minY < rect.top) {
        minY = rect.top;
    }

    if (minY > maxY) {
        return false;
    }
    return true;
}
