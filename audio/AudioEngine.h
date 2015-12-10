/*
 * Copyright 2015 Alexander Koch
 * File: AudioEngine.h
 * Description: Audio processing class.
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
