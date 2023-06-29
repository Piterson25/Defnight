#include "Player.hpp"

Player::Player(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
               float t_y)
    : Entity(t_name, t_vm, t_x, t_y)
{
    this->texture.loadFromFile("assets/textures/player/" + t_name + ".png");
    this->sprite.setTexture(this->texture);
    this->entitySize =
        static_cast<uint32_t>(this->sprite.getGlobalBounds().width / 128);
    this->sprite.setTextureRect(sf::IntRect(0, 32, 16, 16));
    this->sprite.setScale(calcScale(4, vm), calcScale(4, vm));
    this->sprite.setColor(sf::Color(255, 255, 255, 0));

    this->shadow_texture.loadFromFile("assets/textures/entity_shadow.png");
    this->shadow.setTexture(this->shadow_texture);
    this->shadow.setTextureRect(sf::IntRect(0, 0, 8, 4));
    this->shadow.setScale(calcScale(4, vm), calcScale(4, vm));
    this->shadow.setPosition(this->getDownCenter().x -
                                 this->shadow.getTextureRect().width / 2 *
                                     this->shadow.getScale().x,
                             this->getDownCenter().y);
    this->shadow.setColor(sf::Color(255, 255, 255, 0));

    this->ability_texture.loadFromFile("assets/textures/abilities.png");
    this->ability.setTexture(this->ability_texture);
    this->ability.setScale(calcScale(4, vm), calcScale(4, vm));
    this->ability.setTextureRect(sf::IntRect(0, 0, 16, 16));
    this->ability.setPosition(this->sprite.getPosition().x,
                              this->sprite.getPosition().y + calcY(52, vm));
    this->ability.setColor(sf::Color(255, 255, 255, 0));

    this->attack = 1;
    this->attackSpeed = 1;
    this->maxHP = 10;
    this->HP = 10;
    this->reg = 1;
    this->speed = 1;
    this->reach = 1;
    this->armor = 1;

    this->gold = 0;
    this->armor = 0;
    this->reg = 0;
    this->level = 1;
    this->maxXP = 40;
    this->lastMaxXP = 0;
    this->sprint = 100.f;
    this->maxSprint = 100;
    this->criticalChance = 0;
    this->kills = 0;
    this->upgraded = false;
    this->increasedArmor = false;
    this->regenerating = false;
    this->regCooldown = 0.f;
    this->leveling = false;
    this->sprinting = false;
    this->spawned = false;
    this->spawnCountdown = 0.f;
    this->abilityActive = false;
    this->abilityCooldown = 0.f;
    this->abilityTime = 0.f;
    this->abilityMaxTime = 0.f;
    this->soundPlayed = false;
}

Player::~Player() = default;

const uint32_t Player::getGold() const
{
    return this->gold;
}

const uint32_t Player::getArmor() const
{
    return this->armor;
}

const uint32_t Player::getReg() const
{
    return this->reg;
}

const uint32_t Player::getLevel() const
{
    return this->level;
}

const uint32_t Player::getMaxXP() const
{
    return this->maxXP;
}

const uint32_t Player::getLastMaxXP() const
{
    return this->lastMaxXP;
}

const float Player::getSprint() const
{
    return this->sprint;
}

const uint32_t Player::getMaxSprint() const
{
    return this->maxSprint;
}

const uint32_t Player::getCriticalChance() const
{
    return this->criticalChance;
}

const uint32_t Player::getKills() const
{
    return this->kills;
}

const bool Player::isUpgraded() const
{
    return this->upgraded;
}

const bool Player::isIncreasedArmor() const
{
    return this->increasedArmor;
}

const bool Player::isRegenerating() const
{
    return this->regenerating;
}

const bool Player::isLeveling() const
{
    return this->leveling;
}

const bool Player::isSprinting() const
{
    return this->sprinting;
}

const bool Player::hasSpawned() const
{
    return this->spawned;
}

const bool Player::isAbilityActive() const
{
    return this->abilityActive;
}

const float Player::getAbilityCooldown() const
{
    return this->abilityCooldown;
}

const float Player::getAbilityTime() const
{
    return this->abilityTime;
}

const float Player::getAbilityMaxTime() const
{
    return this->abilityMaxTime;
}

const bool Player::isSoundPlayed() const
{
    return this->soundPlayed;
}

void Player::setGold(uint32_t t_gold)
{
    this->gold = t_gold;
}

void Player::setArmor(uint32_t t_armor)
{
    this->armor = t_armor;
}

void Player::setReg(uint32_t t_reg)
{
    this->reg = t_reg;
}

void Player::setSprint(float t_sprint)
{
    this->sprint = t_sprint;
}

void Player::setMaxSprint(uint32_t t_maxSprint)
{
    this->maxSprint = t_maxSprint;
}

void Player::setCriticalChance(uint32_t t_criticalChance)
{
    this->criticalChance = t_criticalChance;
}

void Player::setKills(uint32_t t_kills)
{
    this->kills = t_kills;
}

void Player::setUpgraded(bool t_upgraded)
{
    this->upgraded = t_upgraded;
}

void Player::setRegenerating(bool t_regenerating)
{
    this->regenerating = t_regenerating;
}

void Player::setLeveling(bool t_leveling)
{
    this->leveling = t_leveling;
}

void Player::setSprinting(bool t_isSprinting)
{
    this->sprinting = t_isSprinting;
}

void Player::setAbilityActive(bool t_abilityActive)
{
    this->abilityActive = t_abilityActive;
}

void Player::setAbilityCooldown(float t_abilityCooldown)
{
    this->abilityCooldown = t_abilityCooldown;
}

void Player::setAbilityTime(float t_abilityTime)
{
    this->abilityTime = t_abilityTime;
}

void Player::setAbilityMaxTime(float t_abilityMaxTime)
{
    this->abilityMaxTime = t_abilityMaxTime;
}

void Player::setPlayedSound(bool t_soundPlayed)
{
    this->soundPlayed = t_soundPlayed;
}

void Player::controls(const std::unordered_map<std::string, int> &keybinds,
                      float dt)
{
    this->velocity = sf::Vector2f(0.f, 0.f);

    const float vel = ((this->speed * 0.2f + 0.8f) * 2.f *
                       this->sprite.getGlobalBounds().width) *
                      dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("MOVE_UP")))) {
        this->velocity.y += -(vel);
    }
    if (sf::Keyboard::isKeyPressed(
            sf::Keyboard::Key(keybinds.at("MOVE_DOWN")))) {
        this->velocity.y += vel;
    }
    if (sf::Keyboard::isKeyPressed(
            sf::Keyboard::Key(keybinds.at("MOVE_LEFT")))) {
        this->velocity.x += -(vel);
    }
    if (sf::Keyboard::isKeyPressed(
            sf::Keyboard::Key(keybinds.at("MOVE_RIGHT")))) {
        this->velocity.x += vel;
    }

    if (this->name == "scout" && this->abilityActive) {
        this->velocity *= 1.3f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("SPRINT"))) &&
        this->velocity != sf::Vector2f(0.f, 0.f)) {
        if (this->sprint > 0) {
            this->velocity *= 1.5f;

            this->sprinting = true;
            const float minusSprint = dt * 10.f;

            if (static_cast<uint32_t>(this->sprint - minusSprint) <
                static_cast<uint32_t>(this->sprint)) {
                sf::RectangleShape particle;
                particle.setPosition(this->getDownCenter());
                particle.setFillColor(sf::Color::White);
                particle.setSize(sf::Vector2f(calcX(8, vm), calcY(8, vm)));
                particle.rotate(Random::Float() * 360.f - 90.f);
                this->particles.push_back(particle);
            }

            if (this->sprint - minusSprint > 0.f) {
                this->sprint -= minusSprint;
            }
            else {
                this->sprint = 0.f;
            }
        }
        else {
            this->sprinting = false;
        }
    }
    else {
        this->sprinting = false;
        const float plusSprint = dt * this->maxSprint / 40.f;
        if (this->sprint + plusSprint < this->maxSprint) {
            this->sprint += plusSprint;
        }
        else {
            this->sprint = static_cast<float>(this->maxSprint);
        }
    }
}

void Player::whooshSound(SoundEngine &soundEngine)
{
    if (this->isAttacking() && this->frame == 80 && !this->soundPlayed) {
        soundEngine.addSound("whoosh");
        this->soundPlayed = true;
    }
    if (this->soundPlayed && this->frame != 80) {
        this->soundPlayed = false;
    }
}

const bool Player::hasLeveledUp(uint32_t monsterXP)
{
    this->setLeveling(true);
    this->setXP(this->getXP() + monsterXP);

    bool addedlevel = false;

    while (this->XP >= this->maxXP) {
        this->level++;
        this->lastMaxXP = this->maxXP;
        this->maxXP += this->maxXP + this->level * 10;
        addedlevel = true;
    }
    if (addedlevel) {
        return true;
    }
    return false;
}

const bool Player::isHPRegenerating(float dt)
{
    if (this->regCooldown < 1.f && this->HP < this->maxHP) {
        this->regCooldown += ((this->reg * 0.2f + 0.8f) / 4.f) * dt;
    }

    if (this->regCooldown >= 1.f) {
        this->regCooldown = 0.f;
        if (this->HP < this->maxHP) {
            this->HP++;
            return true;
        }
    }
    return false;
}

const bool Player::isAbilityActivated()
{
    if (this->level >= 5 && sf::Mouse::isButtonPressed(sf::Mouse::Right) &&
        this->abilityCooldown == this->abilityMaxTime) {
        this->abilityActive = true;
        this->abilityCooldown = 0.f;

        if (this->name == "knight") {
            this->ability.setTextureRect(sf::IntRect(0, 0, 16, 16));
        }
        else if (this->name == "scout") {
            this->ability.setTextureRect(sf::IntRect(16, 0, 16, 16));
        }
        else if (this->name == "crusader") {
            this->ability.setTextureRect(sf::IntRect(32, 0, 16, 16));
        }
        else if (this->name == "paladin") {
            this->ability.setTextureRect(sf::IntRect(48, 0, 16, 16));
        }
        this->ability.setColor(sf::Color(255, 255, 255, 128));
        return true;
    }
    return false;
}

void Player::abilityCounter(float dt)
{
    if (this->abilityMaxTime > 0.f && this->level >= 5) {
        if (this->abilityCooldown < this->abilityMaxTime) {
            this->abilityCooldown += dt;
        }

        if (this->abilityCooldown >= this->abilityTime && this->abilityActive) {
            this->abilityActive = false;
            endAbility();
        }
        else if (this->abilityCooldown > this->abilityMaxTime) {
            this->abilityCooldown = this->abilityMaxTime;
        }
    }
}

void Player::endAbility()
{
    if (this->name == "knight") {
        this->armor -= 5;
        this->increasedArmor = false;
    }
    else if (this->name == "crusader") {
        this->armor -= 5;
        this->attack -= 5;
        this->increasedArmor = false;
    }
    else if (this->name == "paladin") {
        this->armor -= 5;
        this->reg -= 5;
        this->increasedArmor = false;
    }
}

void Player::doAbility(SoundEngine &soundEngine)
{
    if (this->name == "ninja") {
        soundEngine.addSound("shuriken");
    }
    else if (this->name == "knight") {
        this->armor += 5;
        this->increasedArmor = true;
        soundEngine.addSound("ability");
    }
    else if (this->name == "scout") {
        soundEngine.addSound("ability");
    }
    else if (this->name == "master") {
        soundEngine.addSound("shuriken");
    }
    else if (this->name == "bomber") {
        soundEngine.addSound("shuriken");
    }
    else if (this->name == "crusader") {
        this->increasedArmor = true;
        this->armor += 5;
        this->attack += 5;
        soundEngine.addSound("ability");
    }
    else if (this->name == "paladin") {
        this->increasedArmor = true;
        this->armor += 5;
        this->reg += 5;
        soundEngine.addSound("ability");
    }
}

void Player::spawn(float dt)
{
    if (this->spawnCountdown < 0.5f) {
        this->sprite.setColor(
            sf::Color(255, 255, 255,
                      static_cast<sf::Uint8>(this->spawnCountdown * 510.f)));
        this->shadow.setColor(
            sf::Color(255, 255, 255,
                      static_cast<sf::Uint8>(this->spawnCountdown * 510.f)));
        this->spawnCountdown += dt;
    }
    if (this->spawnCountdown >= 0.5f) {
        this->spawned = true;
        this->sprite.setColor(sf::Color::White);
        this->shadow.setColor(sf::Color::White);
    }
}

void Player::updateSprint(float dt)
{
    for (auto particle = this->particles.begin();
         particle != this->particles.end();) {
        if (particle->getFillColor().a <= 0) {
            particle = this->particles.erase(particle);
        }
        else {
            particle->setFillColor(
                sf::Color(255, 255, 255, particle->getFillColor().a - 1));
            particle++;
        }
    }
}

void Player::update(float dt)
{
    this->shadow.setPosition(this->getDownCenter().x -
                                 this->shadow.getTextureRect().width / 2 *
                                     this->shadow.getScale().x,
                             this->getDownCenter().y);
    this->ability.setPosition(this->sprite.getPosition());
}

void Player::draw(sf::RenderTarget &target)
{
    target.draw(this->sprite);
    if (this->abilityActive &&
        (this->increasedArmor || this->name == "scout")) {
        target.draw(this->ability);
    }
}

void Player::drawShadow(sf::RenderTarget &target)
{
    for (auto &particle : this->particles) {
        target.draw(particle);
    }
    target.draw(this->shadow);
}
