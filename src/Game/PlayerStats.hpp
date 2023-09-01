#pragma once

#include "GUI/GUI.hpp"

class PlayerStats {
public:
    struct PlayerData {
        uint32_t wave;
        uint32_t xp;
        uint32_t maxLevel;
        uint32_t kills;
        uint32_t damageDealt;
        uint32_t damageTaken;
        uint32_t deaths;
        uint32_t gold;
        uint32_t boughtItems;
        uint32_t timePlayed;
        uint32_t gamesPlayed;
        uint32_t ruins;
        uint32_t desolation;
        uint32_t permafrost;
        uint32_t easy;
        uint32_t normal;
        uint32_t hard;
    };

    struct Rank {
        std::string name;
        uint32_t minXP;
        sf::Color color;
    };

    static const std::vector<Rank> ranks;

    static const CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
    static const CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE];

    static void saveStats(const std::string &filename,
                          const PlayerData &playerdata);
    static void loadStats(const std::string &filename, PlayerData &playerdata);

private:
    static void EncryptData(const std::string &plaintext,
                            std::string &ciphertext);
    static void DecryptData(const CryptoPP::byte *ciphertext,
                            size_t ciphertextLength, std::string &plaintext);
};
