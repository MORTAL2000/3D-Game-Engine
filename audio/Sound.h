#ifndef SOUND_H
#define SOUND_H

#include <vector>
using std::vector;

class Sound
{
public:
	Sound();

	void setData(const vector<short>& data);
	void setSampleRate(unsigned long sr);
	void setChannels(short c);
	void setBitsPerSample(short bits);

	unsigned long getSampleRate();
	short getChannels();
	short getBitsPerSample();
	vector<short> getData();

	bool valid();

private:
	unsigned long m_sampleRate;
	short m_channels;
	short m_bits_per_sample;
	vector<short> m_data;
};

#endif
