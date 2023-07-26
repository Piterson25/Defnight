#include "Drop.hpp"
#include "Functions.hpp"

Drop::Drop(const std::string &t_name, sf::VideoMode &t_vm, float t_x, float t_y,
           uint32_t t_worth, bool t_vanishing)
    : Entity(t_name, t_vm, t_x, t_y)
{

    this->texture.loadFromFile("assets/textures/drop.png");
    this->sprite.setTexture(this->texture);
    this->sprite.setScale(calcScale(2, vm), calcScale(2, vm));
    this->sprite.setPosition(t_x, t_y);
    this->sprite.setColor(sf::Color::Transparent);

    this->entitySize =
        static_cast<uint32_t>(this->sprite.getGlobalBounds().width / 96);

    this->shadow_texture.loadFromFile("assets/textures/entity_shadow.png");
    this->shadow.setTexture(this->shadow_texture);
    this->shadow.setTextureRect(sf::IntRect(0, 0, 8, 4));
    this->shadow.setScale(calcScale(2, vm), calcScale(2, vm));
    this->shadow.setPosition(this->getDownCenter().x -
                                 this->shadow.getTextureRect().width / 2 *
                                     this->shadow.getScale().x,
                             this->getDownCenter().y);
    this->shadow.setColor(sf::Color(255, 255, 255, 0));

    this->spinCooldown = 0.f;
    this->worth = t_worth;
    this->angle = 0.f;

    this->vanishingCountdown = 0.f;
    this->vanishing = t_vanishing;
    this->spawned = false;
    this->spawnCountdown = 0.f;
}

Drop::~Drop() = default;

const uint32_t Drop::getWorth() const
{
    return this->worth;
}

const bool Drop::hasSpawned() const
{
    return this->spawned;
}

const bool Drop::hasVanished() const
{
    return this->vanishingCountdown >= 10.f;
}

void Drop::spawn(float dt)
{
    if (this->spawnCountdown < 0.25f) {
        this->sprite.setColor(
            sf::Color(255, 255, 255,
                      static_cast<sf::Uint8>(this->spawnCountdown * 1020.f)));
        this->shadow.setColor(
            sf::Color(255, 255, 255,
                      static_cast<sf::Uint8>(this->spawnCountdown * 510.f)));
        this->spawnCountdown += dt;
    }
    if (this->spawnCountdown >= 0.25) {
        this->spawned = true;
        this->sprite.setColor(sf::Color::White);
        this->shadow.setColor(sf::Color::White);
    }
}

void Drop::spin(float dt)
{
}

void Drop::move(float posX, float posY, float dt)
{
    this->angle = getAngle(this->sprite.getPosition().x + calcX(8, vm),
                           this->sprite.getPosition().y + calcY(8, vm),
                           posX + calcX(32, vm), posY + calcY(32, vm)) +
                  90.f;

    this->velocity.x = this->sprite.getGlobalBounds().width * 16.f *
                       cos((3.1415f / 180) * this->angle) * dt;
    this->velocity.y = this->sprite.getGlobalBounds().width * 16.f *
                       sin((3.1415f / 180) * this->angle) * dt;
    this->sprite.move(this->velocity);
}

const bool Drop::isPickedByPlayer(Player &player, PlayerGUI &playerGUI,
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
        if (this->name == "coin") {
            player.setGold(player.getGold() + this->worth);
            playerGUI.update_Gold();
            floatingTextSystem.addFloatingText(
                "gold", "+" + std::to_string(this->worth), calcChar(16, vm),
                this->sprite.getPosition().x - calcX(16, vm),
                this->sprite.getPosition().y, false);
            soundEngine.addSound("coin");
        }
        else if (player.getHP() < player.getMaxHP()) {
            player.setHP(player.getHP() + 1);
            player.setRegenerating(true);
            playerGUI.updateHP();
            floatingTextSystem.addFloatingText(
                "red", "+" + std::to_string(this->worth), calcChar(16, vm),
                this->sprite.getPosition().x - calcX(16, vm),
                this->sprite.getPosition().y - calcY(16, vm), false);
            soundEngine.addSound("heart");
        }

        return true;
    }
    return false;
}

void Drop::update(float dt)
{
    this->shadow.setPosition(this->getDownCenter().x -
                                 this->shadow.getTextureRect().width / 2 *
                                     this->shadow.getScale().x,
                             this->getDownCenter().y);
    if (this->spawned) {
        this->spin(dt);
    }
    else {
        this->spawn(dt);
    }
    if (this->vanishing) {
        this->vanishingCountdown += dt;
    }
}

void Drop::drawShadow(sf::RenderTarget &target)
{
    target.draw(this->shadow);
}
