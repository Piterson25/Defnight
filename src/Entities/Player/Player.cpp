#include "Player.hpp"

Player::Player(const std::string &t_name, sf::VideoMode &t_vm, float t_x,
               float t_y)
    : Entity(t_name, t_vm, t_x, t_y), particleCooldown(0.f)
{
    this->texture.loadFromFile("assets/textures/player/" + toLowerCase(t_name) +
                               ".png");
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
    this->ability.setTextureRect(sf::IntRect(0, 0, 0, 0));
    this->ability.setPosition(this->sprite.getPosition().x,
                              this->sprite.getPosition().y + calcY(52, vm));
    this->ability.setColor(sf::Color(255, 255, 255, 0));

    this->damageDealt = 0;
    this->damageTaken = 0;
    this->boughtItems = 0;
    this->gold = 0;
    this->armor = 0;
    this->reg = 0;
    this->level = 1;
    this->maxXP = 40;
    this->pendingXP = 0;
    this->maxHP = 10;
    this->HP = 10;
    this->sprint = 100.f;
    this->maxSprint = 100;
    this->attack = 1;
    this->attackSpeed = 1;
    this->speed = 1;
    this->criticalChance = 0;
    this->projectileAttack = 0;
    this->projectilePiercing = 1;
    this->projectileArea = 1;
    this->kills = 0;
    this->reach = 1;
    this->upgraded = false;
    this->increasedArmor = 0;
    this->increasedReg = 0;
    this->increasedAttack = 0;
    this->increasedGold = 0;
    this->increasedTargets = 0;
    this->goldReward = 0;
    this->attackLimit = 1;
    this->timeSlowdown = 0.f;
    this->regenerating = false;
    this->regCooldown = 0.f;
    this->leveling = false;
    this->sprinting = false;
    this->spawned = false;
    this->spawnCountdown = 0.f;
    this->soundPlayed = false;

    this->abilityComponent = std::make_unique<AbilityComponent>(this->vm);
}

Player::~Player() = default;

const uint32_t Player::getBoughtItems() const
{
    return this->boughtItems;
}

const uint32_t Player::getDamageDealt() const
{
    return this->damageDealt;
}

const uint32_t Player::getDamageTaken() const
{
    return this->damageTaken;
}

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

const uint32_t Player::getPendingXP() const
{
    return this->pendingXP;
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

const uint32_t Player::getProjectileAttack() const
{
    return this->projectileAttack + this->abilityComponent->getAttackIncrease();
}

const uint32_t Player::getProjectilePiercing() const
{
    return this->projectilePiercing;
}

const uint32_t Player::getProjectileArea() const
{
    return this->projectileArea;
}

const uint32_t Player::getKills() const
{
    return this->kills;
}

const bool Player::isUpgraded() const
{
    return this->upgraded;
}

const uint32_t Player::getIncreasedArmor() const
{
    return this->increasedArmor;
}

const uint32_t Player::getIncreasedReg() const
{
    return this->increasedReg;
}

const uint32_t Player::getIncreasedAttack() const
{
    return this->increasedAttack;
}

const uint32_t Player::getIncreasedGold() const
{
    return this->increasedGold;
}

const uint32_t Player::getIncreasedTargets() const
{
    return this->increasedTargets;
}

const uint32_t Player::getGoldReward() const
{
    return this->goldReward;
}

const uint32_t Player::getAttackLimit() const
{
    return this->attackLimit;
}

const float Player::getTimeSlowdown() const
{
    return this->timeSlowdown;
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

const float Player::getAbilityMaxTime() const
{
    return this->abilityComponent->getAbilityMaxTime();
}

const float Player::getAbilityMaxTimeModifier() const
{
    return this->abilityComponent->getAbilityMaxTimeModifier();
}

const uint32_t Player::getAttackIncrease() const
{
    return this->abilityComponent->getAttackIncrease();
}

const bool Player::isParticleCooldown(float dt)
{
    this->particleCooldown += dt;

    if (this->particleCooldown > 0.1f) {
        this->particleCooldown = 0.f;
        return true;
    }
    return false;
}

void Player::setDamageDealt(uint32_t t_damageDealt)
{
    this->damageDealt = t_damageDealt;
}

void Player::setDamageTaken(uint32_t t_damageTaken)
{
    this->damageTaken = t_damageTaken;
}

const bool Player::isSoundPlayed() const
{
    return this->soundPlayed;
}

const bool Player::isAbilityActive() const
{
    return this->abilityComponent->isAbilityActive();
}

const float Player::getAbilityCooldown() const
{
    return this->abilityComponent->getAbilityCooldown();
}

const float Player::getAbilityTime() const
{
    return this->abilityComponent->getAbilityTime();
}

const float Player::getAbilityTotalMaxTime() const
{
    return this->abilityComponent->getAbilityTotalMaxTime();
}

void Player::setBoughtItems(uint32_t t_boughtItems)
{
    this->boughtItems = t_boughtItems;
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

void Player::setProjectileAttack(uint32_t t_projectileAttack)
{
    this->projectileAttack = t_projectileAttack;
}

void Player::setProjectilePiercing(uint32_t t_projectilePiercing)
{
    this->projectilePiercing = t_projectilePiercing;
}

void Player::setProjectileArea(uint32_t t_projectileArea)
{
    this->projectileArea = t_projectileArea;
}

void Player::setIncreasedArmor(uint32_t t_increasedArmor)
{
    this->increasedArmor = t_increasedArmor;
}

void Player::setIncreasedReg(uint32_t t_increasedReg)
{
    this->increasedReg = t_increasedReg;
}

void Player::setIncreasedAttack(uint32_t t_increasedAttack)
{
    this->increasedAttack = t_increasedAttack;
}

void Player::setIncreasedGold(uint32_t t_increasedGold)
{
    this->increasedGold = t_increasedGold;
}

void Player::setIncreasedTargets(uint32_t t_increasedTargets)
{
    this->increasedTargets = t_increasedTargets;
}

void Player::setAttackLimit(uint32_t t_attackLimit)
{
    this->attackLimit = t_attackLimit;
}

void Player::setTimeSlowdown(float t_timeSlowdown)
{
    this->timeSlowdown = t_timeSlowdown;
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

void Player::setPendingXP(uint32_t t_pendingXP)
{
    this->pendingXP = t_pendingXP;
}

void Player::setSprinting(bool t_isSprinting)
{
    this->sprinting = t_isSprinting;
}

void Player::setAbilityActive(bool t_abilityActive)
{
    this->abilityComponent->setAbilityActive(t_abilityActive);
}

void Player::setAbilityCooldown(bool t_abilityCooldown)
{
    this->abilityComponent->setAbilityCooldown(t_abilityCooldown);
}

void Player::setAbilityMaxTime(float t_abilityMaxTime)
{
    this->abilityComponent->setAbilityMaxTime(t_abilityMaxTime);
}

void Player::setAbilityMaxTimeModifier(float t_abilityMaxTimeModifier)
{
    this->abilityComponent->setAbilityMaxTimeModifier(t_abilityMaxTimeModifier);
}

void Player::setAttackIncrease(uint32_t t_attackIncrease)
{
    this->abilityComponent->setAttackIncrease(t_attackIncrease);
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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keybinds.at("SPRINT"))) &&
        this->velocity != sf::Vector2f(0.f, 0.f)) {
        if (this->sprint > 0) {
            this->velocity *= 1.5f;

            this->sprinting = true;
            const float minusSprint = dt * 10.f;

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

const bool Player::levelUp()
{
    uint32_t newXP = 0;

    if (this->XP + this->pendingXP < this->maxXP) {
        newXP = this->pendingXP;
        this->pendingXP -= newXP;
        this->setXP(this->getXP() + newXP);
    }
    else {
        newXP = this->maxXP - this->XP;
        this->level++;
        this->maxXP += (sqrtf(5) - 1) * this->maxXP;
        this->pendingXP -= newXP;
        this->setXP(this->getXP() + newXP);
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
    if (this->upgraded && sf::Mouse::isButtonPressed(sf::Mouse::Right) &&
        this->getAbilityCooldown() == this->getAbilityTotalMaxTime()) {
        this->abilityComponent->setAbilityActive(true);
        this->abilityComponent->setAbilityCooldown(0.f);

        setAbilityTexture();
        this->ability.setColor(sf::Color(255, 255, 255, 128));
        return true;
    }
    return false;
}

void Player::abilityCounter(float dt)
{
    if (this->upgraded && this->abilityComponent->abilityCounter(dt)) {
        endAbility();
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

void Player::upgrade(const std::string &t_name, sf::IntRect &intRect)
{
    this->setAbilityActive(false);
    this->upgradeAttributes(t_name, intRect);
    this->setTexturePath("assets/textures/player/" +
                         toLowerCase(this->getName()) + ".png");
    this->setUpgraded(true);
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
    if (this->isAbilityActive()) {
        target.draw(this->ability);
    }
}

void Player::drawShadow(sf::RenderTarget &target)
{
    target.draw(this->shadow);
}
