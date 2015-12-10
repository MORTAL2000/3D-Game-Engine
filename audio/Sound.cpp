/*
 * Copyright 2015 Alexander Koch
 * File: Sound.cpp
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
