#ifndef SOUNDENGINE_HPP
#define SOUNDENGINE_HPP

#include "SoundEffect.hpp"

class SoundEngine {
public:
    SoundEngine(const float &volume);
    virtual ~SoundEngine();

    void addSound(const std::string &name);
    void playSounds();
    void stopSounds();

    void update();

private:
    std::list<std::unique_ptr<SoundEffect>> sounds;
    float volume;
};

#endif