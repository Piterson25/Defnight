#include "SoundEngine.hpp"

SoundEngine::SoundEngine(float gameVolume, float playerVolume, float monsterVolume)
    : gameVolume(gameVolume), playerVolume(playerVolume), monsterVolume(monsterVolume)
{
    soundsMap.emplace("cyclops_hit", SoundType::MONSTER);
    soundsMap.emplace("cyclops_death", SoundType::MONSTER);
    soundsMap.emplace("cyclops_attack", SoundType::MONSTER);
    soundsMap.emplace("orc_attack", SoundType::MONSTER);
    soundsMap.emplace("orc_hit", SoundType::MONSTER);
    soundsMap.emplace("orc_death", SoundType::MONSTER);
    soundsMap.emplace("spider_hit", SoundType::MONSTER);
    soundsMap.emplace("spider_death", SoundType::MONSTER);
    soundsMap.emplace("spider_attack", SoundType::MONSTER);
    soundsMap.emplace("goblin_hit", SoundType::MONSTER);
    soundsMap.emplace("goblin_death", SoundType::MONSTER);
    soundsMap.emplace("goblin_attack", SoundType::MONSTER);
    soundsMap.emplace("minotaur_attack", SoundType::MONSTER);
    soundsMap.emplace("minotaur_ability", SoundType::MONSTER);
    soundsMap.emplace("minotaur_spawn", SoundType::MONSTER);
    soundsMap.emplace("minotaur_death", SoundType::MONSTER);
    soundsMap.emplace("whoosh_hit", SoundType::PLAYER);
    soundsMap.emplace("whoosh", SoundType::PLAYER);
    soundsMap.emplace("upgrade", SoundType::PLAYER);
    soundsMap.emplace("slowtime", SoundType::PLAYER);
    soundsMap.emplace("shuriken", SoundType::PLAYER);
    soundsMap.emplace("punch", SoundType::MONSTER);
    soundsMap.emplace("option", SoundType::GAME);
    soundsMap.emplace("new_wave", SoundType::GAME);
    soundsMap.emplace("levelup", SoundType::GAME);
    soundsMap.emplace("hi_hi", SoundType::GAME);
    soundsMap.emplace("heart", SoundType::GAME);
    soundsMap.emplace("gameover", SoundType::GAME);
    soundsMap.emplace("explosion", SoundType::PLAYER);
    soundsMap.emplace("coin", SoundType::GAME);
    soundsMap.emplace("buy", SoundType::GAME);
    soundsMap.emplace("button", SoundType::GAME);
    soundsMap.emplace("ability", SoundType::PLAYER);
}

SoundEngine::~SoundEngine()
{
    this->sounds.clear();
}

void SoundEngine::addSound(const std::string &name)
{
    const SoundType soundType = soundsMap.at(name);
    const float soundVolume = soundType == SoundType::GAME ? gameVolume : monsterVolume;
    this->sounds.emplace_back(std::make_unique<SoundEffect>(name, soundVolume, soundType));
}

void SoundEngine::playSounds()
{
    for (const auto &s : this->sounds) {
        if (s->hasStopped()) {
            s->play();
        }
    }
}

void SoundEngine::setVolume(float t_volume, SoundType soundType)
{
    switch (soundType) {
        case SoundType::GAME:
            gameVolume = t_volume;
        case SoundType::PLAYER:
            playerVolume = t_volume;
        case SoundType::MONSTER:
            monsterVolume = t_volume;
    }
}

void SoundEngine::stopSounds()
{
    for (const auto &s : this->sounds) {
        if (!s->hasStopped()) {
            s->stop();
        }
    }
}

void SoundEngine::update()
{
    for (auto s = this->sounds.begin(); s != this->sounds.end();) {
        if ((*s)->hasStopped()) {
            s = this->sounds.erase(s);
        }
        else {
            ++s;
        }
    }
}
