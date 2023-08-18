#include "PlayerStats.hpp"

void PlayerStats::saveStats(const std::string &filename, PlayerData &playerdata)
{
    PlayerData loadedPlayerData{0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0};
    loadStats("data/player_stats.dat", loadedPlayerData);

    const uint32_t wave = std::max(playerdata.wave, loadedPlayerData.wave);
    const uint32_t xp = playerdata.xp + loadedPlayerData.xp;
    const uint32_t maxLevel =
        std::max(playerdata.maxLevel, loadedPlayerData.maxLevel);
    const uint32_t kills = playerdata.kills + loadedPlayerData.kills;
    const uint32_t damageDealt =
        playerdata.damageDealt + loadedPlayerData.damageDealt;
    const uint32_t damageTaken =
        playerdata.damageTaken + loadedPlayerData.damageTaken;
    const uint32_t deaths = playerdata.deaths + loadedPlayerData.deaths;
    const uint32_t gold = playerdata.gold + loadedPlayerData.gold;
    const uint32_t boughtItems =
        playerdata.boughtItems + loadedPlayerData.boughtItems;

    const uint32_t timePlayed =
        playerdata.timePlayed + loadedPlayerData.timePlayed;
    const uint32_t gamesPlayed =
        playerdata.gamesPlayed + loadedPlayerData.gamesPlayed;
    const uint32_t ruins = playerdata.ruins + loadedPlayerData.ruins;
    const uint32_t desolation =
        playerdata.desolation + loadedPlayerData.desolation;
    const uint32_t permafrost =
        playerdata.permafrost + loadedPlayerData.permafrost;
    const uint32_t easy = playerdata.easy + loadedPlayerData.easy;
    const uint32_t normal = playerdata.normal + loadedPlayerData.normal;
    const uint32_t hard = playerdata.hard + loadedPlayerData.hard;

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing." << std::endl;
        return;
    }

    const std::string data =
        std::to_string(wave) + "\n" + std::to_string(xp) + "\n" +
        std::to_string(maxLevel) + "\n" + std::to_string(kills) + "\n" +
        std::to_string(damageDealt) + "\n" + std::to_string(damageTaken) +
        "\n" + std::to_string(deaths) + "\n" + std::to_string(gold) + "\n" +
        std::to_string(boughtItems) + "\n" + std::to_string(timePlayed) + "\n" +
        std::to_string(gamesPlayed) + "\n" + std::to_string(ruins) + "\n" +
        std::to_string(desolation) + "\n" + std::to_string(permafrost) + "\n" +
        std::to_string(easy) + "\n" + std::to_string(normal) + "\n" +
        std::to_string(hard);
    std::string encodedData;
    CryptoPP::StringSource(
        data, true,
        new CryptoPP::Base64Encoder(new CryptoPP::StringSink(encodedData)));

    file << encodedData;
}

void PlayerStats::loadStats(const std::string &filename, PlayerData &playerdata)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file for reading." << std::endl;
        return;
    }

    std::string encodedData;
    file >> encodedData;

    std::string decodedData;
    CryptoPP::StringSource(
        encodedData, true,
        new CryptoPP::Base64Decoder(new CryptoPP::StringSink(decodedData)));

    std::istringstream iss(decodedData);
    iss >> playerdata.wave;
    iss >> playerdata.xp;
    iss >> playerdata.maxLevel;
    iss >> playerdata.kills;
    iss >> playerdata.damageDealt;
    iss >> playerdata.damageTaken;
    iss >> playerdata.deaths;
    iss >> playerdata.gold;
    iss >> playerdata.boughtItems;

    iss >> playerdata.timePlayed;
    iss >> playerdata.gamesPlayed;
    iss >> playerdata.ruins;
    iss >> playerdata.desolation;
    iss >> playerdata.permafrost;
    iss >> playerdata.easy;
    iss >> playerdata.normal;
    iss >> playerdata.hard;
}
