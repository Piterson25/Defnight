#include "PlayerStats.hpp"

const std::vector<PlayerStats::Rank> PlayerStats::ranks = {
    {"LEGEND", 1000000, gui::GOLD},     {"MASTER", 500000, gui::DARK_RED},
    {"GENERAL", 150000, gui::FLAMINGO}, {"MAJOR", 50000, gui::GREEN},
    {"VETERAN", 15000, gui::BROWN},     {"CAPTAIN", 5000, gui::BLUE},
    {"SOLDIER", 2000, gui::PURPLE},     {"AMATEUR", 500, gui::LIME},
    {"RECRUIT", 100, gui::LIGHT_GREY},  {"NOVICE", 0, gui::WHITE}};

const CryptoPP::byte PlayerStats::key[CryptoPP::AES::DEFAULT_KEYLENGTH] = {
    0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
    0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};

const CryptoPP::byte PlayerStats::iv[CryptoPP::AES::BLOCKSIZE] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

void PlayerStats::saveStats(const std::string &filename,
                            const PlayerData &playerdata)
{
    PlayerData loadedPlayerData{0, 0, 1, 0, 0, 0, 0, 0, 0,
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

    std::string data;
    data.reserve(80);
    data.append(reinterpret_cast<const char *>(&wave), sizeof(wave));
    data.append(reinterpret_cast<const char *>(&xp), sizeof(xp));
    data.append(reinterpret_cast<const char *>(&maxLevel), sizeof(maxLevel));
    data.append(reinterpret_cast<const char *>(&kills), sizeof(kills));
    data.append(reinterpret_cast<const char *>(&damageDealt),
                sizeof(damageDealt));
    data.append(reinterpret_cast<const char *>(&damageTaken),
                sizeof(damageTaken));
    data.append(reinterpret_cast<const char *>(&deaths), sizeof(deaths));
    data.append(reinterpret_cast<const char *>(&gold), sizeof(gold));
    data.append(reinterpret_cast<const char *>(&boughtItems),
                sizeof(boughtItems));
    data.append(reinterpret_cast<const char *>(&timePlayed),
                sizeof(timePlayed));
    data.append(reinterpret_cast<const char *>(&gamesPlayed),
                sizeof(gamesPlayed));
    data.append(reinterpret_cast<const char *>(&ruins), sizeof(ruins));
    data.append(reinterpret_cast<const char *>(&desolation),
                sizeof(desolation));
    data.append(reinterpret_cast<const char *>(&permafrost),
                sizeof(permafrost));
    data.append(reinterpret_cast<const char *>(&easy), sizeof(easy));
    data.append(reinterpret_cast<const char *>(&normal), sizeof(normal));
    data.append(reinterpret_cast<const char *>(&hard), sizeof(hard));

    std::string encryptedData;
    EncryptData(data, encryptedData);

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing." << std::endl;
        return;
    }
    file.write(encryptedData.c_str(), encryptedData.size());
}

void PlayerStats::loadStats(const std::string &filename, PlayerData &playerdata)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file for reading." << std::endl;
        return;
    }

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<CryptoPP::byte> encryptedData(fileSize);
    file.read(reinterpret_cast<char *>(encryptedData.data()), fileSize);

    std::string decryptedData;
    DecryptData(encryptedData.data(), fileSize, decryptedData);

    size_t offset = 0;
    memcpy(&playerdata.wave, decryptedData.c_str() + offset,
           sizeof(playerdata.wave));
    offset += sizeof(playerdata.wave);
    memcpy(&playerdata.xp, decryptedData.c_str() + offset,
           sizeof(playerdata.xp));
    offset += sizeof(playerdata.xp);
    memcpy(&playerdata.maxLevel, decryptedData.c_str() + offset,
           sizeof(playerdata.maxLevel));
    offset += sizeof(playerdata.maxLevel);
    memcpy(&playerdata.kills, decryptedData.c_str() + offset,
           sizeof(playerdata.kills));
    offset += sizeof(playerdata.kills);
    memcpy(&playerdata.damageDealt, decryptedData.c_str() + offset,
           sizeof(playerdata.damageDealt));
    offset += sizeof(playerdata.damageDealt);
    memcpy(&playerdata.damageTaken, decryptedData.c_str() + offset,
           sizeof(playerdata.damageTaken));
    offset += sizeof(playerdata.damageTaken);
    memcpy(&playerdata.deaths, decryptedData.c_str() + offset,
           sizeof(playerdata.deaths));
    offset += sizeof(playerdata.deaths);
    memcpy(&playerdata.gold, decryptedData.c_str() + offset,
           sizeof(playerdata.gold));
    offset += sizeof(playerdata.gold);
    memcpy(&playerdata.boughtItems, decryptedData.c_str() + offset,
           sizeof(playerdata.boughtItems));
    offset += sizeof(playerdata.boughtItems);
    memcpy(&playerdata.timePlayed, decryptedData.c_str() + offset,
           sizeof(playerdata.timePlayed));
    offset += sizeof(playerdata.timePlayed);
    memcpy(&playerdata.gamesPlayed, decryptedData.c_str() + offset,
           sizeof(playerdata.gamesPlayed));
    offset += sizeof(playerdata.gamesPlayed);
    memcpy(&playerdata.ruins, decryptedData.c_str() + offset,
           sizeof(playerdata.ruins));
    offset += sizeof(playerdata.ruins);
    memcpy(&playerdata.desolation, decryptedData.c_str() + offset,
           sizeof(playerdata.desolation));
    offset += sizeof(playerdata.desolation);
    memcpy(&playerdata.permafrost, decryptedData.c_str() + offset,
           sizeof(playerdata.permafrost));
    offset += sizeof(playerdata.permafrost);
    memcpy(&playerdata.easy, decryptedData.c_str() + offset,
           sizeof(playerdata.easy));
    offset += sizeof(playerdata.easy);
    memcpy(&playerdata.normal, decryptedData.c_str() + offset,
           sizeof(playerdata.normal));
    offset += sizeof(playerdata.normal);
    memcpy(&playerdata.hard, decryptedData.c_str() + offset,
           sizeof(playerdata.hard));
}

void PlayerStats::EncryptData(const std::string &plaintext,
                              std::string &ciphertext)
{
    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryptor;
    encryptor.SetKeyWithIV(PlayerStats::key, CryptoPP::AES::DEFAULT_KEYLENGTH,
                           PlayerStats::iv);

    CryptoPP::StringSource(
        plaintext, true,
        new CryptoPP::StreamTransformationFilter(
            encryptor, new CryptoPP::StringSink(ciphertext)));
}

void PlayerStats::DecryptData(const CryptoPP::byte *ciphertext,
                              size_t ciphertextLength, std::string &plaintext)
{
    CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryptor;
    decryptor.SetKeyWithIV(PlayerStats::key, CryptoPP::AES::DEFAULT_KEYLENGTH,
                           PlayerStats::iv);

    CryptoPP::StreamTransformationFilter filter(
        decryptor, new CryptoPP::StringSink(plaintext));
    filter.Put(ciphertext, ciphertextLength);
    filter.MessageEnd();
}
