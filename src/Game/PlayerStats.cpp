#include "PlayerStats.hpp"

const std::vector<PlayerStats::Rank> PlayerStats::ranks = {
    {"LEGEND", 1000000, gui::GOLD}, {"MASTER", 500000, gui::DARK_RED}, {"GENERAL", 150000, gui::FLAMINGO},
    {"MAJOR", 50000, gui::GREEN},   {"VETERAN", 15000, gui::BROWN},    {"CAPTAIN", 5000, gui::BLUE},
    {"SOLDIER", 2000, gui::PURPLE}, {"AMATEUR", 500, gui::LIME},       {"RECRUIT", 100, gui::LIGHT_GREY},
    {"NOVICE", 0, gui::WHITE}};

const uint8_t PlayerStats::key[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                                      0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};

const uint8_t PlayerStats::iv[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                     0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

void PlayerStats::resetStats()
{
    PlayerData emptyPlayerdata{0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_ECB_encrypt(&ctx, (uint8_t *)&emptyPlayerdata);

    std::ofstream file(PlayerStats::filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Failed to save player stats " << PlayerStats::filePath << '\n';
        return;
    }
    file.write((char *)&emptyPlayerdata, sizeof(PlayerData));
}

void PlayerStats::saveStats(PlayerData &playerdata)
{
    PlayerData loadedPlayerData{0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    loadStats(loadedPlayerData);

    playerdata.wave = std::max(playerdata.wave, loadedPlayerData.wave);
    playerdata.xp = playerdata.xp + loadedPlayerData.xp;
    playerdata.maxLevel = std::max(playerdata.maxLevel, loadedPlayerData.maxLevel);
    playerdata.kills = playerdata.kills + loadedPlayerData.kills;
    playerdata.damageDealt = playerdata.damageDealt + loadedPlayerData.damageDealt;
    playerdata.damageTaken = playerdata.damageTaken + loadedPlayerData.damageTaken;
    playerdata.deaths = playerdata.deaths + loadedPlayerData.deaths;
    playerdata.gold = playerdata.gold + loadedPlayerData.gold;
    playerdata.boughtItems = playerdata.boughtItems + loadedPlayerData.boughtItems;

    playerdata.timePlayed = playerdata.timePlayed + loadedPlayerData.timePlayed;
    playerdata.gamesPlayed = playerdata.gamesPlayed + loadedPlayerData.gamesPlayed;
    playerdata.ruins = playerdata.ruins + loadedPlayerData.ruins;
    playerdata.desolation = playerdata.desolation + loadedPlayerData.desolation;
    playerdata.permafrost = playerdata.permafrost + loadedPlayerData.permafrost;
    playerdata.easy = playerdata.easy + loadedPlayerData.easy;
    playerdata.normal = playerdata.normal + loadedPlayerData.normal;
    playerdata.hard = playerdata.hard + loadedPlayerData.hard;
    playerdata.extreme = playerdata.extreme + loadedPlayerData.extreme;

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_ECB_encrypt(&ctx, (uint8_t *)&playerdata);

    std::ofstream file(PlayerStats::filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Failed to save player stats " << PlayerStats::filePath << '\n';
        return;
    }
    file.write((char *)&playerdata, sizeof(PlayerData));
}

void PlayerStats::loadStats(PlayerData &playerdata)
{
    std::ifstream file(PlayerStats::filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Failed to load player stats " << PlayerStats::filePath << '\n';
        return;
    }
    file.read((char *)&playerdata, sizeof(PlayerData));

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_ECB_decrypt(&ctx, (uint8_t *)&playerdata);
}
