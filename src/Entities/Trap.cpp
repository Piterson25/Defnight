#include "Trap.hpp"

Trap::Trap(sf::VideoMode &t_vm, const sf::Vector2f &t_position, uint32_t t_attack)
    : Entity("TRAP", t_vm, t_position.x, t_position.y), shadow(shadow_texture)
{
    this->texture = sf::Texture("assets/textures/trap.png");
    this->sprite = sf::Sprite(this->texture);
    this->sprite.setTextureRect(sf::IntRect({0, 0}, {16, 16}));
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

    this->attack = t_attack;
    this->state = TrapState::OPENED;
    this->closingCountdown = 0.f;
}

Trap::~Trap() = default;

const TrapState Trap::getState() const
{
    return this->state;
}

const uint32_t Trap::getAttack() const
{
    return this->attack;
}

void Trap::checkSteppedOn(Monster &monster, PlayerGUI &playerGUI, FloatingTextSystem &floatingTextSystem,
                          SoundEngine &soundEngine, float dt)
{
    if (!monster.isPunched() && vectorDistance(this->getCenter(), monster.getCenter()) <= calcX(32, this->vm) &&
        this->state == TrapState::OPENED) {
        this->state = TrapState::CLOSING;
        soundEngine.addSound("trap_activation");
        if (static_cast<int>(monster.getHP() - attack) < 0) {
            monster.setHP(0);
        }
        else {
            monster.setHP(monster.getHP() - attack);
        }
        monster.punch();
        floatingTextSystem.addFloatingText(gui::RED, std::to_string(-attack), calcChar(16, vm),
                                           monster.getPosition().x + calcX(32, vm),
                                           monster.getPosition().y + calcY(32, vm), false);
    }
}

void Trap::update(float dt)
{
    if (this->state == TrapState::CLOSING) {
        this->closingCountdown += dt;
        if (this->closingCountdown > 0.25f) {
            frame += 1;
            if (frame == 4) {
                this->state = TrapState::CLOSED;
            }
            this->sprite.setTextureRect(sf::IntRect({static_cast<int>(frame) * 16, 0}, {16, 16}));
            this->closingCountdown = 0.f;
        }
    }
}

void Trap::drawShadow(sf::RenderTarget &target)
{
    target.draw(this->shadow);
}
