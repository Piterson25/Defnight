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

void ShopGUI::addShopItem(const std::string &t_name, float t_x, float t_y,
                          uint32_t iconID, const std::string &desc,
                          const std::string &value, uint32_t price,
                          uint32_t boughtNumber, uint32_t maxNumber)
{
    float offsetY = 0.f;

    if (maxNumber > 0) {
        offsetY = 12.f;
    }

    this->shopItems.emplace(
        t_name,
        BuyItem{
            std::make_unique<gui::Sprite>(this->attributesTexture, t_x, t_y,
                                          calcScale(4, vm), false),
            std::make_unique<gui::ButtonSprite>(
                gui::RECT_BUTTON, t_x - calcX(12, vm), t_y - calcY(12, vm),
                calcScale(1, vm), gui::DARK_GREEN, gui::WHITE, false),
            std::make_unique<gui::Sprite>("assets/textures/lock.png", t_x, t_y,
                                          calcScale(4, vm), false),
            std::make_unique<gui::Text>(desc, calcChar(16, vm),
                                        t_x + calcX(166, vm),
                                        t_y - calcY(2, vm) - calcY(offsetY, vm),
                                        sf::Color(255, 255, 255), true),
            std::make_unique<gui::Text>(
                value, calcChar(16, vm), t_x + calcX(166, vm),
                t_y + calcY(20, vm) - calcY(offsetY, vm),
                sf::Color(255, 255, 255), true),
            std::make_unique<gui::Sprite>(
                this->attributesTexture, t_x + calcX(132, vm),
                t_y + calcY(38, vm) - calcY(offsetY, vm), calcScale(2, vm),
                false),
            std::make_unique<gui::Text>(
                std::to_string(price), calcChar(16, vm), t_x + calcX(166, vm),
                t_y + calcY(48, vm) - calcY(offsetY, vm), gui::GOLD, false),
            std::vector<std::unique_ptr<gui::Sprite>>(),
            boughtNumber,
            maxNumber,
            price,
            false,
        });

    this->shopItems[t_name].sprite->setTextureRect(
        sf::IntRect(16 * iconID, 0, 16, 16));

    this->shopItems[t_name].button->setColor(gui::DARK_RED);

    this->shopItems[t_name].coinSprite->setTextureRect(
        sf::IntRect(0, 0, 16, 16));

    if (maxNumber > 0) {
        sf::Texture texture;
        texture.loadFromFile("assets/textures/progress_segment.png");
        const sf::Vector2f position =
            this->shopItems[t_name].sprite->getPosition();
        for (uint32_t i = 0; i < maxNumber; i++) {
            this->shopItems[t_name].segments.push_back(
                std::make_unique<gui::Sprite>(
                    texture,
                    position.x + calcX(static_cast<float>(i) * 12.f + 108, vm),
                    position.y + calcY(74, vm) - calcY(offsetY, vm),
                    calcScale(2, vm), false));
            this->shopItems[t_name].segments[i]->setTextureRect(
                sf::IntRect(4, 0, 4, 8));

            if (i < boughtNumber) {
                this->shopItems[t_name].segments[i]->setTextureRect(
                    sf::IntRect(0, 0, 4, 8));
            }
        }
    }
}

void ShopGUI::updateSegments(const std::string &t_name)
{
    this->shopItems[t_name]
        .segments[this->shopItems[t_name].boughtNumber]
        ->setTextureRect(sf::IntRect(0, 0, 4, 8));
    this->shopItems[t_name].boughtNumber++;

    if (this->shopItems[t_name].boughtNumber ==
        this->shopItems[t_name].maxNumber) {
        lockItem(t_name);
    }
}

const bool ShopGUI::hasBoughtItem(const sf::Vector2i &mousePos,
                                  const std::string &t_name,
                                  FloatingTextSystem *floatingTextSystem,
                                  SoundEngine *soundEngine)
{
    if (this->shopItems.find(t_name) == this->shopItems.end() ||
        this->shopItems[t_name].locked) {
        return false;
    }

    if (player.getGold() >= getPrice(t_name)) {
        if (this->shopItems[t_name].button->isPressed(mousePos)) {
            buy(t_name, floatingTextSystem);
            player.setBoughtItems(player.getBoughtItems() + 1);
            soundEngine->addSound("buy");
            return true;
        }
    }

    return false;
}

void ShopGUI::lockItem(const std::string &t_name)
{
    this->shopItems[t_name].locked = true;
    this->shopItems[t_name].button->setColor(gui::DARK_RED);
}

void ShopGUI::unlockItem(const std::string &t_name)
{
    this->shopItems[t_name].locked = false;
}

void ShopGUI::disableItem(const std::string &t_name)
{
    this->shopItems[t_name].button->setColor(gui::DARK_RED);
}

void ShopGUI::buy(const std::string &t_name,
                  FloatingTextSystem *floatingTextSystem)
{
    player.setGold(player.getGold() - getPrice(t_name));
    floatingTextSystem->addFloatingText(
        gui::GOLD, "-" + std::to_string(getPrice(t_name)), calcChar(16, vm),
        calcX(20, vm), calcX(96, vm), true);
}

void ShopGUI::updateItemFrames()
{
    for (auto &pair : shopItems) {

        if (pair.first == "FULL_HP") {
            if (!pair.second.locked && player.getGold() >= pair.second.price &&
                player.getHP() < player.getMaxHP()) {
                pair.second.button->setColor(gui::DARK_GREEN);
            }
            else {
                pair.second.button->setColor(gui::DARK_RED);
            }
        }
        else {
            if (!pair.second.locked && player.getGold() >= pair.second.price) {
                pair.second.button->setColor(gui::DARK_GREEN);
            }
            else {
                pair.second.button->setColor(gui::DARK_RED);
            }
        }
    }
}

void ShopGUI::draw(sf::RenderTarget &target)
{
    for (const auto &pair : shopItems) {
        pair.second.sprite->draw(target);
        if (pair.second.locked) {
            pair.second.lockSprite->draw(target);
        }
        pair.second.button->draw(target);
        pair.second.desc->draw(target);
        pair.second.effect->draw(target);
        pair.second.coinSprite->draw(target);
        pair.second.priceText->draw(target);
        for (const auto &segment : pair.second.segments) {
            segment->draw(target);
        }
    }
}
