#include "stdafx.h"
#include "MusicEngine.h"

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
		if (!this->music.openFromFile("external/music/" + this->tracks[static_cast<size_t>(Random::Float()) * this->tracks.size()])) {
			throw("ERROR - COULDN'T FIND MUSIC");
		}
		this->playMusic();
	}
}
