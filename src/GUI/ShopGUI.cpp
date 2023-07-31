#include "ShopGUI.hpp"

ShopGUI::ShopGUI(sf::VideoMode &t_vm, Player &t_player)
    : vm(t_vm), player(t_player)
{
    this->attributesTexture.loadFromFile(
        "assets/textures/attributes_icons.png");
}

ShopGUI::~ShopGUI()
{
    this->shopItems.clear();
}

const uint32_t ShopGUI::getPrice(const std::string &t_name)
{
    return this->shopItems[t_name].price;
}

void ShopGUI::increasePrice(const std::string &t_name)
{
    this->shopItems[t_name].price += static_cast<uint32_t>(
        (((1 + sqrtf(5)) / 2.f) - 1) * this->shopItems[t_name].price);
    this->shopItems[t_name].itemPrice->setText(
        std::to_string(getPrice(t_name)));
}

void ShopGUI::addShopItem(const std::string &t_name, float t_x, float t_y,
                          uint32_t iconID, const std::string &desc,
                          const std::string &value, uint32_t price)
{
    this->shopItems.emplace(
        t_name,
        BuyItem{
            std::make_unique<gui::Sprite>(this->attributesTexture, t_x, t_y,
                                          calcScale(4, vm), false),
            std::make_unique<gui::ButtonSprite>(
                gui::RECT_BUTTON, t_x - calcX(12, vm), t_y - calcY(12, vm),
                calcScale(1, vm), false),
            std::make_unique<gui::Sprite>("assets/textures/lock.png", t_x, t_y,
                                          calcScale(4, vm), false),
            std::make_unique<gui::Text>(
                desc, calcChar(16, vm), t_x + calcX(166, vm),
                t_y - calcY(2, vm), sf::Color(255, 255, 255), true),
            std::make_unique<gui::Text>(
                value, calcChar(16, vm), t_x + calcX(166, vm),
                t_y + calcY(20, vm), sf::Color(255, 255, 255), true),
            std::make_unique<gui::Sprite>(
                this->attributesTexture, t_x + calcX(132, vm),
                t_y + calcY(38, vm), calcScale(2, vm), false),
            std::make_unique<gui::Text>(
                std::to_string(price), calcChar(16, vm), t_x + calcX(166, vm),
                t_y + calcY(48, vm), sf::Color(255, 246, 76), false),
            price,
            false,
        });

    this->shopItems[t_name].itemSprite->setTextureRect(
        sf::IntRect(16 * iconID, 0, 16, 16));

    this->shopItems[t_name].itemButton->setColor(gui::DARK_RED);

    this->shopItems[t_name].itemCoin->setTextureRect(sf::IntRect(0, 0, 16, 16));
}

const bool ShopGUI::isPressed(const std::string &t_name, bool mouseClicked)
{
    return this->shopItems[t_name].itemButton->isPressed() && !mouseClicked;
}

const bool ShopGUI::hasBoughtItem(const sf::Vector2i &mousePos,
                                  bool mouseClicked, const std::string &t_name,
                                  FloatingTextSystem *floatingTextSystem,
                                  SoundEngine *soundEngine)
{
    if (this->shopItems.find(t_name) == this->shopItems.end() ||
        this->shopItems[t_name].locked) {
        return false;
    }

    if (player.getGold() >= getPrice(t_name)) {
        update(t_name, mousePos);
        if (isPressed(t_name, mouseClicked)) {
            buy(t_name, floatingTextSystem);
            soundEngine->addSound("buy");
            return true;
        }
    }

    return false;
}

void ShopGUI::lockItem(const std::string t_name)
{
    this->shopItems[t_name].locked = true;
    this->shopItems[t_name].itemButton->setColor(gui::DARK_RED);
}

void ShopGUI::unlockItem(const std::string t_name)
{
    this->shopItems[t_name].locked = false;
}

void ShopGUI::disableItem(const std::string t_name)
{
    this->shopItems[t_name].itemButton->setColor(gui::DARK_RED);
}

void ShopGUI::buy(const std::string &t_name,
                  FloatingTextSystem *floatingTextSystem)
{
    player.setGold(player.getGold() - getPrice(t_name));
    floatingTextSystem->addFloatingText(
        gui::GOLD, "-" + std::to_string(getPrice(t_name)), calcChar(16, vm),
        calcX(20, vm), calcX(96, vm), true);
    increasePrice(t_name);
}

void ShopGUI::updateItemFrames()
{
    for (auto &pair : shopItems) {

        if (pair.first == "FULL_HP") {
            if (!pair.second.locked && player.getGold() >= pair.second.price &&
                player.getHP() < player.getMaxHP()) {
                pair.second.itemButton->setColor(gui::GREEN);
            }
            else {
                pair.second.itemButton->setColor(gui::DARK_RED);
            }
        }
        else {
            if (!pair.second.locked && player.getGold() >= pair.second.price) {
                pair.second.itemButton->setColor(gui::GREEN);
            }
            else {
                pair.second.itemButton->setColor(gui::DARK_RED);
            }
        }
    }
}

void ShopGUI::update(const std::string &t_name, const sf::Vector2i &mousePos)
{
    this->shopItems[t_name].itemButton->update(mousePos);
}

void ShopGUI::draw(sf::RenderTarget &target)
{
    for (const auto &pair : shopItems) {
        pair.second.itemSprite->draw(target);
        if (pair.second.locked) {
            pair.second.itemLock->draw(target);
        }
        pair.second.itemButton->draw(target);
        pair.second.itemDesc->draw(target);
        pair.second.itemValue->draw(target);
        pair.second.itemCoin->draw(target);
        pair.second.itemPrice->draw(target);
    }
}
