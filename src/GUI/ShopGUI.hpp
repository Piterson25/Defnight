#pragma once

#include "UpgradeGUI.hpp"

class ShopGUI {
public:
    ShopGUI(sf::VideoMode &t_vm, Player &t_player);
    ~ShopGUI();

    const uint32_t getPrice(const std::string &t_name);

    void addShopItem(const std::string &t_name, float t_x, float t_y,
                     uint32_t iconID, const std::string &desc,
                     const std::string &value, uint32_t price,
                     uint32_t boughtNumber, uint32_t maxNumber);
    void updateSegments(const std::string &t_name);

    const bool hasBoughtItem(const sf::Vector2i &mousePos,
                             const std::string &t_name,
                             FloatingTextSystem *floatingTextSystem,
                             SoundEngine *soundEngine);
    void lockItem(const std::string &t_name);
    void unlockItem(const std::string &t_name);
    void disableItem(const std::string &t_name);
    void buy(const std::string &t_name, FloatingTextSystem *floatingTextSystem);
    void updateItemFrames();
    void draw(sf::RenderTarget &target);

private:
    sf::VideoMode &vm;
    Player &player;
    sf::Texture attributesTexture;

    std::unordered_map<std::string, BuyItem> shopItems;
};
