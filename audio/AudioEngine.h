#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <string>
#include <map>
#include <vector>
#include <utility>
using namespace std;

#include <core/Commons.h>
#include <core/Console.h>
#include <audio/SoundLoader.h>

typedef map<string, pair<ALuint, ALuint>> SoundCache;

class AudioEngine
{
public:
	static AudioEngine& getInstance();

	void initialize();
	void free();

	void loadSound(const string& path, const string& key);
	void playSound(const string& key);

private:
	AudioEngine();

	SoundCache m_cache;
};

#endif
