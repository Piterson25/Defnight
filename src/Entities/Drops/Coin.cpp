#include "Coin.hpp"

Coin::Coin(const std::string &t_name, sf::VideoMode &t_vm, float t_x, float t_y,
           uint32_t t_worth, bool t_vanishing)
    : Drop(t_name, t_vm, t_x, t_y, t_worth, t_vanishing)
{
    this->sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
}

Coin::~Coin() = default;

void Coin::spin(float dt)
{
    this->spinCooldown += dt;
    if (this->spinCooldown > 0.166f) {
        const int w = this->sprite.getTextureRect().left;

        if (w == 80) {
            this->sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
        }
        else {
            this->sprite.setTextureRect(sf::IntRect(w + 16, 0, 16, 16));
        }

        this->spinCooldown = 0.f;
    }
}

const bool Coin::isPickedByPlayer(Player &player, PlayerGUI &playerGUI,
                                  FloatingTextSystem &floatingTextSystem,
                                  SoundEngine &soundEngine, float dt)
{
    const float distance = vectorDistance(
        sf::Vector2f(this->sprite.getPosition().x + calcX(8, vm),
                     this->sprite.getPosition().y + calcY(8, vm)),
        sf::Vector2f(player.getPosition().x + calcX(32, vm),
                     player.getPosition().y + calcY(32, vm)));

    if (distance <= player.getReach() * calcX(64, vm)) {
        this->move(player.getPosition().x, player.getPosition().y, dt);
    }
    if (distance <= player.getReach() * calcX(16, vm)) {
        player.setGold(player.getGold() + this->worth);
        playerGUI.update_Gold();
        floatingTextSystem.addFloatingText(
            "gold", "+" + std::to_string(this->worth), calcChar(16, vm),
            this->sprite.getPosition().x - calcX(16, vm),
            this->sprite.getPosition().y, false);
        soundEngine.addSound("coin");

        return true;
    }
    return false;
}
