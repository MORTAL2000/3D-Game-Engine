#include "AudioEngine.h"

AudioEngine::AudioEngine() {}

AudioEngine& AudioEngine::getInstance()
{
	static AudioEngine instance;
	return instance;
}

void AudioEngine::initialize()
{
	const char* devname = alcGetString(0, ALC_DEFAULT_DEVICE_SPECIFIER);
	Console::log("Audio: Initializing -> (Device) %s", devname);

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
	Sound sound = SoundLoader::getInstance().read(path);
	if(!sound.valid())
	{
		Console::log("Audio: File contains no samples '%s'", path.c_str());
		return;
	}

	ALuint source;
	ALuint buffer;

	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_FALSE);

	ALenum format;
	if(sound.getChannels() == 1)
	{
		if(sound.getBitsPerSample() == 8)
		{
			format = AL_FORMAT_MONO8;
		}
		else
		{
			format = AL_FORMAT_MONO16;
		}
	}
	else if(sound.getChannels() == 2)
	{
		if(sound.getBitsPerSample() == 8)
		{
			format = AL_FORMAT_STEREO8;
		}
		else
		{
			format = AL_FORMAT_STEREO16;
		}
	}

	vector<short> samples = sound.getData();
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, samples.data(), samples.size(), sound.getSampleRate());
	alSourcei(source, AL_BUFFER, buffer);

	m_cache[key] = make_pair(buffer, source);
}


void AudioEngine::playSound(const string& key)
{
	for(SoundCache::iterator it = m_cache.begin(); it != m_cache.end(); it++)
	{
		if(it->first == key)
		{
			alSourcePlay(it->second.second);
		}
	}
}
