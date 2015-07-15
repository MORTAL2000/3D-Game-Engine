#include "Sound.h"

Sound::Sound() {}

void Sound::setData(const vector<short>& data)
{
	m_data = data;
}

void Sound::setSampleRate(unsigned long sr)
{
	m_sampleRate = sr;
}

void Sound::setChannels(short c)
{
	m_channels = c;
}

void Sound::setBitsPerSample(short bits)
{
	m_bits_per_sample = bits;
}

unsigned long Sound::getSampleRate()
{
	return m_sampleRate;
}

short Sound::getChannels()
{
	return m_channels;
}

short Sound::getBitsPerSample()
{
	return m_bits_per_sample;
}

vector<short> Sound::getData()
{
	return m_data;
}

bool Sound::valid()
{
	return !m_data.empty();
}
