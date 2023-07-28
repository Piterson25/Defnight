#include "ShopGUI.hpp"

ShopGUI::ShopGUI(sf::VideoMode &t_vm, Player &t_player)
    : vm(t_vm), player(t_player)
{
    this->selectsTexture.loadFromFile("assets/textures/select.png");
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

void ShopGUI::deleteItem(const std::string &t_name)
{
    this->shopItems.erase(t_name);
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
            std::make_unique<gui::Sprite>(
                this->selectsTexture, t_x - calcX(12, vm), t_y - calcY(12, vm),
                calcScale(1, vm), false),
            std::make_unique<gui::ButtonSprite>(
                this->selectsTexture, t_x - calcX(12, vm), t_y - calcY(12, vm),
                calcScale(1, vm), false),
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
        });

    this->shopItems[t_name].itemSprite->setTextureRect(
        sf::IntRect(16 * iconID, 0, 16, 16));

    this->shopItems[t_name].itemFrame->setTextureRect(
        sf::IntRect(176, 0, 88, 88));

    this->shopItems[t_name].itemButton->setTextureRect(
        sf::IntRect(0, 0, 88, 88));

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
    if (this->shopItems.find(t_name) == this->shopItems.end()) {
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

void ShopGUI::disableItem(const std::string t_name)
{
    this->shopItems[t_name].itemFrame->setTextureRect(
        sf::IntRect(176, 0, 88, 88));
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
        pair.second.itemButton->setTransparent();

        if (pair.first == "FULL_HP") {
            if (player.getGold() >= pair.second.price &&
                player.getHP() < player.getMaxHP()) {
                pair.second.itemFrame->setTextureRect(
                    sf::IntRect(264, 0, 88, 88));
            }
            else {
                pair.second.itemFrame->setTextureRect(
                    sf::IntRect(176, 0, 88, 88));
            }
        }
        else {
            if (player.getGold() >= pair.second.price) {
                pair.second.itemFrame->setTextureRect(
                    sf::IntRect(264, 0, 88, 88));
            }
            else {
                pair.second.itemFrame->setTextureRect(
                    sf::IntRect(176, 0, 88, 88));
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
        pair.second.itemFrame->draw(target);
        pair.second.itemButton->draw(target);
        pair.second.itemDesc->draw(target);
        pair.second.itemValue->draw(target);
        pair.second.itemCoin->draw(target);
        pair.second.itemPrice->draw(target);
    }
}
