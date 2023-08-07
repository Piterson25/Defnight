#include "UpgradeGUI.hpp"

UpgradeGUI::UpgradeGUI(sf::VideoMode &t_vm, Player &t_player)
    : vm(t_vm), player(t_player)
{
    this->abilitiesTexture.loadFromFile("assets/textures/abilities_icons.png");
    this->attributesTexture.loadFromFile(
        "assets/textures/attributes_icons.png");
    this->upgradesTexture.loadFromFile("assets/textures/upgrades_icons.png");
}

UpgradeGUI::~UpgradeGUI()
{
    this->upgrades.clear();
}

const bool UpgradeGUI::isPressed(const std::string &t_name, bool mouseClicked)
{
    return this->upgrades[t_name].button->isPressed() && !mouseClicked;
}

void UpgradeGUI::changeUpgrade(const std::string &t_name, float t_x, float t_y,
                               const std::string &desc, uint32_t abilityIconID,
                               uint32_t attributeIconID,
                               uint32_t attributeValue,
                               const std::string &attributeText)
{
    this->upgrades[t_name] = Upgrade{
        std::make_unique<gui::Sprite>(this->upgradesTexture, t_x, t_y,
                                      calcScale(4, vm), false),
        std::make_unique<gui::ButtonSprite>(
            gui::RECT_BUTTON, t_x - calcX(12, vm), t_y - calcY(12, vm),
            calcScale(1, vm), false),
        std::make_unique<gui::Text>(desc, calcChar(16, vm),
                                    t_x + calcX(160, vm), t_y + calcY(4, vm),
                                    sf::Color(255, 255, 255), true),
        std::make_unique<gui::Sprite>(this->abilitiesTexture,
                                      t_x + calcX(100, vm), t_y + calcY(28, vm),
                                      calcScale(2, vm), false),
        std::make_unique<gui::Sprite>(this->attributesTexture,
                                      t_x + calcX(152, vm), t_y + calcY(28, vm),
                                      calcScale(2, vm), false),
        std::make_unique<gui::Text>("+" + attributeText, calcChar(16, vm),
                                    t_x + calcX(190, vm), t_y + calcY(37, vm),
                                    sf::Color(255, 255, 255), false),
        attributeValue,
    };

    const uint32_t positionX = static_cast<uint32_t>(abilityIconID % 3);
    const uint32_t positionY = static_cast<uint32_t>(abilityIconID / 3);

    this->upgrades[t_name].sprite->setTextureRect(
        sf::IntRect(16 * positionX, 16 * positionY + 16, 16, 16));

    this->upgrades[t_name].ability->setTextureRect(
        sf::IntRect(16 * positionX, 16 * positionY, 16, 16));

    this->upgrades[t_name].attribute->setTextureRect(
        sf::IntRect(16 * attributeIconID, 0, 16, 16));
}

void UpgradeGUI::deleteUpgrade(const std::string &t_name)
{
    this->upgrades.erase(t_name);
}

const bool UpgradeGUI::hasClickedUpgrade(const sf::Vector2i &mousePos,
                                         bool mouseClicked,
                                         const std::string &t_name,
                                         SoundEngine *soundEngine)
{
    if (this->upgrades.find(t_name) == this->upgrades.end()) {
        return false;
    }

    update(t_name, mousePos);
    if (isPressed(t_name, mouseClicked)) {
        soundEngine->addSound("upgrade");
        return true;
    }

    return false;
}

void UpgradeGUI::update(const std::string &t_name, const sf::Vector2i &mousePos)
{
    this->upgrades[t_name].button->update(mousePos);
}

void UpgradeGUI::draw(sf::RenderTarget &target)
{
    for (const auto &pair : upgrades) {
        if (this->player.getLevel() == 10 && pair.first == "UPGRADE3") {
            continue;
        }

        pair.second.sprite->draw(target);
        pair.second.button->draw(target);
        pair.second.name->draw(target);
        pair.second.ability->draw(target);
        pair.second.attribute->draw(target);
        pair.second.attributeEffect->draw(target);
    }
}
