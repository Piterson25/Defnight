#pragma once

#include "Entities/Player/Player.hpp"

class PlayerStats {
public:
    struct PlayerData {
        uint32_t wave;
        uint32_t kills;
        uint32_t gold;
    };

    static void saveStats(Player &player, const std::string &filename);
    static void loadStats(const std::string &filename, PlayerData &playerdata);
};
