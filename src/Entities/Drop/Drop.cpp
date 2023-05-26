#include "Drop.hpp"
#include "Functions.hpp"

Drop::Drop(sf::VideoMode &vm, const std::string &name, float posX, float posY,
           uint32_t worth, bool vanishing)
    : name(name), worth(worth), vanishing(vanishing), vm(vm)
{
    this->texture.loadFromFile("assets/textures/drop.png");
    this->sprite.setTexture(this->texture);
    this->sprite.setPosition(posX, posY);
    this->sprite.setScale(calcScale(2, vm), calcScale(2, vm));
    this->sprite.setColor(sf::Color::Transparent);

    if (this->name == "coin") {
        this->sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
    }
    else {
        this->sprite.setTextureRect(sf::IntRect(0, 16, 16, 16));
    }

    this->spinCooldown = 0.f;
    this->velocity = sf::Vector2f(0.f, 0.f);
    this->angle = 0.f;
    this->vanishingCountdown = 0.f;

    this->spawned = false;
    this->spawnCountdown = 0.f;
}

Drop::~Drop()
{
}

const std::string Drop::getName() const
{
    return this->name;
}

const uint32_t Drop::getWorth() const
{
    return this->worth;
}

const sf::Vector2f Drop::getPosition() const
{
    return this->sprite.getPosition();
}

const bool Drop::getSpawned() const
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
        this->spawnCountdown += dt;
    }
    if (this->spawnCountdown >= 0.25) {
        this->spawned = true;
        this->sprite.setColor(sf::Color::White);
    }
}

void Drop::spin(float dt)
{
    this->spinCooldown += dt;
    if (this->spinCooldown > 0.166f) {
        const int w = this->sprite.getTextureRect().left;
        if (this->name == "coin") {
            if (w == 80) {
                this->sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
            }
            else {
                this->sprite.setTextureRect(sf::IntRect(w + 16, 0, 16, 16));
            }
        }
        else {
            if (w == 80) {
                this->sprite.setTextureRect(sf::IntRect(0, 16, 16, 16));
            }
            else {
                this->sprite.setTextureRect(sf::IntRect(w + 16, 16, 16, 16));
            }
        }

        this->spinCooldown = 0.f;
    }
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

const bool Drop::playerPick(Player &player, PlayerGUI &playerGUI,
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
                "+" + std::to_string(this->worth), calcChar(16, vm),
                this->sprite.getPosition().x - calcX(16, vm),
                this->sprite.getPosition().y, sf::Color(255, 246, 76), false);
            soundEngine.addSound("coin");
        }
        else if (player.getHP() < player.getMaxHP()) {
            player.setHP(player.getHP() + 1);
            player.setRegenerating(true);
            playerGUI.update_HP();
            floatingTextSystem.addFloatingText(
                "+" + std::to_string(this->worth), calcChar(16, vm),
                this->sprite.getPosition().x - calcX(16, vm),
                this->sprite.getPosition().y - calcY(16, vm),
                sf::Color(182, 60, 53), false);
            soundEngine.addSound("heart");
        }

        return true;
    }
    return false;
}

void Drop::update(float dt)
{
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

void Drop::draw(sf::RenderTarget &target)
{
    target.draw(this->sprite);
}
