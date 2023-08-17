#include "PlayerStats.hpp"

void PlayerStats::saveStats(Player &player, const std::string &filename)
{
    PlayerData playerData{0, 0, 0};
    loadStats("data/player_stats.dat", playerData);

    const uint32_t wave = std::max(player.getWave(), playerData.wave);
    const uint32_t kills = std::max(player.getKills(), playerData.kills);
    const uint32_t gold = std::max(player.getGold(), playerData.gold);

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing." << std::endl;
        return;
    }

    const std::string data = std::to_string(wave) + "\n" +
                             std::to_string(kills) + "\n" +
                             std::to_string(gold);
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
    iss >> playerdata.kills;
    iss >> playerdata.gold;
}
