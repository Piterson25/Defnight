#include "MusicEngine.hpp"

MusicEngine::MusicEngine(const float& volume)
	:volume(volume)
{
	this->music.setVolume(volume);
}

MusicEngine::~MusicEngine()
{

}

void MusicEngine::addMusic(const std::string& name)
{
	this->tracks.push_back(name);
}

void MusicEngine::clearMusic()
{
	if (!this->hasStopped()) this->music.stop();
	this->tracks.clear();
}

void MusicEngine::playMusic()
{
	this->music.play();
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
	if (this->music.getStatus() == sf::Music::Stopped) return true;
	return false;
}

const bool MusicEngine::isEmpty() const
{
	return this->tracks.empty();
}

void MusicEngine::update()
{
	if (this->hasStopped() && this->tracks.size() > 0) {
		const int tempTrack = static_cast<int>(Random::Float() * this->tracks.size());
		if (!this->music.openFromFile("assets/music/" + this->tracks[static_cast<size_t>(tempTrack)])) {
			throw("ERROR - COULDN'T FIND MUSIC");
		}
		this->playMusic();
	}
}
