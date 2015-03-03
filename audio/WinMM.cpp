#include "WinMM.h"

#include <windows.h>
#include <mmsystem.h>

void play_sound(const std::string& file)
{
	PlaySound((LPCSTR)file.c_str(), 0, SND_FILENAME | SND_SYNC);
}
