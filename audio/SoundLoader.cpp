/*
 * Copyright 2015 Alexander Koch
 * File: SoundLoader.cpp
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

#include "SoundLoader.h"

SoundLoader::SoundLoader() {}

SoundLoader& SoundLoader::getInstance()
{
	static SoundLoader instance;
	return instance;
}

Sound SoundLoader::read(const string& filename)
{
	Sound sound;
	vector<short> data;
	FILE* fp = fopen(filename.c_str(), "rb");
	if(!fp)
	{
		Console::log("SoundLoader: File '%s' not found", filename.c_str());
		return sound;
	}

	char id[5];
	fread(id, sizeof(char), 4, fp);
	id[4] = '\0';

	unsigned long size, format_length, sample_rate, avg_bytes_sec, data_size;
	short format_tag, channels, block_align, bits_per_sample;
	if(!strcmp(id, "RIFF"))
	{
		fread(&size, sizeof(unsigned long), 1, fp);
		fread(id, sizeof(char), 4, fp);
		id[4] = '\0';

		if(!strcmp(id, "WAVE"))
		{
			fread(id, sizeof(char), 4, fp);
			fread(&format_length, sizeof(unsigned long), 1, fp);
			fread(&format_tag, sizeof(short), 1, fp);
			fread(&channels, sizeof(short), 1, fp);
			fread(&sample_rate, sizeof(unsigned long), 1, fp);
			fread(&avg_bytes_sec, sizeof(unsigned long), 1, fp);
			fread(&block_align, sizeof(short), 1, fp);
			fread(&bits_per_sample, sizeof(short), 1, fp);
			fread(id, sizeof(char), 4, fp);
			id[4] = '\0';

			if(!strcmp(id, "data"))
			{
				fread(&data_size, sizeof(unsigned long), 1, fp);

				unsigned long length = data_size/sizeof(short);
				for(unsigned long i = 0; i < length; i++)
				{
					short idx;
					fread(&idx, sizeof(short), 1, fp);
					data.push_back(idx);
				}
			}
			else
			{
				Console::log("SoundLoader: Missing data tag in file '%s'", filename.c_str());
			}
		}
		else
		{
			Console::log("SoundLoader: File '%s' is a RIFF file, but not a WAV", filename.c_str());
		}
	}
	else
	{
		Console::log("SoundLoader: File '%s' is not a RIFF file", filename.c_str());
	}

	fclose(fp);

	sound.setBitsPerSample(bits_per_sample);
	sound.setSampleRate(sample_rate);
	sound.setChannels(channels);
	sound.setData(data);
	return sound;
}
