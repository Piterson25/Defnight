#include "Trap.hpp"

Trap::Trap(sf::VideoMode &t_vm, const sf::Vector2f &t_position, TrapStats t_stats)
    : Entity("TRAP", t_vm, t_position.x, t_position.y), shadow(shadow_texture), type(t_stats.type),
      releaseTimeLimit(t_stats.releaseTimeLimit), attack(t_stats.attack), attackTimeLimit(t_stats.attackTimeLimit)
{
    this->texture = sf::Texture("assets/textures/traps.png");
    this->sprite = sf::Sprite(this->texture);
    switch (type) {
        case TrapType::NORMAL:
            this->sprite.setTextureRect(sf::IntRect({0, 0}, {16, 16}));
            break;
        case TrapType::DAMAGE_DEALING:
            this->sprite.setTextureRect(sf::IntRect({0, 16}, {16, 16}));
            break;
        case TrapType::SCENTED:
            this->sprite.setTextureRect(sf::IntRect({0, 32}, {16, 16}));
            break;
    };
    this->sprite.setScale({calcScale(4, vm), calcScale(4, vm)});
    this->sprite.setPosition(t_position);

    this->shadow_texture = sf::Texture("assets/textures/entity_shadow.png");
    this->shadow = sf::Sprite(this->shadow_texture);
    this->shadow.setTextureRect(sf::IntRect({0, 0}, {8, 4}));
    this->shadow.setScale({calcScale(2, vm), calcScale(2, vm)});
    this->shadow.setPosition(
        {this->getDownCenter().x - (this->shadow.getTextureRect().size.x * 0.5f) * this->shadow.getScale().x,
         this->getDownCenter().y});
    this->shadow.setColor(sf::Color(255, 255, 255, 0));

    this->state = TrapState::OPENED;
    this->closingCountdown = 0.f;
    this->releaseCooldown = 0.f;
    this->attackCooldown = 0.f;

    this->trappedMonster = nullptr;
}

Trap::~Trap()
{
    delete this->trappedMonster;
}

const TrapType Trap::getType() const
{
    return this->type;
}

const uint32_t Trap::getAttack() const
{
    return this->attack;
}

const bool Trap::isBroken() const
{
    return this->state == TrapState::BROKEN;
}

const bool Trap::isDamageDealing() const
{
    return this->type == TrapType::DAMAGE_DEALING;
}

void Trap::checkSteppedOn(Monster *monster, SoundEngine &soundEngine, float dt)
{
    if (!monster->isPunched() && vectorDistance(this->getCenter(), monster->getCenter()) <= calcX(32, this->vm) &&
        this->state == TrapState::OPENED) {
        this->state = TrapState::CLOSING;
        soundEngine.addSound("trap_activation");
        monster->setFrozen(true);
        monster->setPosition(sprite.getPosition().x, sprite.getPosition().y);
        this->trappedMonster = monster;
    }
}

void Trap::dealDamage(Player &player, PlayerGUI &playerGUI, FloatingTextSystem &floatingTextSystem, float dt)
{
    if (this->trappedMonster != nullptr) {
        attackCooldown += dt;
        if (this->attackCooldown > this->attackTimeLimit) {
            this->attackCooldown = 0.f;
            this->trappedMonster->punch();
            player.setDamageDealt(player.getDamageDealt() + attack);
            floatingTextSystem.addFloatingText(gui::RED, std::to_string(-attack), calcChar(16, vm),
                                               this->trappedMonster->getPosition().x + calcX(32, vm),
                                               this->trappedMonster->getPosition().y + calcY(32, vm), false);
            if (static_cast<int>(this->trappedMonster->getHP() - attack) < 0) {
                this->trappedMonster->setHP(0);
                this->state = TrapState::BROKEN;
                this->trappedMonster = nullptr;
            }
            else {
                this->trappedMonster->setHP(this->trappedMonster->getHP() - attack);
            }
        }
    }
}

void Trap::update(float dt)
{
    switch (this->state) {
        case TrapState::CLOSING:
            this->closingCountdown += dt;
            if (this->closingCountdown > 0.25f) {
                this->closingCountdown = 0.f;
                frame += 1;
                if (frame == 3) {
                    this->state = TrapState::CLOSED;
                }
                switch (type) {
                    case TrapType::NORMAL:
                        this->sprite.setTextureRect(sf::IntRect({static_cast<int>(frame) * 16, 0}, {16, 16}));
                        break;
                    case TrapType::DAMAGE_DEALING:
                        this->sprite.setTextureRect(sf::IntRect({static_cast<int>(frame) * 16, 16}, {16, 16}));
                        break;
                    case TrapType::SCENTED:
                        this->sprite.setTextureRect(sf::IntRect({static_cast<int>(frame) * 16, 32}, {16, 16}));
                        break;
                };
            }
            break;
        case TrapState::CLOSED:
            this->releaseCooldown += dt;
            if (this->releaseCooldown > this->releaseTimeLimit) {
                this->state = TrapState::BROKEN;
                if (trappedMonster) {
                    trappedMonster->setFrozen(false);
                }
                trappedMonster = nullptr;
            }
            break;
        default:
            break;
    };
}

void Trap::drawShadow(sf::RenderTarget &target)
{
    target.draw(this->shadow);
}
