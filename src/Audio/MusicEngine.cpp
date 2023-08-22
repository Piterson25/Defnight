#include "MusicEngine.hpp"

MusicEngine::MusicEngine(float volume) : volume(volume)
{
    this->music.setVolume(this->volume);
}

MusicEngine::~MusicEngine() = default;

void MusicEngine::addMusic(const std::string &name)
{
    this->tracks.push_back(name);
}

void MusicEngine::clearMusic()
{
    if (!this->hasStopped()) {
        this->music.stop();
    }
    this->tracks.clear();
}

void MusicEngine::playMusic()
{
    this->music.play();
}

void MusicEngine::playSelectedMusic(const std::string &name)
{
    stopMusic();
    if (!this->music.openFromFile("assets/music/" + name)) {
        throw("ERROR - COULDN'T FIND MUSIC");
    }
    playMusic();
}

void MusicEngine::pauseMusic()
{
    this->music.pause();
}

void MusicEngine::stopMusic()
{
    this->music.stop();
}

const bool MusicEngine::hasStopped() const
{
    return (this->music.getStatus() == sf::Music::Stopped);
}

const bool MusicEngine::isEmpty() const
{
    return this->tracks.empty();
}

void MusicEngine::update()
{
    if (this->hasStopped() && this->tracks.size() > 0) {
        const int tempTrack =
            static_cast<int>(Random::Float() * this->tracks.size());
        if (!this->music.openFromFile(
                "assets/music/" +
                this->tracks[static_cast<size_t>(tempTrack)])) {
            throw("ERROR - COULDN'T FIND MUSIC");
        }
        this->playMusic();
    }
}
