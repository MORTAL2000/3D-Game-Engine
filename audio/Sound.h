/*
 * Copyright 2015 Alexander Koch
 * File: Sound.h
 * Description: Class representing a sound.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
