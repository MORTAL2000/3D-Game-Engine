#include "AudioEngine.h"

AudioEngine::AudioEngine()
{

}

AudioEngine& AudioEngine::getInstance()
{
	static AudioEngine instance;
	return instance;
}

void AudioEngine::initialize()
{
	Console::log("Audio: Initializing...");
	const char* devname = alcGetString(0, ALC_DEFAULT_DEVICE_SPECIFIER);
	Console::log("Audio: (Device) %s", devname);

	ALCdevice *dev = alcOpenDevice(0);
	if(!dev)
	{
		Console::log("Audio: Could not access (device) %s", devname);
	}

	ALCcontext *ctx = alcCreateContext(dev, 0);
	if(!ctx)
	{
		Console::log("Audio: Could not create device context");
	}

	alcMakeContextCurrent(ctx);
}

void AudioEngine::free()
{
	for(SoundCache::iterator it = m_cache.begin(); it != m_cache.end(); it++)
	{
		ALuint& buffer = it->second.first;
		ALuint& source = it->second.second;

		alDeleteSources(1, &source);
		alDeleteBuffers(1, &buffer);
	}

	ALCcontext *ctx = alcGetCurrentContext();
    ALCdevice *dev = alcGetContextsDevice(ctx);

    alcMakeContextCurrent(0);
    alcDestroyContext(ctx);
    alcCloseDevice(dev);
	Console::log("Audio: Released.");
}

void AudioEngine::loadSound(const string& path, const string& key)
{
	ALuint buffer;
	ALuint source;

	alGenBuffers(1, &buffer);
	alGenSources(1, &source);

	//load sound path
	vector<short> samples;
	float sampleRate = 44100;

	alBufferData(buffer, AL_FORMAT_MONO16, samples.data(), samples.size(), sampleRate);
	alSourcei(source, AL_BUFFER, buffer);

	m_cache[key] = make_pair(buffer, source);
}


void AudioEngine::play(const string& key)
{
	for(SoundCache::iterator it = m_cache.begin(); it != m_cache.end(); it++)
	{
		if(it->first == key)
		{
			// std::pair<string, std::pair<ALuint, ALuint>>;
			// std::pair<"BANG!", std::pair<buffer, source>>;
			alSourcePlay(it->second.second);
		}
	}
}
