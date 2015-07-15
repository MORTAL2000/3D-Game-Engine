#ifndef SOUNDLOADER_H
#define SOUNDLOADER_H

#include <core/Commons.h>
#include <core/Console.h>
#include <audio/Sound.h>

#include <cstdio>
#include <string>
#include <vector>
using std::string;
using std::vector;

class SoundLoader
{
public:
	static SoundLoader& getInstance();
	Sound read(const string& filename);

private:
	SoundLoader();
};

#endif
