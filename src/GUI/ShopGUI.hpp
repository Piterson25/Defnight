#pragma once

#include "UpgradeGUI.hpp"

class ShopGUI {
public:
    ShopGUI(sf::VideoMode &t_vm, Player &t_player);
    ~ShopGUI();

    struct BuyItem {
        std::unique_ptr<gui::Sprite> itemSprite;
        std::unique_ptr<gui::Sprite> itemFrame;
        std::unique_ptr<gui::ButtonSprite> itemButton;
        std::unique_ptr<gui::Text> itemDesc;
        std::unique_ptr<gui::Text> itemValue;
        std::unique_ptr<gui::Sprite> itemCoin;
        std::unique_ptr<gui::Text> itemPrice;
        uint32_t price;
    };

    const uint32_t getPrice(const std::string &t_name);

    void increasePrice(const std::string &t_name);

    void deleteItem(const std::string &t_name);

    void addShopItem(const std::string &t_name, float t_x, float t_y,
                     uint32_t iconID, const std::string &desc,
                     const std::string &value, uint32_t price);

    const bool isPressed(const std::string &t_name, bool mouseClicked);
    const bool hasBoughtItem(const sf::Vector2i &mousePos, bool mouseClicked,
                             const std::string &t_name,
                             FloatingTextSystem *floatingTextSystem,
                             SoundEngine *soundEngine);
    void disableItem(const std::string t_name);
    void buy(const std::string &t_name, FloatingTextSystem *floatingTextSystem);
    void updateItemFrames();
    void update(const std::string &t_name, const sf::Vector2i &mousePos);
    void draw(sf::RenderTarget &target);

private:
    sf::VideoMode &vm;
    Player &player;
    sf::Texture selectsTexture;
    sf::Texture attributesTexture;

    std::map<std::string, BuyItem> shopItems;
};
